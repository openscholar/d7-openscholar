<?php
/**
 * @file
 * os_software.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_software_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create software_project content'.
  $permissions['create software_project content'] = array(
    'name' => 'create software_project content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create software_release content'.
  $permissions['create software_release content'] = array(
    'name' => 'create software_release content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any software_project content'.
  $permissions['delete any software_project content'] = array(
    'name' => 'delete any software_project content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any software_release content'.
  $permissions['delete any software_release content'] = array(
    'name' => 'delete any software_release content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own software_project content'.
  $permissions['delete own software_project content'] = array(
    'name' => 'delete own software_project content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own software_release content'.
  $permissions['delete own software_release content'] = array(
    'name' => 'delete own software_release content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any software_project content'.
  $permissions['edit any software_project content'] = array(
    'name' => 'edit any software_project content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any software_release content'.
  $permissions['edit any software_release content'] = array(
    'name' => 'edit any software_release content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own software_project content'.
  $permissions['edit own software_project content'] = array(
    'name' => 'edit own software_project content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own software_release content'.
  $permissions['edit own software_release content'] = array(
    'name' => 'edit own software_release content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
