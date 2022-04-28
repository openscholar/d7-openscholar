<?php
/**
 * @file
 * cp_user.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function cp_user_user_default_permissions() {
  $permissions = array();
  // Exported permission: 'access content overview'.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'viewer' => 'viewer',
      'administrator' => 'administrator',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin'
    ),
    'module' => 'node',
  );

  return $permissions;
}
