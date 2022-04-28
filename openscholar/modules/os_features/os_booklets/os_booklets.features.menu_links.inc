<?php
// $Id$


/**
 * @file
 * os_booklets.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function os_booklets_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-menu:documents
  $menu_links['primary-menu:documents'] = array(
    'menu_name' => 'primary-menu',
    'link_path' => 'documents',
    'router_path' => 'documents',
    'link_title' => 'Documents',
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
  t('Documents');


  return $menu_links;
}

