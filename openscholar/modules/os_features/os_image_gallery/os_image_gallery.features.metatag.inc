<?php
/**
 * @file
 * os_image_gallery.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_image_gallery_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:media_gallery.
  $config['node:media_gallery'] = array(
    'instance' => 'node:media_gallery',
    'config' => array(
      'og:type' => array(
        'value' => 'album',
      ),
    ),
  );

  return $config;
}
