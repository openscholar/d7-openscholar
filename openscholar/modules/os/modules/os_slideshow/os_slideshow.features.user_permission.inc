<?php
/**
 * @file
 * os_slideshow.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_slideshow_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create slideshow_slide content'.
  $permissions['create slideshow_slide content'] = array(
    'name' => 'create slideshow_slide content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any slideshow_slide content'.
  $permissions['delete any slideshow_slide content'] = array(
    'name' => 'delete any slideshow_slide content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own slideshow_slide content'.
  $permissions['delete own slideshow_slide content'] = array(
    'name' => 'delete own slideshow_slide content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any slideshow_slide content'.
  $permissions['edit any slideshow_slide content'] = array(
    'name' => 'edit any slideshow_slide content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own slideshow_slide content'.
  $permissions['edit own slideshow_slide content'] = array(
    'name' => 'edit own slideshow_slide content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
