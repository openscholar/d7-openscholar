<?php

/**
 * Returns an array of links for this user in the Manage Members table.
 *
 * @param int $uid
 *   The specified user object's User ID.
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 *
 * @return array $columns
 */
function cp_user_get_member_rows($gid) {
  $rows = array();

  // Defines end-user text for these values.
  $default_status = t('Active');
  $owner_role = '<strong>' . t('Site owner') . '</strong>';
  $status_edit_text = t('remove');
  $change_site_owner_text = t('Change owner');

  $vsite = spaces_get_space();

  global $user;

  // Checks if the logged user is the VSite owner or can change owner.
  $can_change_owner = ((bool) ($vsite->group->uid == $user->uid)) || user_access('administer group');

  // Load the members accounts.
  $accounts = user_load_multiple(cp_user_get_member_uids($gid));

  foreach ($accounts as $account) {

    // Loads the user's full name-- first and last name.
    $name = _cp_user_get_full_name($account);

    // Checks if the account belongs to the VSite's owner.
    $is_owner = (bool) ($vsite->group->uid == $account->uid);

    // If the user is the owner, return a special row with no edit links.
    if ($is_owner) {
      $role_edit_text = '';
      $role = $owner_role;
      $status_edit_link = '';

      $options = array('attributes' => array('class' => array('cp-user-change-site-owner')));
      $change_ownership = $can_change_owner ? l($change_site_owner_text, "cp/users/change_site_owner", $options) : '';

      $options = array('attributes' => array('class' => array('cp-user-float-right')));
      $role_edit_link = l($role_edit_text, "cp/users/edit_membership/{$account->uid}", $options);
    }

    // Otherwise, checks the current user's OG roles.
    // The member's role could be one of: Vsite admin, Content Editor, or Member.
    else {
      $role_edit_text = t('edit');
      $change_ownership = '';
      $human_name = TRUE;
      $options = array('attributes' => array('class' => array('cp-user-float-right')));

      $membership = og_get_membership('node', $gid, 'user', $account->uid);
      if ($membership->type == 'support_expire') {  // is there a constant for this I should be using?
        $role = t('Support User');
        $expire = strtotime('+'.variable_get('vsite_support_expire', '3 days'), $membership->created);
        $remaining = $expire - REQUEST_TIME;
        $role_edit_link = '<span class="cp-user-float-right">No longer member in '
          .(($remaining > 86400)?date('j', $remaining).' days.':date('G', $remaining).' hours.')
          .'</span>';
      }
      else {
        $role = _cp_user_get_role($account->uid, $vsite->group->nid, $human_name);
        $role_edit_link = l($role_edit_text, "cp/users/edit_membership/{$account->uid}", $options);
      }
      $status_edit_link = l($status_edit_text, "cp/users/delete_membership/{$account->uid}", $options);
    }

    // Prepare the row to be used by theme_table().
    // @see cp_user_user_management_page()
    $row = array(
      'name' => $name,
      'username' => $account->name,
      'role' => $role . $role_edit_link . $change_ownership,
      'status' => $default_status . $status_edit_link,
    );

    $rows[] = $row;
  }

  return $rows;
}

/**
 * Returns an array containing the given vsite's members User IDs.
 *
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 *
 * @return array $members
 *   The list of members' User IDs in the specified vsite.
 */
function cp_user_get_member_uids($gid) {
  $query = new EntityFieldQuery();
  $results = $query
    ->entityCondition('entity_type', 'og_membership')
    ->propertyCondition('gid', $gid)
    ->propertyCondition('entity_type', 'user')
    ->execute();

  if (empty($results['og_membership'])) {
    // No members.
    return array();
  }

  $og_memberships = og_membership_load_multiple(array_keys($results['og_membership']));

  $member_uids = array();

  foreach ($og_memberships as $og_membership) {
    $member_uids[$og_membership->etid] = $og_membership->etid;
  }

  return $member_uids;
}

/**
 * Page callback; returns content for the CP > People page (Manage Members).
 *
 * @param int $gid
 *   The specified vsite object's OG group Node ID.
 */
function cp_user_user_management_page($gid) {
  // Prepares end user strings for the page.
  $add_new_user_link_title = t('+ Add a member');
  $header = array(t('Name'), t('Username'), t('Role'), t('Status'));
  $rows = cp_user_get_member_rows($gid);
  $attached_css = drupal_get_path('module', 'cp_user') . '/cp_user.css';

  $build = array(
    'cp_user' => array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => 'cp-user',
        'class' => array('cp-manage-users-wrapper'),
      ),
      'cp_user_actions' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('cp-add-new-user')
        ),
        'cp_user_add_new' => array(
          '#theme' => 'links',
          '#links' => array(
            array(
              'title' => $add_new_user_link_title,
              'href' => 'cp/users/add',
              'attributes' => array(
                'class' => array('os-green-button', 'cp-user-float-right'),
              ),
            ),
          ),
        ),
        '#weight' => 0,
      ),
      'cp_user_main' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('cp-manager-user-content')
        ),
        'cp_user_list' => array(
          '#theme' => 'table',
          '#header' => $header,
          '#rows' => $rows,
        ),
        '#weight' => 1,
      ),
    ),
    '#attached' => array(
      'css' => array(
        'type' => 'file',
        'data' => $attached_css,
      ),
    ),
  );

  return $build;
}

/**
 * Form builder; provides confirmation for "Remove Member" action.
 */
function cp_user_remove_membership_confirm($form_state, $user_load) {
  $space = spaces_get_space();
  // Form sets.
  $user = $user_load['build_info']['args'][0];
  $user_name = _cp_user_get_full_name($user);
  $site_name = $space->group->title;
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['uid'] = array('#type' => 'value', '#value' => $user->uid);
  $form['user_name'] = array('#type' => 'value', '#value' => $user_name);
  $form['site_name'] = array('#type' => 'value', '#value' => $site_name);

  // Prepares necessary values for confirm_form().
  $user_name = $user_load['build_info']['args'][0]->name;

  $question = t('Remove member <em>@user</em> from site <em>@site</em>?',
    array('@user' => $user_name, '@site' => $site_name));
  $path = 'cp/users';
  $description = t('You may add a removed member back to your site at any time.');
  $yes = t('Remove');
  $no = t('Cancel');

  return confirm_form($form, $question, $path, $description, $yes, $no);
}

/**
 * Submit callback; removes the specified member after user confirms action.
 */
function cp_user_remove_membership_confirm_submit($form, &$form_state) {
  global $user;
  // Ungroup the user from the node
  $gid = $form_state['values']['gid'];
  $uid = $form_state['values']['uid'];
  // Obtaining delete user's og role, e.g.Basic member / Content editor / Administrator / Viewer.
  $deleted_member_rid = array_keys(og_get_user_roles('node', $gid, $uid, FALSE));
  $roles_human_names = _cp_user_list_roles('rids-human', NULL, $gid);
  og_ungroup('node', $gid, 'user', $uid);

  $user_name = $form_state['values']['user_name'];
  $site_name = $form_state['values']['site_name'];
  // Set the message and redirect back to the cp/users page
  drupal_set_message(t('Member <em>@user</em> has been removed from <em>@site</em>', array('@user' => $user_name, '@site' => $site_name)));

  $vsite = vsite_get_vsite();
  // Loading logged in user's user object for full name.
  $account_modifier = user_load($user->uid);
  $account_modifier_full_name = $account_modifier->field_first_name[LANGUAGE_NONE][0]['value'] . ' ' . $account_modifier->field_last_name[LANGUAGE_NONE][0]['value'];
  $purl_base_domain = variable_get('purl_base_domain');
  $account = user_load($form_state['values']['uid']);
  // Checking if this vsite has a custom domain or not.
  $custom_domain = db_select('purl', 'p')
    ->fields('p', array('value'))
    ->condition('provider', 'vsite_domain', '=')
    ->condition('id', $vsite->group->nid, '=')
    ->execute()
    ->fetchField();

  // Setting up email parameters.
  $to = $account->mail;
  $from = variable_get('site_mail', ini_get('sendmail_from'));
  $params['!user-role'] = $roles_human_names[$deleted_member_rid[0]];
  $params['!account-modifier'] = !empty($account_modifier_full_name) ? $account_modifier_full_name : $account_modifier->name;
  $params['!site-url'] = url('', array(
    'purl' => array(
      'id' => $vsite->id,
      'provider' => $custom_domain ? 'vsite_domain' : 'spaces_og',
    ),
    'absolute' => true,
  ));
  $params['!account-modifier-email'] = $account_modifier->mail;
  $params['!user-firstname'] = $account->field_first_name[LANGUAGE_NONE][0]['value'];
  $params['!sitewide-name'] = variable_get('site_name', variable_get('sitewide_name'));
  $params['headers']['Cc'] = $account_modifier->mail;
  // Send email notification to deleted user.
  drupal_mail('cp_user', 'os_user_delete_from_group', $to, $language, $params, $from);

  drupal_goto('cp/users');
}

/**
 * Builds the "Change site owner" form.
 */
function cp_user_change_site_owner_form($form_state) {
  global $user;
  $vsite = vsite_get_vsite();
  $site_name = $vsite->group->title;

  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $vsite->group->nid);
  $form['bundle'] = array('#type' => 'value', '#value' => $vsite->group->type);
  $form['site_name'] = array('#type' => 'value', '#value' => $site_name);

  $form['title'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Choose a new site owner for the @site site', array('@site' => $site_name)) . '</h2>',
  );

  // Get all VSite members UIDs.
  $members_uids = cp_user_get_member_uids($vsite->group->nid);
  // Remove the VSite owner from the list.
  unset($members_uids[$vsite->group->uid]);

  $options = array();
  if (!empty($members_uids)) {
    $accounts = user_load_multiple($members_uids);

    foreach ($accounts as $account) {
      // If the pinserver module is enabled, don't show users who don't have an associated HUID
      if ($user->uid == 1 || !module_exists('pinserver') || (module_exists('pinserver') && variable_get('pinserver_pin_url', FALSE) && pinserver_user_has_associated_pin($account->uid))) {
        $options[$account->uid] = $account->name;
      }
    }
  }

  $form['new_site_owner'] = array(
    '#type' => 'select',
    '#title' => t('Username'),
    '#required' => TRUE,
    '#description' => '',
    '#options' => $options,
  );

  // Builds and returns a confirmation form.
  return confirm_form($form, t('Change Site Ownership'), 'cp/users', '', t('Save'), t('Cancel'));
}

/**
 * Submit callback; Changes owner of the site after confirmation.
 */
function cp_user_change_site_owner_form_submit($form, &$form_state) {
  global $user;
  // Accesses values from the form.
  $bundle = $form_state['values']['bundle'];
  $gid = $form_state['values']['gid'];

  $node = node_load($gid);
  $account = user_load($form_state['values']['new_site_owner']);

  // Make sure previous owner remains "vsite admin".
  _cp_user_set_role($node->uid, $gid, OS_USER_ROLE_ADMIN, $bundle);

  // Set site owner (VSite manager).
  $node->uid = $account->uid;
  node_save($node);

  // Give the new owner the "vsite admin" role and revoke other roles.
  _cp_user_set_role($form_state['values']['new_site_owner'], $gid, OS_USER_ROLE_ADMIN, $bundle);

  // Set the message and redirect back to the cp/users page
  $site_name = $form_state['values']['site_name'];
  drupal_set_message(t('The user %user is now the owner of the site %site.', array('%user' => $account->name, '%site' => $site_name)));

  $vsite = vsite_get_vsite();
  // Loading logged in user's user object for full name.
  $account_modifier = user_load($user->uid);
  $account_modifier_full_name = $account_modifier->field_first_name[LANGUAGE_NONE][0]['value'] . ' ' . $account_modifier->field_last_name[LANGUAGE_NONE][0]['value'];
  $purl_base_domain = variable_get('purl_base_domain');  

  // Checking if this vsite has a custom domain or not.
  $custom_domain = db_select('purl', 'p')
    ->fields('p', array('value'))
    ->condition('provider', 'vsite_domain', '=')
    ->condition('id', $vsite->group->nid, '=')
    ->execute()
    ->fetchField();
  // Setting up email parameters.
  $to = $account->mail;
  $from = variable_get('site_mail', ini_get('sendmail_from'));
  $params['!user-role'] = 'Site Owner';
  $params['!account-modifier'] = !empty(trim($account_modifier_full_name)) ? $account_modifier_full_name : $account_modifier->name;
  $params['!site-url'] = url('', array(
    'purl' => array(
      'id' => $vsite->id,
      'provider' => $custom_domain ? 'vsite_domain' : 'spaces_og',
    ),
    'absolute' => true,
  ));
  $params['!account-modifier-email'] = $account_modifier->mail;
  $params['!user-firstname'] = $account->field_first_name[LANGUAGE_NONE][0]['value'];
  $params['!sitewide-name'] = variable_get('site_name', variable_get('sitewide_name'));
  $params['headers']['Cc'] = $account_modifier->mail;
  // Sending email notification to modified user.
  drupal_mail('cp_user', 'os_user_role_change', $to, language_default(), $params, $from);

  drupal_goto('cp/users');
}

/**
 * Form builder; provides the options for the "Edit membership" form.
 */
function cp_user_edit_membership_form($form_state, $user_load) {
  $space = spaces_get_space();
  if (!isset($user_load['build_info']['args'][0]->uid)) {
    return;
  }
  $group_user = $user_load['build_info']['args'][0];

  // Prepares replacement value text for the modal form.
  $name = _cp_user_get_full_name($group_user);
  $site = $space->group->title;

  // Form sets.
  $form['gid'] = array('#type' => 'value', '#value' => $space->group->nid);
  $form['bundle'] = array('#type' => 'value', '#value' => $space->group->type);
  $form['uid'] = array('#type' => 'value', '#value' => $group_user->uid);
  $form['user_name'] = array('#type' => 'value', '#value' => $name);
  $form['site_name'] = array('#type' => 'value', '#value' => $site);
  $form['title'] = array(
    '#type' => 'markup',
    '#markup' => t('<h2>Editing member <em>@name</em></h2>', array('@name' => $name)),
    '#weight' => 0,
  );
  $form['edit_membership'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role:'),
    '#weight' => 10,
  );
  $form['edit_membership']['edit_role'] = array(
    '#type' => 'radios',
    // Each option is themed and includes a description.
    '#options' => _cp_user_list_roles('options', $space->group->type, $space->group->nid),
    '#default_value' => _cp_user_get_role($group_user->uid, $space->group->nid),
  );

  // Show the option to change site owner only for the current owner or a support team member with an associated HUID if the pinserver module is enabled
  global $user;
  if ($user->uid == 1 || !module_exists('pinserver') || (module_exists('pinserver') && variable_get('pinserver_pin_url', FALSE) && pinserver_user_has_associated_pin($group_user->uid))) {

    $form['edit_membership']['group_manager'] = array(
      '#type' => 'checkbox',
      // Theme the checkbox label to look like the radio options.
      '#title' => _cp_user_role_option(t('Set as site owner'), t('Make <em>@name</em> the site owner.', array('@name' => $name))),
      '#default_value' => $space->group->uid == $group_user->uid,
      '#access' => in_array('support team', $user->roles) || ($space->group->uid == $user->uid) || ($user->uid == 1),
    );
  }

  // Builds and returns a confirmation form.
  $question = t('Editing members for site <em>@site</em>', array('@name' => $name, '@site' => $site));
  $path = 'cp/users';
  $description = ' ';
  $yes = t('Save');
  $no = t('Cancel');
  return confirm_form($form, $question, $path, $description, $yes, $no);
}

/**
 * Submit callback; updates roles after user confirms action.
 */
function cp_user_edit_membership_form_submit($form, &$form_state) {
  global $user;
  // Accesses values from the form.
  $bundle = $form_state['values']['bundle'];
  $new_role = $form_state['values']['edit_role'];
  $group_manager = $form_state['values']['group_manager'];
  $gid = $form_state['values']['gid'];
  $uid = $form_state['values']['uid'];
  $member_rid_before_modification = array_keys(og_get_user_roles('node', $gid, $uid, FALSE));

  // Grants the new role and revokes other roles.
  _cp_user_set_role($uid, $gid, $new_role, $bundle);

  if ($group_manager) {
    $node = node_load($gid);

    // Make sure previous owner remains "vsite admin".
    _cp_user_set_role($node->uid, $gid, OS_USER_ROLE_ADMIN, $bundle);

    // Set VSite manager.
    $node->uid = $uid;
    node_save($node);

    // Give the new manager the "vsite admin" role and revoke other roles.
    _cp_user_set_role($uid, $gid, OS_USER_ROLE_ADMIN, $bundle);
  }

  // Set the message and redirect back to the cp/users page
  $user_name = $form_state['values']['user_name'];
  $site = $form_state['values']['site_name'];
  drupal_set_message(t('User <em>@user\'s</em> role on site <em>@site</em> was updated successfully.', array('@user' => $user_name, '@site' => $site)));

  $modified_member_rid = array_keys(og_get_user_roles('node', $gid, $uid, FALSE));
  // If the changed role is not same as previously saved role, or the user role is set as 'Site Owner' then only the email will be sent.
  if (!array_intersect($modified_member_rid, $member_rid_before_modification) || $group_manager) {
    $vsite = vsite_get_vsite();
    // Loading logged in user's user object for full name.
    $account_modifier = user_load($user->uid);
    $account_modifier_full_name = $account_modifier->field_first_name[LANGUAGE_NONE][0]['value'] . ' ' . $account_modifier->field_last_name[LANGUAGE_NONE][0]['value'];
    $purl_base_domain = variable_get('purl_base_domain');

    // Role will be 'Site Owner' is site owner checkbox is selected.
    if ($group_manager) {
      $new_role_name = 'Site Owner';
    } else {
      $roles_human_names = _cp_user_list_roles('rids-human', NULL, $gid);
      $new_role_name = $roles_human_names[$modified_member_rid[0]];
    }

    $account = user_load($uid);
    // Checking if this vsite has a custom domain or not.
    $custom_domain = db_select('purl', 'p')
      ->fields('p', array('value'))
      ->condition('provider', 'vsite_domain', '=')
      ->condition('id', $vsite->group->nid, '=')
      ->execute()
      ->fetchField();
    // Setting up email parameters.
    $to = $account->mail;
    $from = variable_get('site_mail', ini_get('sendmail_from'));
    $params['!user-role'] = $new_role_name;
    $params['!account-modifier'] = !empty(trim($account_modifier_full_name)) ? $account_modifier_full_name : $account_modifier->name;
    $params['!site-url'] = url('', array(
      'purl' => array(
        'id' => $vsite->id,
        'provider' => $custom_domain ? 'vsite_domain' : 'spaces_og',
      ),
      'absolute' => true,
    ));
    $params['!account-modifier-email'] = $account_modifier->mail;
    $params['!user-firstname'] = $account->field_first_name[LANGUAGE_NONE][0]['value'];
    $params['!sitewide-name'] = variable_get('site_name', variable_get('sitewide_name'));
    $params['headers']['Cc'] = $account_modifier->mail;
    // Sending email notification to modified user.
    drupal_mail('cp_user', 'os_user_role_change', $to, language_default(), $params, $from);
  }

  drupal_goto('cp/users');
}

/**
 * Returns a name like "First Last (username)", or else username if none found.
 *
 * @param object $user
 *   The fully loaded user object.
 *
 * @return string $name
 *   The user's full name, or username if none found.
 */
function _cp_user_get_full_name($user) {
  // Tries to get the user's first and last name, otherwise uses username.
  // Gets the first name field's safe value.
  $items = field_get_items('user', $user, 'field_first_name');
  if (isset($items[0]['safe_value'])) {
    $first_name = $items[0]['safe_value'];
  }
  else {
    $first_name = '';
  }
  // Gets the last name field's safe value.
  $items = field_get_items('user', $user, 'field_last_name');
  if (isset($items[0]['safe_value'])) {
    $last_name = $items[0]['safe_value'];
  }
  else {
    $last_name = '';
  }
  // Uses either the full name, or the username if first and last not available.
  if ($first_name && $last_name) {
    $name = "$first_name $last_name";
  }
  else {
    $name = $user->name;
  }

  return $name;
}

/**
 * Returns a themed label and description of a role for the Manage Members page.
 */
function _cp_user_role_option($label, $description) {
  return "<strong>$label</strong><br/>$description";
}

/**
 * Returns the user's current role for this group.
 *
 * Note: Users in OS are not supposed to be assigned more than one role in each
 * group, but this is true only in the UI. In fact, a user in OS may have two
 * roles - when a user is assigned a new role, they keep the 'member' role,
 * but it is not shown in the UI. This means that if the user has another role
 * besides the 'member' role, then this is the role we wish to display.
 *
 * @param bool $human_name
 *   If TRUE, returns the human-readable name (i.e. "Administrator")
 *   If FALSE, returns the machine name (i.e. "vsite admin")
 */
function _cp_user_get_role($uid, $gid, $human_name = FALSE) {
  // Get all OG roles assigned to the given user in the given group, without
  // including the roles 'member' and 'non-member'.
  $user_roles = og_get_user_roles('node', $gid, $uid, FALSE);

  if (empty($user_roles)) {
    // Empty array probably means that the user is just a member, which we
    // display as 'None' to show that they do not have a specific role.
    return $human_name ? 'None' : OG_AUTHENTICATED_ROLE;
  }

  // User has a role besides 'member', get the only role in the array.
  $role = reset($user_roles);

  if ($human_name) {
    // Get all human names for OG roles in this group.
    $names = _cp_user_list_roles('names', NULL, $gid);
    // Get the role's human name, if any (will default to the machine name).
    $role = $names[$role];
  }

  return $role;
}

/**
 * Grants the specified role for this user on this group, and revokes others.
 * @param int $uid
 * @param int $gid
 * @param string $role_name
 */
function _cp_user_set_role($uid, $gid, $role_name, $bundle = 'personal') {
  // Gets a map of group roles with (key) role machine name => (value) role ID.
  $roles = _cp_user_list_roles('rids', $bundle, $gid);

  // Loops over each possible OG role...
  foreach ($roles as $machine_name => $rid) {
    // If this is the new role to grant the user...
    if ($role_name == $machine_name) {
      // Checks the user's current roles in this group.
      $user_roles = og_get_user_roles('node', $gid, $uid);
      // Only grants the role if the user doesn't have the role yet.
      if (empty($user_roles[$rid])) {
        og_role_grant('node', $gid, $uid, $rid);
      }
    }
    // Otherwise, we remove the non-specified role.
    // Only one role per-user-per-group allowed at a time.
    else {
      og_role_revoke('node', $gid, $uid, $rid);
    }
  }
}
