<?php
/**
 * @file
 * os_events.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_events_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer event_signup_simple registration'.
  $permissions['administer event_signup_simple registration'] = array(
    'name' => 'administer event_signup_simple registration',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'administer own event_signup_simple registration'.
  $permissions['administer own event_signup_simple registration'] = array(
    'name' => 'administer own event_signup_simple registration',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'administer registration'.
  $permissions['administer registration'] = array(
    'name' => 'administer registration',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'administer registration states'.
  $permissions['administer registration states'] = array(
    'name' => 'administer registration states',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'administer registration types'.
  $permissions['administer registration types'] = array(
    'name' => 'administer registration types',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'create event content'.
  $permissions['create event content'] = array(
    'name' => 'create event content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create event_signup_simple registration'.
  $permissions['create event_signup_simple registration'] = array(
    'name' => 'create event_signup_simple registration',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'create event_signup_simple registration other anonymous'.
  $permissions['create event_signup_simple registration other anonymous'] = array(
    'name' => 'create event_signup_simple registration other anonymous',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'create event_signup_simple registration other users'.
  $permissions['create event_signup_simple registration other users'] = array(
    'name' => 'create event_signup_simple registration other users',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'delete any event content'.
  $permissions['delete any event content'] = array(
    'name' => 'delete any event content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any event_signup_simple registration'.
  $permissions['delete any event_signup_simple registration'] = array(
    'name' => 'delete any event_signup_simple registration',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'delete own event content'.
  $permissions['delete own event content'] = array(
    'name' => 'delete own event content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own event_signup_simple registration'.
  $permissions['delete own event_signup_simple registration'] = array(
    'name' => 'delete own event_signup_simple registration',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'edit any event content'.
  $permissions['edit any event content'] = array(
    'name' => 'edit any event content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit event_signup_simple registration state'.
  $permissions['edit event_signup_simple registration state'] = array(
    'name' => 'edit event_signup_simple registration state',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'edit own event content'.
  $permissions['edit own event content'] = array(
    'name' => 'edit own event content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'update any event_signup_simple registration'.
  $permissions['update any event_signup_simple registration'] = array(
    'name' => 'update any event_signup_simple registration',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'update own event_signup_simple registration'.
  $permissions['update own event_signup_simple registration'] = array(
    'name' => 'update own event_signup_simple registration',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'view event_signup_simple registration'.
  $permissions['view event_signup_simple registration'] = array(
    'name' => 'view event_signup_simple registration',
    'roles' => array(
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'view own event_signup_simple registration'.
  $permissions['view own event_signup_simple registration'] = array(
    'name' => 'view own event_signup_simple registration',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'registration',
  );

  return $permissions;
}
