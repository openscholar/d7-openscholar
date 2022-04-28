<?php
/**
 * @file
 * os.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function os_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: primary-menu.
  $menus['primary-menu'] = array(
    'menu_name' => 'primary-menu',
    'title' => 'Primary Menu',
    'description' => 'A menu used on OpenScholar sites to show primary content.',
  );
  // Exported menu: secondary-menu.
  $menus['secondary-menu'] = array(
    'menu_name' => 'secondary-menu',
    'title' => 'Secondary Menu',
    'description' => 'A menu used on OpenScholar sites to show secondary content.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('A menu used on OpenScholar sites to show primary content.');
  t('A menu used on OpenScholar sites to show secondary content.');
  t('Primary Menu');
  t('Secondary Menu');


  return $menus;
}
