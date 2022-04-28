<?php
/**
 * @file
 * vsite.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vsite_user_default_roles() {
  $roles = array();

  // Exported role: support team.
  $roles['support team'] = array(
    'name' => 'support team',
    'weight' => 8,
  );

  return $roles;
}
