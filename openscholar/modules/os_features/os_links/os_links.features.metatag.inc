<?php
/**
 * @file
 * os_links.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_links_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:link.
  $config['node:link'] = array(
    'instance' => 'node:link',
    'config' => array(
      'og:type' => array(
        'value' => 'website',
      ),
    ),
  );

  return $config;
}
