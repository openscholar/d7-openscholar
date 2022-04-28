<?php
/**
 * @file
 * os_presentations.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_presentations_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:presentation.
  $config['node:presentation'] = array(
    'instance' => 'node:presentation',
    'config' => array(
      'og:type' => array(
        'value' => 'activity',
      ),
    ),
  );

  return $config;
}
