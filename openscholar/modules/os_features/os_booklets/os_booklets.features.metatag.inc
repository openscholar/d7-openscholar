<?php
/**
 * @file
 * os_booklets.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_booklets_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:book.
  $config['node:book'] = array(
    'instance' => 'node:book',
    'config' => array(
      'og:type' => array(
        'value' => 'book',
      ),
    ),
  );

  return $config;
}
