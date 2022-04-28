<?php
/**
 * @file
 * os_reports.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function os_reports_user_default_roles() {
  $roles = array();

  // Exported role: os report admin.
  $roles['os report admin'] = array(
    'name' => 'os report admin',
    'weight' => 9,
  );

  return $roles;
}
