<?php
/**
 * @file
 * os_publications.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_publications_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:biblio.
  $config['node:biblio'] = array(
    'instance' => 'node:biblio',
    'config' => array(
      'description' => array(
        'value' => '[node:citation]',
      ),
      'og:description' => array(
        'value' => '[node:citation]',
      ),
      'og:image' => array(
        'value' => '[node:field_biblio_image]',
      ),
    ),
  );

  return $config;
}