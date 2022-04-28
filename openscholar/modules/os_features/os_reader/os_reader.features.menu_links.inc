<?php
/**
 * @file
 * os_reader.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function os_reader_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-menu:reader
  $menu_links['primary-menu:reader'] = array(
    'menu_name' => 'primary-menu',
    'link_path' => 'reader',
    'router_path' => 'reader',
    'link_title' => 'Reader',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Reader');


  return $menu_links;
}
