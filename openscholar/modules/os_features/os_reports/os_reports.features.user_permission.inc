<?php
/**
 * @file
 * os_reports.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_reports_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access reports'.
  $permissions['access os reports'] = array(
    'name' => 'access os reports',
    'roles' => array(
      'os report admin' => 'os report admin',
    ),
    'module' => 'os_reports',
  );

  return $permissions;
}
