<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_font_plugin() {
  $plugins['font'] = array(
    'title' => t('Font'),
    'icon file' => 'toolbars.gif',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/font',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
