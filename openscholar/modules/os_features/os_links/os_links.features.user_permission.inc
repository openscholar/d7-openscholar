<?php
/**
 * @file
 * os_links.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_links_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create link content'.
  $permissions['create link content'] = array(
    'name' => 'create link content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any link content'.
  $permissions['delete any link content'] = array(
    'name' => 'delete any link content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own link content'.
  $permissions['delete own link content'] = array(
    'name' => 'delete own link content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any link content'.
  $permissions['edit any link content'] = array(
    'name' => 'edit any link content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own link content'.
  $permissions['edit own link content'] = array(
    'name' => 'edit own link content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
