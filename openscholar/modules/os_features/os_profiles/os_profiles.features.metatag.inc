<?php
/**
 * @file
 * os_profiles.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_profiles_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:person.
  $config['node:person'] = array(
    'instance' => 'node:person',
    'config' => array(
      'og:type' => array(
        'value' => 'profile',
      ),
      'og:image' => array(
        'value' => '[node:field_person_photo]',
      ),
    ),
  );

  return $config;
}
