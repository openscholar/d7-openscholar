<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_colorbutton_plugin() {
  $plugins['colorbutton'] = array(
    'title' => t('Color button'),
    'icon file' => 'textcolor.png',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/colorbutton',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
