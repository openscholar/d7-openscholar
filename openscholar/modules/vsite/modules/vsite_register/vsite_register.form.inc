<?php

/**
 * @file
 * Form functions for Vsite Register module.
 */

/**
 * Form builder for the vsite register form found at "site/register".
 */
function vsite_register_form($form, &$state) {
  if (variable_get('is_angular', TRUE)) {
    // For sub-sites
    $param = drupal_get_query_parameters();
    $form['html'] = array(
      '#type' => 'markup',
      '#markup' => '<site-creation-form ' . (!empty($param['parent']) ? ' id="add-subsite-' . $param['parent'] . '"':''). ' auto-open="true"><a>Create Your Site Here</a></site-creation-form>',
      '#attached' => array(
        'library' => array(
          array('os_site_creation',  'site_creation')
        )
      ),
      '#angular_app' => 'SiteCreationForm',
      '#post_render' => array(
        'os_common_element_post_render'
      )
    );
    return $form;
  }
  global $base_url;
  global $user;
  $form = array();

  $anonymous = ($user->uid == 0);
  $manager = (user_access('create vsite content on behalf') && user_access('administer users'));


  $parent = NULL;
  if (isset($_GET['parent']) && $parent = vsite_get_vsite($_GET['parent'])) {
    $parent = $_GET['parent'];
  }
  $form['parent'] = array(
    '#type' => 'hidden',
    '#value' => $parent,
  );

  $form['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Your URL'),
    '#required' => TRUE,
    '#field_prefix' => $base_url . '/',
    '#size' => 35,
    '#description' => t('Enter a minimum of 3 characters, lowercase only. Periods and punctuation are not allowed except for hyphens. <br />Example: Entering jdoe into the field above will result in this URL: %site.', array('%site' => $base_url . '/jdoe')),
    '#ajax' => array(
      'callback' => 'vsite_register_ajax_validation',
      'wrapper' => 'domain-errors',
    ),
    '#field_suffix' => theme('vsite_register_form_errors', array('name' => 'domain')),
    '#attached' => array('js' => array(drupal_get_path('module', 'vsite_register') . '/vsite_register.js')),
  );

  // Gets the available vsite bundles as options for the form.
  $bundles = array();
  $group_types = og_get_all_group_bundle('node');
  $default_subsite_bundles = array('personal', 'project');
  foreach ($group_types as $bundle => $label) {
    // Verifies this user has access to create this node type.
    $user_access = user_access("create $bundle content");
    // Verifies this node type either has a parent, or doesn't need a parent.
    $subsite_bundles = variable_get('subsite_bundles', $default_subsite_bundles);
    $hierarchy_valid = (bool) (!isset($parent) || in_array($bundle, $subsite_bundles));
    // Restricts options to only valid vsite types that the user may create.
    if ($user_access && $hierarchy_valid) {
      $bundles[$bundle] = $label;
    }
  }

  // Sets the form bundle option to the default value.
  $default_bundle = 'personal';
  $default_bundle = variable_get('os_default_group_bundle', $default_bundle);
  // If the default bundle is not available among the available options,
  // Instead default to the first available bundle in the options list.
  if (!isset($bundles[$default_bundle])) {
    // Gets the bundle machine name from the options array keys.
    $default_bundle = current(array_keys($bundles));
  }

  $form['bundle'] = array(
    '#title' => t('Type of site'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $bundles,
    '#default_value' => $default_bundle,
  );

  // If there are fewer than 2 available group node types for this user,
  // then hides the select option from the form and uses the default value.
  if (count($bundles) <= 1) {
    $form['bundle']['#type'] = 'value';
    $form['bundle']['#value'] = $default_bundle;
  }
  // Otherwise adds AJAX callback to update preset value when bundle changes.
  else {
    $form['bundle']['#ajax'] = array(
      'event' => 'change',
      'wrapper' => 'preset-wrapper',
      'callback' => 'vsite_register_ajax_available_presets',
      'method' => 'replace',
    );
  }

  if (isset($state['values']['bundle'])) {
    $selected_bundle =  $state['values']['bundle'];
  }
  else {
    $selected_bundle = $default_bundle;
  }

  // Gets enabled presets and load their titles.
  $enabled_presets = array_filter(variable_get('os_enabled_spaces_presets', array()));
  $presets = spaces_preset_load();
  foreach ($enabled_presets as $key => $value) {
    $enabled_presets[$key] = $presets[$key]->title;
    // Removes presets that don't go with this bundle.
    if (isset($presets[$key]->value['bundles']) && !in_array($selected_bundle, $presets[$key]->value['bundles'])) {
      unset($enabled_presets[$key]);
    }
  }
  // Sorts presets with Minimal as default, and others alphabetical.
  if (count($enabled_presets) > 1) {
    $unsorted_presets = $enabled_presets;
    $sorted_presets = array();
    $sorted = FALSE;
    $minimal_title = ucfirst($selected_bundle) . ' Minimal';
    foreach ($unsorted_presets as $key => $value) {
      // Only makes any order changes if a "Bundle Minimal" preset exists.
      if ($value == $minimal_title) {
        $sorted = TRUE;
        // Adds the minimal value as the first option.
        $sorted_presets[$key] = t('Minimal');
        unset($unsorted_presets[$key]);
        // Sorts and adds the remaining preset options.
        asort($unsorted_presets);
        foreach ($unsorted_presets as $key => $value) {
          $sorted_presets[$key] = $value;
        }
        break;
      }
    }
    if ($sorted) {
      $enabled_presets = $sorted_presets;
    }
  }

  $default_preset = variable_get('spaces_preset_og', 'os_scholar');
  $form['preset'] = array(
    '#prefix' => '<div id="preset-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Initial setup'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => $enabled_presets,
    '#default_value' => (isset($enabled_presets[$default_preset])) ? $default_preset : current(array_keys($enabled_presets)),
  );

  // If there are fewer than 2 enabled presets for this group node type,
  // then hides the select option from the form and uses the default value.
  if (count($enabled_presets) <= 1) {
    $form['preset']['#type'] = 'value';
    $form['preset']['#value'] = $form['preset']['#default_value'];
  }

  $form['vsite_private'] = array(
    '#type' => 'radios',
    '#title' => t('Site visibility'),
    '#default_value' => VSITE_ACCESS_PUBLIC,
    '#options' => vsite_access_privacy_available_values(),
    '#description' => t('Choose who has access to view your site.'),
    '#access' => module_exists('vsite_access'),
  );

  if ($anonymous) {
    // Anonymous users must always create new accounts.
    $form += _vsite_register_user_form();
    $form['vicarious_user'] = array(
      '#type' => 'hidden',
      '#value' => FALSE,
    );
    // Prepares flag for JS to set when create new user is clicked.
    $form['user_fieldset']['create_new_user'] = array(
      '#type' => 'hidden',
      '#default_value' => TRUE,
    );
  }
  elseif ($manager) {
    // Managers may:
    // - create a new site,
    // - give a site to another user, or
    // - give a site to a new account.
    $form['vicarious_user'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create this site on behalf of another user.'),
    );

    $form['user_fieldset'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#states' => array('visible' => array(':input[name="vicarious_user"]' => array('checked' => TRUE))),
    );

    // $ajax_link = l(t('create a new user'), 'javascript:void(0)', array('attributes'=>array('id'=>array('new-user-link'))));
    // @todo figure out l() later
    $ajax_link = '';
    if (!module_exists('pinserver')) {
      $ajax_link = '  or <a href="javascript:void(0)" id="new-user-link">' . t('create a new user') . '</a>';
    }

    $form['user_fieldset']['existing_username'] = array(
      '#type' => 'textfield',
      '#title' => t('Find Existing User'),
      '#description' => t('Enter the name of the user who will manage this new site!create_new.',
        array('!create_new' => $ajax_link)
      ),
      '#autocomplete_path' => 'vsite/autocomplete/user/register',
    );

    if (module_exists('pinserver') && variable_get('pinserver_pin_url', FALSE)) {
      $form['user_fieldset']['existing_username']['#description'] .= ' ' . t('Only users who have linked their OpenScholar accounts to HarvardKey are listed.') . ' ' . l('Learn more about linking accounts', 'https://harvard.service-now.com/ithelp?id=kb_article&sys_id=bcb7c5eadbba0fc0a914fff31d9619dc', array('attributes' => array('target' => '_blank', 'rel' => 'noopener')));
    }

      // Prepares flag for JS to set when create new user is clicked.
    $form['user_fieldset']['create_new_user'] = array(
      '#type' => 'hidden',
      '#default_value' => FALSE,
    );

    $form['user_fieldset']['new_user_div'] = array(
      '#markup' => '<div id="new_user_div">',
    );

    if (!module_exists('pinserver') || !variable_get('pinserver_pin_url', FALSE)) {
      $form['user_fieldset'] += _vsite_register_user_form();
    }

    $form['user_fieldset']['huid'] = array();

    $form['user_fieldset']['new_user_close_div'] = array(
      '#markup' => '</div>',
    );
  }
  else {
    // Regular users may only create sites for themselves.
    $form['vicarious_user'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create your site'),
    '#weight' => 40,
    '#prefix' => '<div id="form-errors"></div>',
    '#suffix' => '<div id="submit-suffix"></div>',
    '#attributes' => array('class' => array('use-ajax-submit')),
    '#ajax' => array('callback' => 'vsite_register_form_submit'),
  );

  return $form;
}

/**
 * Form builder callback; builds user account section for vsite_register form.
 */
function _vsite_register_user_form() {
  $form = array();
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Desired user name'),
    '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, and underscores.'),
    '#ajax' => array(
      'callback' => 'vsite_register_ajax_validation',
      'wrapper' => 'name-errors',
    ),
    '#field_suffix' => theme('vsite_register_form_errors', array('name' => 'name')),
  );

  $form['first_name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('First name'),
  );

  $form['last_name'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Last name'),
  );

  $form['mail'] = array(
    '#type' => 'textfield',
    '#size' => 35,
    '#title' => t('Email address'),
    '#description' => t('A valid e-mail address is required. All e-mails from OpenScholar will be sent to this address.'),
    '#ajax' => array(
      'callback' => 'vsite_register_ajax_validation',
      'wrapper' => 'mail-errors',
    ),
    '#field_suffix' => theme('vsite_register_form_errors', array('name' => 'mail')),
  );

  // Prepares password field.
  $form['password'] = array(
    '#type' => 'password_confirm',
    '#size' => 35,
  );
  return $form;
}

/**
 * Form AJAX submit callback; attempts vsite register and sends messages.
 *
 * Creates user and vsite when site/register form is submitted
 */
function vsite_register_form_submit(&$form, &$state) {
  ctools_include('user', 'os');
  ctools_include('vsite', 'vsite');
  $values = &drupal_static('vsite_register_form_values');
  $values = $state['values'];

  // The site has created on the behalf of a new user.
  $new_user = FALSE;

  // If the specified user account already exists...
  if ($values['vicarious_user'] && $values['existing_username']) {
    // Loads that user account as site owner.
    $site_owner = user_load_by_name($values['existing_username']);
  }
  elseif (($values['vicarious_user'] && !$values['existing_username']) || (!$values['vicarious_user'] && $values['name'])) {
    // Create user for current logged in user or on someone else's behalf.
    $user_options = array(
      'name' => $values['name'],
      'pass' => $values['password'],
      'mail' => $values['mail'],
      'status' => 1,
      'field_first_name' => $values['first_name'],
      'field_last_name' => $values['last_name'],
    );
    $site_owner = os_user_create($user_options);

    // We created a new user. After creating the vsite we'll grant him the vsite
    // admin role.
    $new_user = TRUE;

    // Send out an email to notify the user of the newly created account.
    // Email only sent if user requested to be notified of new account.
    // if ($values['notify']) {
    //  _user_mail_notify('register_no_approval_required', $site_owner);
    // }

    // Logs in as the new user, if we're not already logged in.
    global $user;
    if ($user->uid == 0) {
      $user = $site_owner;
      user_login_finalize($state);
    }
  }
  else {
    // Creates site for current logged in user. No need to create a new user.
    global $user;
    $site_owner = $user;
  }

  // Creates the vsite node.
  $name = $purl = $values['domain'];
  $author = $site_owner->uid;
  $bundle = $values['bundle'];
  $preset = $values['preset'];
  $parent = $_POST['parent'] ? $_POST['parent'] : FALSE;
  $visibility = isset($values['vsite_private']) ? $values['vsite_private'] : FALSE;
  $state['additional_settings'] = empty($state['additional_settings']) ? array() : $state['additional_settings'];
  $vsite = vsite_create_vsite($name, $purl, $author, $bundle, $preset, $parent, $visibility, $state['additional_settings']);
  if ($vsite) {
    $message = vsite_register_message($form, $values['domain']);
    $commands[] = ajax_command_replace('#submit-suffix', $message);
    $commands[] = ajax_command_remove('#edit-submit');

    // Grant the proper roles to the user.
    if ($new_user) {
      os_role_grant($site_owner->uid, 'vsite admin', $vsite->nid);
    }

    // If we have gotten to this point, then the vsite registration was success.
    // Clears the errors.
    drupal_get_messages('error');
  }
  else {
    $commands[] = _vsite_register_form_error();
  }

  // Calls any other submit functions here.
  foreach ($form['#submit'] as $fnc) {
    if (__FUNCTION__ !== $fnc) {
      call_user_func($fnc, $form, $state);
    }
  }

  // Check for a present queued og_tasks batch.
  $batch =& batch_get();
  if ($vsite && $batch) {
    // Run all the batch commands right now.
    $batch['progressive'] = FALSE;
		batch_process();
  }

  print ajax_render($commands);
  exit;
}



/**
 * Form validate callback for vsite_register_form.
 */
function vsite_register_form_validate(&$form, &$state) {
  // Validates AJAX
  if ($_GET['q'] == 'system/ajax') {
    if ($state['triggering_element']['#ajax']['callback'] == 'vsite_register_ajax_validation' ||
      $state['triggering_element']['#ajax']['callback'] == 'iqss_pinserver_register_ajax_validation'
    ) {
      // The AJAX callback validates just this element and return a message.
      return;
    }
    elseif ($state['triggering_element']['#ajax']['callback'] != 'vsite_register_ajax_available_presets') {
      // Otherwise, processes the complete form.
      // @todo maybe this should happen after the validate? or at least duplicate its efforts.
      return vsite_register_ajax_submit_validation($form, $state);
    }
  }

  // Defaults to standard validation
  $values = $state['values'];
  foreach (vsite_register_validate_elements($values) as $element => $msg) {
    if ($msg) {
      form_set_error($element, $msg);
    }
  }

  if ($values['create_new_user']) {
    // Tells javascript to switch to new user form instead of making user switch it again.
    drupal_add_js(array('vsite_register' => array('new_user_clicked' => TRUE)), 'setting');
  }
}

/**
 * Form AJAX validate callback; Returns errors on ajax elements.
 */
function vsite_register_ajax_validation(&$form, &$state) {
  // Clears this to avoid having notices clutter the form during AJAX validate.
  drupal_get_messages();
  $elem = $state['triggering_element']['#name'];
  return theme('vsite_register_form_errors', (array(
        'name' => $elem,
        'errors' => _vsite_register_validate($elem, $state['values']),
      )));
  //$errors = form_get_errors();//$state['triggering_element']['#name']);
}

/**
 * Simply returns the value determined in the form build function.
 */
function vsite_register_ajax_available_presets($form, $form_state) {
  return $form['preset'];
}

/**
 * Validate callback; called when submit button is clicked and ajax is enabled.
 *
 * @see vsite_register_form_validate()
 */
function vsite_register_ajax_submit_validation(&$form, &$state) {
  $values = $state['values'];
  $valid = $commands = array();

  foreach (vsite_register_validate_elements($values) as $element => $msg) {
    if ($msg) {
      $html = theme('item_list', array('items' => array($msg), 'type' => 'ul', 'attributes' => array('class' => array('error'))));
      $commands[] = ajax_command_html("#${element}-errors", $html);
    }
    else {
      $valid[] = ajax_command_html("#${element}-errors", '');
    }
  }

  // Prints new errors.
  if (count($commands)) {
    print ajax_render(array_merge($commands, $valid));
    exit;
  }
}

/**
 * Form validate callback; may return array of errors.
 *
 * Errors can be handled as usual or ajax rendered.
 *
 * Exists as separate function because there's more logic than usual in choosing which elements need to validate
 * based on the state of the form.
 *
 * @param array $values
 *   Form state's values array
 *
 * @return array
 *   An indexed array keyed by element name and holding error messages.
 */
function vsite_register_validate_elements($values) {
  // Always checks domain and form.
  $elements = array('domain', 'form');

  // Checks new user info.
  if ($values['create_new_user']) {
    $elements = array_merge($elements, array('name', 'mail', 'password'));
  }

  // Checks existing user info.
  if ($values['vicarious_user'] && !$values['create_new_user']) {
    $elements = array_merge($elements, array('existing_username'));
  }

  $msgs = array();
  foreach ($elements as $element) {
    if ($errors = _vsite_register_validate($element, $values)) {
      $msgs[$element] = implode(' ', $errors);
    }
  }

  return $msgs;
}

/**
 * Form validate callback; checks all common values.
 *
 * Invoked in both:
 *  - vsite_register_ajax_validation()
 *  - vsite_register_form_validate()
 *
 * @param string $element
 * 	Name of element to validate
 * @param array $values
 * 	values array from returned form state
 *
 * @return mixed
 *   NULL if no errors, otherwise an array of strings for all errors found.
 */
function _vsite_register_validate($element, &$values) {
  $msg = array();
  switch ($element) {
    case 'domain':
      if (strlen($values['domain']) < 3 || !valid_url($values['domain']) || !_vsite_register_valid_url($values['domain'])) {
        $msg[] = t('Invalid URL specified.');
      }
      if (($purl = purl_load(array('value' => $values['domain'], 'provider' => 'spaces_og'), TRUE)) || menu_get_item($values['domain'])) {
        $msg[] = t('URL %url is taken.  Please choose another.', array('%url' => $values['domain']));
      }
      break;

    case 'mail':
      if ($mail_error = user_validate_mail($values['mail'])) {
        $msg[] = $mail_error;
      }

      if (_vsite_register_mail_in_use($values['mail'])) {
        $msg[] = t('Email address already in use.  Please try another.');
      }
      break;

    case 'name':
      if ($user_error = user_validate_name($values['name'])) {
        $msg[] = $user_error;
      }

      if ($user = user_load_by_name($values['name'])) {
        $msg[] = t('Username %name is taken.  Please choose another.', array('%name' => $values['name']));
      }
      break;

    // Checks that all dependent fields are provided.

    case 'form':
      // If "on behalf" is selected, but no user specified...
      if ($values['vicarious_user'] && !(strlen($values['existing_username']) || $values['create_new_user'])) {
        $msg[] = t('You have selected to create this site on behalf of another user.  Please specify an existing user or create a new account.');
      }

      // If "create new user" is selected...
      if ($values['create_new_user']) {
        if (!(strlen($values['name']) && strlen($values['mail']) && strlen($values['password']))) {
          $msg[] = t('You have selected to create a new user.  Please provide a username, email address, and password for the new user.');
        }

        // Checks password matches confirmed password.
        $pass1 = trim($_POST['password']['pass1']);
        $pass2 = trim($_POST['password']['pass2']);
        if (!empty($pass1) || !empty($pass2)) {
          if (strcmp($pass1, $pass2)) {
            $msg[] = t('The specified passwords do not match.');
          }
        }
        else {
          $msg[] = t('The password field is required.');
        }

        if (module_exists('password_policy')) {
          //borrowed from `password_policy_user_profile_form_validate()`
          $account = user_load(0);
          $policies = PasswordPolicy::matchedPolicies($account);
          $errors = array();
          foreach ($policies as $policy) {
            $errors = $errors + $policy->check($pass1, $account);
          }
          $msg = array_merge($msg, $errors);
        }
      }

      // If an existing user is selected...
      if ($values['vicarious_user'] && !$values['create_new_user']) {
        if (!user_load_by_name($values['existing_username'])) {
          $msg[] = t('User @name does not exist.  Please try another user name or create a new user account instead.', array('@name' => $values['existing_username']));
        }
      }
      break;
  }

  return (count($msg)) ? $msg : NULL;
}

/**
 * Returns TRUE if an email address is already in use by another user.
 *
 * Code borrowed from user.module's user_account_form_validate()
 *
 * @param string $mail
 * 	The email address a visitor is attempting to register a new account with.
 *
 * @see user_account_form_validate()
 */
function _vsite_register_mail_in_use($mail) {
  return (bool) db_select('users')
    ->fields('users', array('uid'))
    ->condition('mail', db_like($mail, 'LIKE'))
    ->range(0, 1)
    ->execute()
    ->fetchField();
}

/**
 * Returns TRUE if a URL has no restricted characters and passes purl validate.
 *
 * @see _vsite_register_validate()
 * @see purl_validate()
 */
function _vsite_register_valid_url($url) {
  foreach (array('!', ';', '/', '?', ':', '@', '&', '=', '"', "'", ".", '_',
    ) as $char) {
    if (strpos($url, $char) !== FALSE) {
      return FALSE;
    }
  }

  return purl_validate(array('value' => $url, 'provider' => 'spaces_og'));
}

/**
 * Sends emails using drupal_mail() to both new site owner and admin account.
 */
function vsite_register_notify($vsite) {
  // Invokes drupal_mail() to send email notification to new site owner.
  vsite_register_notify_author($vsite);

  // Invokes drupal_mail() to send email notification to admin account,
  // or whatever email is specified in variable "vsite_register_admin_mail".
  $admin_mail = variable_get('vsite_register_admin_mail', '');
  if ($admin_mail) {
    vsite_register_notify_admin($vsite, $admin_mail);
  }
}

/**
 * Form submit AJAX callback; sends out an email to the vsite owner user.
 *
 * Notifies the vsite owner of their newly created site.
 *
 * @see vsite_register_form_submit()
 */
function vsite_register_notify_author($vsite) {
  // Prepares replacement values for email token text.
  $key = 'vsite_register_notify_author';
  $author = user_load($vsite->group->uid);
  $replacements = _vsite_register_mail_replacements($vsite, $author, $key);

  $subject = t('An OpenScholar website has been created for you.');

  // Checks to see if the user logged in matched the newly created user.
  global $user;
  $body_text = _vsite_register_notify_author_body_text($user, $author);
  $body = t($body_text, $replacements);

  global $language;

  $params = array();
  $params['vsite'] = $vsite;
  $params['user'] = $author;
  $params['subject'] = $subject;
  $params['body'] = $body;

  $from = variable_get('site_mail', ini_get('sendmail_from'));

  $result = drupal_mail('vsite_register', 'vsite_register_notify_author', $author->mail, $language->language, $params, $from, $send = TRUE);
  if ($result == FALSE) {
    drupal_set_message(t('Error sending notification mail to user.'), 'error');
  }
}

/**
 * Form submit AJAX callback; sends out a 2nd email to the administrator.
 *
 * Notifies the administrator that a new site has been registered.
 *
 * @see vsite_register_form_submit()
 */
function vsite_register_notify_admin($vsite, $admin_mail) {
  // Prepares replacement values for email token text.
  $key = 'vsite_register_notify_admin';
  $author = user_load($vsite->group->uid);
  $replacements = _vsite_register_mail_replacements($vsite, $author, $key);

  $subject = t(
    "User !username created a site \"!site_name\" at !sitewide_name",
    $replacements
  );

  $body = t(
      "Hello,\n\n"
      . "On !date,\na new site was registered at !sitewide_name.\n\n"
      . "Link: !site_login_uri"
      . "\n\n&nbsp;\n\n"
      . "<h2>Site information</h2>\n"
      . "Site name:\t\t!site_name\n"
      . "Site URL:\t\t!site_url\n"
      . "Site type:\t\t!site_type\n"
      . "Site visibility:\t!site_visibility"
      . "\n\n&nbsp;\n\n"
      . "<h2 style='font-weight:bold;'>User information</h2>\n"
      . "Username:\t\t!username\n"
      . "Full name:\t\t!full_name\n"
      . "Email:\t\t\t\t!user_mail\n"
      . "Total sites owned:\t!vsite_count\n\n"
      . "Best wishes,\nthe !sitewide_name server",
      $replacements
  );

  $from = variable_get('site_mail', ini_get('sendmail_from'));

  $params = array();
  $params['subject'] = $subject;
  $params['body'] = $body;
  $params['vsite'] = $vsite;
  $params['user'] = $author;

  global $language;
  $result = drupal_mail('vsite_register', $key, $admin_mail, $language->language, $params, $from, $send = TRUE);
  if ($result == FALSE) {
    drupal_set_message(t('Error sending notification mail to admin.'), 'error');
  }
}

/**
 * Implements hook_mail().
 */
function vsite_register_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];

  // Prepares values to be used with any hook_mail_alter() implementations.
  $message['vsite'] = $params['vsite'];
  $message['user'] = $params['user'];
}

/**
 * Returns an array of string replacement values for vsite register mails.
 *
 * @param object $vsite
 *   The newly created vsite object, whose link, etc. will be a replacement val.
 * @param string $key
 *   The mail key used by drupal_mail(), to load certain variables.
 */
function _vsite_register_mail_replacements($vsite, $author, $key) {
  global $base_url;
  $purl_base_domain = variable_get('purl_base_domain', $base_url);
  // Initialize replacements array with values common to both mail keys.
  $site_url = '';
  if ($vsite->purl_provider) {
    $site_url = url(NULL, array(
      'absolute' => TRUE,
      'purl' => array(
        'id' => $vsite->id,
        'provider' => $vsite->purl_provider ? $vsite->purl_provider : 'spaces_og'
      ),
    ));
  }
  else {
    $method = variable_get('purl_method_spaces_og', 'path');

    $processor = purl_get_processor($method);
    $element = new purl_path_element($processor, $vsite->group->purl, 'spaces_og', $vsite->id);
    $options = array('absolute' => TRUE);
    $element->processor->rewrite($site_url, $options, $element);

    if (!empty($options['base_url'])) {
      $site_url_base = empty($site_url) && !empty($vsite->group->purl) ?
        $vsite->group->purl : '';
      $site_url = trim($options['base_url'], '/');

      if (variable_get('purl_method_spaces_og', NULL) !== 'thescholr_subdomain') {
        $site_url .= '/' . ltrim($site_url_base, '/');
      }
    }
  }
  $replacements = array(
    '!username' => $author->name,
    '!site_login_uri' => url($purl_base_domain . '/user', array('absolute' => TRUE, 'query' => array('destination' => 'node/'.$vsite->id))),
    '!site_uri' => url($purl_base_domain . '/user', array('absolute' => TRUE, 'query' => array('destination' => 'node/'.$vsite->id))),
    '!user_mail' => $author->mail,
    '!site_url' => $site_url,
    '!sitewide_name' => variable_get('site_name', 'OpenScholar'),
  );

  // Adds additional replacement values depending on which email is being sent.
  switch ($key) {
    case 'vsite_register_notify_admin':

      // Prepares vsite count value (total number of vsites owned by author).
      $count = count(vsite_get_vsite_by_owner($author->uid));

      // Prepares the new vsite owner (user)'s full name.
      $name_values = array();
      $name_fields = array('field_first_name', 'field_last_name');
      foreach ($name_fields as $field) {
        $items = field_get_items('user', $author, $field);
        if (isset($items[0]['value']) && !empty($items[0]['value'])) {
          $name_values[] = $items[0]['value'];
        }
      }
      $full_name = sizeof($name_values) > 1 ? implode(' ', $name_values) : t('(No entry)');

      // Prepares the site visibility value and fetches human-readable name.
      $items = field_get_items('node', $vsite->group, VSITE_ACCESS_FIELD);
      $visibility = (isset($items[0]['value'])) ? $items[0]['value'] : FALSE;
      if ($visibility !== FALSE) {
        $values = vsite_access_privacy_available_values();
        if (isset($values[$visibility])) {
          $visibility = $values[$visibility];
        }
      }

      // Prepares a site type like "department (os_department_minimal preset)".
      $site_type = isset($vsite->group->type) ? $vsite->group->type : '';
      if ($site_type) {
        if (isset($vsite->controllers->variable->values['space']['spaces_preset_og'])) {
          $preset = $vsite->controllers->variable->values['space']['spaces_preset_og'];
          $site_type .= " ($preset preset)";
        }
      }

      // Prepares the date for the email message.
      global $language;
      $date = format_date(time(), 'long', '', NULL, $language->language);

      $replacements += array(
        '!vsite_count' => $count,
        '!full_name' => $full_name,
        '!site_name' => $vsite->group->title,
        '!site_type' => $site_type,
        '!site_visibility' => $visibility,
        '!date' => $date,
      );

      break;
    case 'author_notify':
      $replacements += array(
        '!user_edit_uri' => url('user/' . $author->uid . '/edit', array(
          'absolute' => TRUE
        )),
        '!user_login_uri' => url('user', array(
          'absolute' => TRUE
        )),
      );
      break;
  }

  return $replacements;
}

/**
 * Returns special instructions for resetting password in certain cases.
 *
 * @param object $current_user
 *   The currently logged-in user.
 * @param object $author
 *   The newly registered vsite's author.
 *
 * @return string $body_text
 *   The email body message text to return, optionally containing instructions
 *   to reset password.
 */
function _vsite_register_notify_author_body_text($current_user, $author) {
  // Checks if the site was created "vicariously", on behalf of author.
  $author_is_current_user = (bool) $current_user->uid == $author->uid;
  // Checks if user is creating first site or additional site.
  $returning_author = (bool) count(vsite_get_vsite_by_owner($author->uid)) > 1;
  // In either of these cases, no password reset is required.
  $no_reset_required = (bool) $returning_author || $author_is_current_user;
  if ($no_reset_required) {
    $body_text = "Hi !username,\n\n"
      . "Your new OpenScholar site has been activated. "
      . "Your website url is !site_url\n\n"
      . "You will be able to log into your site at !site_login_uri "
      . "with username:\n\n!username\n\n"
      . "We hope you will enjoy your new web site.\n"
      . "\n"
      . " * Logging in to your site: https://help.theopenscholar.com/logging-your-site\n"
      . " * OpenScholar documentation: https://help.theopenscholar.com \n"
      . "\n"
      . "Sincerely,\nThe OpenScholar Team";
  }
  // Otherwise, this site was created on the author's behalf, or this is the
  // author's very first site.
  // Includes instructions to reset password in order to login.
  else {
    $body_text = "Hi !username,\n\n"
      . "Your new web site at OpenScholar has been activated.\n\n"
      . "Your website url is !site_url\n\n"
      . "You may now log in and reset your password by clicking on this link "
      . "or copying and pasting it in your browser: \n\n !user_login_url \n\n"
      . "This login can be used only once. "
      . "After logging in, you will be redirected to !user_edit_url "
      . "so you can change your password. "
      . "Once you have set your own password, "
      . "you will be able to log in at !site_login_uri "
      . "as username:\n\n!username\n\n"
      . "We hope you will enjoy your new web site.\n"
      . "\n"
      . " * Logging in to your site: https://help.theopenscholar.com/logging-your-site\n"
      . " * OpenScholar documentation: https://help.theopenscholar.com \n"
      . "\n"
      . "Sincerely,\nThe !sitewide_name Team";
  }

  drupal_alter('vsite_register_notify_author_body_text', $body_text);
  return $body_text;
}

function vsite_register_validate($type, $value) {
  $output = array(
    'valid' => false,
  );

  $values[$type] = $value;
  $msg = _vsite_register_validate($type, $values);
  if (is_array($msg)) {
    $output['message'] = $msg;
  }
  else {
    $output['valid'] = true;
  }

  print drupal_json_encode($output);
  exit;
}

function vsite_register_site_login($form, $form_state, $site_id, $uid, $timestamp, $token) {
  global $user;
  $space = spaces_load('og', $site_id);
  $purl_params = array(
    'purl' => array(
      'provider' => "spaces_{$space->type}",
      'id' => $space->id,
    ),
    'fragment' => 'overlay=cp/welcome'
  );

  // When processing the one-time login link, we have to make sure that a user
  // isn't already logged in.
  if ($user->uid) {
    // The existing user is already logged in.
    if ($user->uid == $uid) {
      purl_goto('<front>', $purl_params);
    }
    // A different user is already logged in on the computer.
    else {
      $reset_link_account = user_load($uid);
      if (!empty($reset_link_account)) {
        drupal_set_message(t('Another user (%other_user) is already logged into the site on this computer, but you tried to use a one-time link for user %resetting_user. Please <a href="!logout">logout</a> and try using the link again.',
          array('%other_user' => $user->name, '%resetting_user' => $reset_link_account->name, '!logout' => url('user/logout'))));
      } else {
        // Invalid one-time link specifies an unknown user.
        drupal_set_message(t('The one-time login link you clicked is invalid.'));
      }
    }
    drupal_goto();
  }
  else {
    // Time out, in seconds, until login URL expires. Defaults to 24 hours =
    // 86400 seconds.
    $timeout = variable_get('user_password_reset_timeout', 86400);
    $current = REQUEST_TIME;
    // Some redundant checks for extra security ?
    $users = user_load_multiple(array($uid), array('status' => '1'));
    if ($timestamp <= $current && $account = reset($users)) {
      // No time out for first time login.
      if ($account->login && $current - $timestamp > $timeout) {
        drupal_set_message(t('You have tried to use a one-time login link that has expired. You must log in through the Admin Login link.'));

        purl_goto('<front>', $purl_params);
      }
      elseif ($account->uid && $timestamp >= $account->login && $timestamp <= $current && $token == user_pass_rehash($account->pass, $timestamp, $account->login)) {
        // First stage is a confirmation form, then login
        // Set the new user.
        $user = $account;
        // user_login_finalize() also updates the login timestamp of the
        // user, which invalidates further use of the one-time login link.
        user_login_finalize();
        watchdog('user', 'User %name used one-time login link at time %timestamp.', array('%name' => $account->name, '%timestamp' => $timestamp));
        drupal_set_message(t('You have just used your one-time login link. You will no longer be able to use this link to log in.'));

        purl_goto('<front>', $purl_params);
      }
      else {
        drupal_set_message(t('You have tried to use a one-time login link that has either been used or is no longer valid. You must log in through the Admin Login link.'));

        purl_goto('<front>', $purl_params);
      }
    }
    else {
      // Deny access, no more clues.
      // Everything will be in the watchdog's URL for the administrator to check.
      drupal_access_denied();
    }
  }
}

function vsite_register_user_exists($name) {
  $output = array(
    'uid' => 0,
  );
  if ($account = user_load_by_name($name)) {}
  elseif ($account = user_load_by_mail($name)) {}

  if ($account) {
    $output['uid'] = $account->uid;
  }

  echo drupal_json_encode($output);
  exit;
}
