<?php
/**
 * @file
 * os_news.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_news_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:news.
  $config['node:news'] = array(
    'instance' => 'node:news',
    'config' => array(
      'og:type' => array(
        'value' => 'activity',
      ),
      'og:image' => array(
        'value' => '[node:field_photo]',
      ),
    ),
  );

  return $config;
}
