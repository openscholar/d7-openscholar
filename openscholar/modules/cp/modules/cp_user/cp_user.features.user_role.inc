<?php
/**
 * @file
 * cp_user.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function cp_user_user_default_roles() {
  $roles = array();

  $roles['viewer'] = array(
    'name' => 'viewer',
    'weight' => 10,
  );

  return $roles;
}
