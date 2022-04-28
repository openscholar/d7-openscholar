<?php
/**
 * @file
 * os_blog.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function os_blog_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:blog.
  $config['node:blog'] = array(
    'instance' => 'node:blog',
    'config' => array(
      'og:type' => array(
        'value' => 'blog',
      ),
    ),
  );

  return $config;
}
