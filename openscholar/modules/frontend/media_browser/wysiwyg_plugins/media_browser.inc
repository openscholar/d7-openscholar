<?php

/**
 * Define wysiwyg plugin.
 */
function media_browser_media_browser_plugin() {

  media_browser_include_browser_js();

  $path = drupal_get_path('module', 'media_browser');
  $plugins['media_browser'] = array(
    'title' => 'Media Browser',
    'vendor url' => 'https://theopenscholar.com',
    'icon path' => $path.'/theme',
    'icon file' => 'os_link.gif',
    'icon title' => 'Media Browser',
    // @todo: move this to the plugin directory for the wysiwyg plugin.
    'js path' => $path . '/misc',
    'js file' => 'wysiwyg-media_browser.js',
    'css file' => NULL,
    'css path' => NULL,
    'settings' => array(),
  );

  return $plugins;
}

/**
 * Helper function; Include media browser library and js.
 */
function media_browser_include_browser_js() {
  static $included = false;
  if ($included) {
    return;
  }
  $included = TRUE;

  drupal_add_library('media', 'media_browser');
}
