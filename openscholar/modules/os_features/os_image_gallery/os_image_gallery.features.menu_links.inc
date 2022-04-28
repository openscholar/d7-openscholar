<?php
/**
 * @file
 * os_image_gallery.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function os_image_gallery_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: primary-menu:galleries
  $menu_links['primary-menu:galleries'] = array(
    'menu_name' => 'primary-menu',
    'link_path' => 'galleries',
    'router_path' => 'galleries',
    'link_title' => 'Galleries',
    'options' => array(
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
  t('Galleries');


  return $menu_links;
}
