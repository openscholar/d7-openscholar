<?php
include_once (drupal_get_path('module', 'vsite_register') . '/vsite_register.form.inc');

/**
 * Menu callback; provides vsite_register_form for authorized users.
 */
function os_pinserver_auth_vsite_register_form_page() {
  global $user;

  // Handle users not logged in already.
  if (!$user->uid){

    // Sends user to pinserver if not logged-in locally or via the pinserver
    if (!pinserver_check_status()) {
      return pinserver_redirect(array('redirect' => 'site/register'));
    }

    // Redirects pinserver-logged-in user if they don't own a vsite.
    if ($uid = pinserver_authenticate_get_uid_from_huid()) {
      // Logs the user in based on the HUID->UID association.
      $user = user_load(array('uid' => $uid));
      $user->login = time();
      db_update('users')->fields(array('login' => $user->login))->condition('uid', $user->uid, '=')->execute();
    }
  }
  else {
    if(pinserver_user_has_associated_pin($user->uid) || $user->uid == 1) {
      return drupal_get_form('vsite_register_form');
    }
    else {
      drupal_set_message(t('Only users who have linked their OpenScholar accounts to HarvardKey are eligible to create a new site.') . ' ' . l('Learn more about linking accounts', 'https://harvard.service-now.com/ithelp?id=kb_article&sys_id=bcb7c5eadbba0fc0a914fff31d9619dc', array('attributes' => array('target' => '_blank', 'rel' => 'noopener'))), 'error', FALSE);
      drupal_goto('/user');
    }
  }

  //This will redirect the user to their site or return TRUE if they can create a site
  if(_os_pinserver_auth_create_vsite_redirect()){
    return drupal_get_form('vsite_register_form');
  }
}


/**
 * Helper function to check whether user has permission to create a new.
 */
function _os_pinserver_auth_vsite_register_form_page() {
  global $user;

  // Handle users not logged in already.
  if (!$user->uid){

    // Sends user to pinserver if not logged-in locally or via the pinserver
    if (!pinserver_check_status()) {
      return false;
    }

    // Redirects pinserver-logged-in user if they don't own a vsite.
    if ($uid = pinserver_authenticate_get_uid_from_huid()) {
      // Logs the user in based on the HUID->UID association.
      $user = user_load(array('uid' => $uid));
      $user->login = time();
      db_update('users')->fields(array('login' => $user->login))->condition('uid', $user->uid, '=')->execute();
    }
  }
  else {
    if(pinserver_user_has_associated_pin($user->uid) || $user->uid == 1) {
      return true;
    }
    else {
      return false;
    }
  }

  //This will redirect the user to their site or return TRUE if they can create a site
  if(_os_pinserver_auth_create_vsite_redirect()){
    return true;
  }
  return false;
}

/**
 * Access callback for /[site|user]/register
 */
function os_pinserver_auth_register_access() {
  // Just override vsite_vsite_exists_access() -- handle actual access check in
  // os_pinserver_auth_vsite_register_form_page().
  return variable_get('os_pinserver_registration', FALSE);
}

/**
 * Alter for vsite_register_form.
 */
function _os_pinserver_auth_vsite_register_form_alter(&$form, $form_state, $form_id) {

  // Attaches submit and validate callbacks
  $form['#submit'][] = 'os_pinserver_auth_register_submit';
  $form['#validate'][] = 'os_pinserver_auth_register_validate';
}

/**
 * Alter for vsite_users_profile_node_form.
 */
function _os_pinserver_auth_vsite_users_profile_node_form_alter(&$form, $form_state, $form_id) {
  if (!isset($form['buttons']['submit'])) {
    return;
  }

  // Overrides submit function from vsite_users to prevent 'home' redirect.
  $key = array_search('vsite_users_form_vsite_users_profile_node_submit', $form['buttons']['submit']['#submit']);
  if ($key) {
    $form['buttons']['submit']['#submit'][$key] = 'os_pinserver_auth_register_vsite_users_profile_node_submit';
  }
}

/**
 * Validation callback; for vsite_register_form.
 */
function os_pinserver_auth_register_validate($form, &$form_state) {
  global $user;

  // Ajax validation.
  if ($_GET['q'] == 'system/ajax') {
    if ($form_state['triggering_element']['#ajax']['callback'] == 'vsite_register_ajax_validation' ||
      $form_state['triggering_element']['#ajax']['callback'] == 'os_pinserver_auth_register_ajax_validation') {
      // Let AJAX handler deal with validating this element.
      return;
    }
    else {
      // Someone submitted the form; must validate all.
      return os_pinserver_auth_register_ajax_submit_validation($form, $form_state);
    }
  }

  // Fall back to standard validation.
  foreach (os_pinserver_auth_register_validate_elements($form_state['values']) as $element => $msg) {
    if ($msg) {
      form_set_error($element, $msg);
    }
  }

  return;
}

/**
 * When form is submitted with AJAX enabled, also run this validation.
 */
function os_pinserver_auth_register_ajax_submit_validation(&$form, &$form_state) {
  global $user;

  $commands = $valid = array();

  foreach(os_pinserver_auth_register_validate_elements($form_state) as $element => $msg) {
    if ($msg) {
      $commands[] = ajax_command_html("#${element}-errors", theme('item_list', array('items'=>array($msg), 'type'=>'ul', 'attributes'=>array('class'=>array('error')))));
  }
  else {
    $valid[] = ajax_command_html("#${element}-errors", '');
	}
	}

	//print new errors
	if (count($commands)) {
	  $generic_error = array(ajax_command_html('#form-errors', theme('item_list', array('items'=>array('There were errors creating your website. Please look above to correct them.'), 'type'=>'ul', 'attributes'=>array('class'=>array('error'))))));
	  print ajax_render(array_merge($commands, $valid, $generic_error));
	  exit;
	}

	return;
}

/**
 * Submit callback; for vsite_register_form.
 */
function os_pinserver_auth_register_submit($form, &$form_state) {
  global $user;

  $uid = $user->uid;
  $huid = NULL;

  // For accounts created by admin, get the new UID of the account we just created.
  if(isset($form_state['values']['name'])) {
    $uid = user_load_by_name($form_state['values']['name'])->uid;
  }

  // Look for HUID. Prefer form value over session value.
  if(isset($form_state['values']['huid']) && !empty($form_state['values']['huid']) && is_numeric($form_state['values']['huid'])) {
    $huid = $form_state['values']['huid'];
  }
  else {
    $huid = pinserver_get_user_huid();
  }

  // Create UID->HUID association in database if UID and HUID are present.
  if (!empty($uid) && !empty($huid) && !pinserver_authenticate_set_user_huid($uid, $huid)) {
    error_log('WARNING: The os_pinserver_auth_register_submit function could not add the UID->HUID association to the database');
  }

  // Removes pinserver session vars.
  pinserver_remove_session();
}

/**
 * Validate all form elements.
 */
function os_pinserver_auth_register_validate_elements(&$form_state) {
  $errors = array();

  // Validate HUID.
  $huid_errors = os_pinserver_auth_register_validate_huid($form_state['values']['huid']);
  if(isset($huid_errors[0])) {
    $errors['huid'] = $huid_errors[0];
  }

  // Check if user can create vsites.
  if ($GLOBALS['user']->uid && !user_access('create vsite content on behalf') && count(vsite_get_vsite_by_owner($user->uid)) >= variable_get('os_vsites_per_user')) {
    $errors['form'] = t('You do not have permission to create more websites.');
  }

  return $errors;
}

/**
 * AJAX callback; for huid field
 */
function os_pinserver_auth_register_ajax_validation(&$form, &$form_state) {
  // clear this to avoid having notices clutter the form during ajax validation
  drupal_get_messages();
  return theme('vsite_register_form_errors', array(
    'name' => $form_state['triggering_element']['#name'],
    'errors' => os_pinserver_auth_register_validate_huid($form_state['values']['huid']),
  ));
}

/**
 * Checks if HUID is numeric and is a new unique ID number.
 *
 * @param int $huid
 *   The HUID to validate
 *
 * @return array $errors
 *   Empty if no errors.
 */
function os_pinserver_auth_register_validate_huid($huid) {
  $errors = array();

  // Empty HUID field is okay.
  if (empty($huid)) {
    return $errors;
  }

  // Verifies HUID is numeric.
  else if (!is_numeric($huid)) {
    $errors[] = t('The @harvard_id must be numeric.', array('@harvard_id' => 'Harvard ID'));
  }

  // Verifies HUID does not already exist in database.
  else if (pinserver_authenticate_get_uid_from_huid($huid)) {
    $errors[] = t('The @harvard_id you have entered already exists in the database.', array('@harvard_id' => 'Harvard ID'));
  }

  return $errors;
}

/**
 * Submit callback; overrides vsite_users.module to prevent 'home' redirect.
 */
function os_pinserver_auth_register_vsite_users_profile_node_submit($form, &$form_state) {
  if (isset($form_state['redirect'])) {
    unset($form_state['redirect']);
  }

  // Customizes confirmation message for add/edit form
  drupal_get_messages($type = NULL, $clear_queue = TRUE);
  drupal_set_message(t('Your personal information has been saved.'));
}

/**
 * Check if user can create vsites or not, or requested a private file, and redirect appropriately.
 */
function _os_pinserver_auth_create_vsite_redirect() {
  global $user;

  // Redirect user to requested private file (or other private destination).
  if (isset($_GET['redirect'])) {
    // Strip superfluous preceeding '/' before redirecting.
    return drupal_goto(preg_replace('|^/|', '', $_GET['redirect']));
  }

  $vsites = vsite_get_vsite_by_owner($user->uid);

  // Can user create vsites or not?
  if (vsite_vsite_exists_access($vsites)) {
    // Can create new vsites; provide form to create a new vsite.
    return TRUE;
  }
  else {
    // Can't create new sites; redirect user to existing vsite(s).
    return _os_pinserver_auth_existing_vsite_redirect($vsites);
  }

  return FALSE;  // This shouldn't happen.
}

/*
 * Redirect to the proper destination after failing to create a vsite.
*/
function _os_pinserver_auth_existing_vsite_redirect($vsites = array()) {
  // First checks if the reason for access denied was due to domain mismatch.
  global $base_url;
  $purl_base_domain = variable_get('purl_base_domain');
  // If there was a domain mismatch...
  if ($base_url != $purl_base_domain) {
    // Redirects to custom vsite's front page.
    drupal_goto();
  }

  // Alerts the user that they have reached the maximum.
  $sites = format_plural(count($vsites), '1 site', '@count sites');
  drupal_get_messages();  // Clear message queue.
  drupal_set_message(t('You have already registered your maximum number of @sites.', array(
  '@sites' => $sites,
  )), 'warning');

  // Redirects user to existing vsite(s) if available.
  if (count($vsites) == 1) {
    drupal_goto($vsites[0]->get_absolute_url());
  }
  // Otherwise redirects to the user page, with the list of all a user's sites.
  else {
    drupal_goto('user');
  }
}