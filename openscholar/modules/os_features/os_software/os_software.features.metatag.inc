<?php
/**
 * @file
 * os_software.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_software_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:software_project.
  $config['node:software_project'] = array(
    'instance' => 'node:software_project',
    'config' => array(
      'og:type' => array(
        'value' => 'product',
      ),
      'twitter:card' => array(
        'value' => 'product',
      ),
    ),
  );

  // Exported Metatag config instance: node:software_release.
  $config['node:software_release'] = array(
    'instance' => 'node:software_release',
    'config' => array(
      'og:type' => array(
        'value' => 'product',
      ),
      'twitter:card' => array(
        'value' => 'product',
      ),
    ),
  );

  return $config;
}
