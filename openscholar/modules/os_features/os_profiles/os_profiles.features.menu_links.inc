<?php
/**
 * @file
 * os_profiles.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function os_profiles_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-menu:people
  $menu_links['primary-menu:people'] = array(
    'menu_name' => 'primary-menu',
    'link_path' => 'people',
    'router_path' => 'people',
    'link_title' => 'People',
    'options' => array(
      'attributes' => array(
        'title' => 'List of people',
      ),
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('People');


  return $menu_links;
}
