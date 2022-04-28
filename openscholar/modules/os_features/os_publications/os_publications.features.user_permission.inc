<?php
/**
 * @file
 * os_publications.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function os_publications_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access biblio content'.
  $permissions['access biblio content'] = array(
    'name' => 'access biblio content',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'administer biblio'.
  $permissions['administer biblio'] = array(
    'name' => 'administer biblio',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'create biblio content'.
  $permissions['create biblio content'] = array(
    'name' => 'create biblio content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any biblio content'.
  $permissions['delete any biblio content'] = array(
    'name' => 'delete any biblio content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own biblio content'.
  $permissions['delete own biblio content'] = array(
    'name' => 'delete own biblio content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any biblio content'.
  $permissions['edit any biblio content'] = array(
    'name' => 'edit any biblio content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit biblio authors'.
  $permissions['edit biblio authors'] = array(
    'name' => 'edit biblio authors',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'edit own biblio content'.
  $permissions['edit own biblio content'] = array(
    'name' => 'edit own biblio content',
    'roles' => array(
      'administrator' => 'administrator',
      'manager' => 'manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'import from file'.
  $permissions['import from file'] = array(
    'name' => 'import from file',
    'roles' => array(
      'administrator' => 'administrator',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'show download links'.
  $permissions['show download links'] = array(
    'name' => 'show download links',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'show export links'.
  $permissions['show export links'] = array(
    'name' => 'show export links',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'show filter tab'.
  $permissions['show filter tab'] = array(
    'name' => 'show filter tab',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'show sort links'.
  $permissions['show sort links'] = array(
    'name' => 'show sort links',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'biblio',
  );

  // Exported permission: 'use text format title_filter'.
  $permissions['use text format title_filter'] = array(
    'name' => 'use text format title_filter',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'view full text'.
  $permissions['view full text'] = array(
    'name' => 'view full text',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'content editor' => 'content editor',
      'manager' => 'manager',
      'vsite admin' => 'vsite admin',
      'vsite user' => 'vsite user',
    ),
    'module' => 'biblio',
  );

  return $permissions;
}
