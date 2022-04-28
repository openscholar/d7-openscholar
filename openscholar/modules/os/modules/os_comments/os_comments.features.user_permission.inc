<?php
/**
 * @file
 * os_comments.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_comments_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access comments'.
  $permissions['access comments'] = array(
    'name' => 'access comments',
    'roles' => array(
      'content editor' => 'content editor',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'administer comments'.
  $permissions['administer comments'] = array(
    'name' => 'administer comments',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'edit own comments'.
  $permissions['edit own comments'] = array(
    'name' => 'edit own comments',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'post comments'.
  $permissions['post comments'] = array(
    'name' => 'post comments',
    'roles' => array(
      'content editor' => 'content editor',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'skip comment approval'.
  $permissions['skip comment approval'] = array(
    'name' => 'skip comment approval',
    'roles' => array(
      'administrator' => 'administrator',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'toggle disqus comments'.
  $permissions['toggle disqus comments'] = array(
    'name' => 'toggle disqus comments',
    'roles' => array(
      'administrator' => 'administrator',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'disqus',
  );

  // Exported permission: 'view disqus comments'.
  $permissions['view disqus comments'] = array(
    'name' => 'view disqus comments',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'disqus',
  );

  return $permissions;
}
