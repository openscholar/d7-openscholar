<?php
/**
 * @file
 * os_events.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_events_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:event.
  $config['node:event'] = array(
    'instance' => 'node:event',
    'config' => array(
      'og:type' => array(
        'value' => 'activity',
      ),
    ),
  );

  return $config;
}
