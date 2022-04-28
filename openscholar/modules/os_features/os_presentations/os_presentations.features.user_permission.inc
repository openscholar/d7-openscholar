<?php
/**
 * @file
 * os_presentations.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_presentations_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create presentation content'.
  $permissions['create presentation content'] = array(
    'name' => 'create presentation content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any presentation content'.
  $permissions['delete any presentation content'] = array(
    'name' => 'delete any presentation content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own presentation content'.
  $permissions['delete own presentation content'] = array(
    'name' => 'delete own presentation content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any presentation content'.
  $permissions['edit any presentation content'] = array(
    'name' => 'edit any presentation content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own presentation content'.
  $permissions['edit own presentation content'] = array(
    'name' => 'edit own presentation content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
