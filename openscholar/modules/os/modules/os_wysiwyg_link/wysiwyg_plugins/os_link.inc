<?php

/**
 * Define wysiwyg plugin.
 */
function os_wysiwyg_link_os_link_plugin() {
  media_include_browser_js();

  os_wysiwyg_link_include_browser_js();

  $path = drupal_get_path('module', 'os_wysiwyg_link');
  $plugins['os_link'] = array(
    'title' => t('Link'),
    'vendor url' => 'http://openscholar.harvard.edu',
    'icon path' => $path.'/theme',
    'icon file' => 'os_link.gif',
    'icon title' => 'Link',
    // @todo: move this to the plugin directory for the wysiwyg plugin.
    'js path' => $path . '/js',
    'js file' => 'wysiwyg-os_link.js',
    'css file' => NULL,
    'css path' => NULL,
    'settings' => array(
      'global' => array(
        'types' => os_wysiwyg_link_get_media_browser_types(),
        'id' => 'os_link_wysiwyg',
      ),
    ),
  );

  return $plugins;
}

/**
 * Helper function; Include media browser library and js.
 */
function os_wysiwyg_link_include_browser_js() {
  static $included = false;
  if ($included) {
    return;
  }
  $included = TRUE;

  $settings = array(
    'browserUrl' => url('os/wysiwyg/link/insert', array(
        'query' => array('render' => 'media-popup'))),
  );

  $js = array('osWysiwygLink' => $settings);
  drupal_add_library('media', 'media_browser');
  drupal_add_js($js, 'setting');
}
