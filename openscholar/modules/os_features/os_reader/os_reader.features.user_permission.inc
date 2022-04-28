<?php
/**
 * @file
 * os_reader.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_reader_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create feed content'.
  $permissions['create feed content'] = array(
    'name' => 'create feed content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any feed content'.
  $permissions['delete any feed content'] = array(
    'name' => 'delete any feed content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own feed content'.
  $permissions['delete own feed content'] = array(
    'name' => 'delete own feed content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any feed content'.
  $permissions['edit any feed content'] = array(
    'name' => 'edit any feed content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own feed content'.
  $permissions['edit own feed content'] = array(
    'name' => 'edit own feed content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
