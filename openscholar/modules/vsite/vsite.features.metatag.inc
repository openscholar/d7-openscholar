<?php
/**
 * @file
 * vsite.features.metatag.inc
 */

/**
 * Implements hook_metatag_export_default().
 */
function vsite_metatag_export_default() {
  $config = array();

  // Exported Metatag config instance: node:department.
  $config['node:department'] = array(
    'instance' => 'node:department',
    'config' => array(
      'description' => array(
        'value' => '[node:field_meta_description]',
      ),
      'og:type' => array(
        'value' => 'university',
      ),
      'og:image' => array(
        'value' => '[node:field_site_logo]',
      ),
      'twitter:image' => array(
        'value' => '[node:field_site_logo]',
      ),
      'twitter:description' => array(
        'value' => '[node:field_site_description]',
      ),
    ),
  );

  // Exported Metatag config instance: node:personal.
  $config['node:personal'] = array(
    'instance' => 'node:personal',
    'config' => array(
      'description' => array(
        'value' => '[node:field_meta_description]',
      ),
      'og:type' => array(
        'value' => 'profile',
      ),
      'og:image' => array(
        'value' => '[node:field_site_logo]',
      ),
      'twitter:image' => array(
        'value' => '[node:field_site_logo]',
      ),
      'twitter:description' => array(
        'value' => '[node:field_site_description]',
      ),
    ),
  );

  // Exported Metatag config instance: node:project.
  $config['node:project'] = array(
    'instance' => 'node:project',
    'config' => array(
      'description' => array(
        'value' => '[node:field_meta_description]',
      ),
      'og:type' => array(
        'value' => 'company',
      ),
      'og:image' => array(
        'value' => '[node:field_site_logo]',
      ),
      'twitter:image' => array(
        'value' => '[node:field_site_logo]',
      ),
      'twitter:description' => array(
        'value' => '[node:field_site_description]',
      ),
    ),
  );

  return $config;
}
