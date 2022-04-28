<?php
/**
 * @file
 * grouper.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function grouper_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'bypass grouper restriction'.
  $permissions['bypass grouper restriction'] = array(
    'name' => 'bypass grouper restriction',
    'roles' => array(
      'support team' => 'support team',
      'vsite user' => 'vsite user',
    ),
    'module' => 'grouper',
  );

  return $permissions;
}
