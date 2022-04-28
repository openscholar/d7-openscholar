<?php
/**
 * @file
 * os.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function os_user_default_roles() {
  $roles = array();

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 2,
  );

  // Exported role: content editor.
  $roles['content editor'] = array(
    'name' => 'content editor',
    'weight' => 6,
  );

  // Exported role: manager.
  $roles['manager'] = array(
    'name' => 'manager',
    'weight' => 3,
  );

  // Exported role: vsite admin.
  $roles['vsite admin'] = array(
    'name' => 'vsite admin',
    'weight' => 4,
  );

  // Exported role: vsite user.
  $roles['vsite user'] = array(
    'name' => 'vsite user',
    'weight' => 5,
  );

  return $roles;
}
