<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_widget_plugin() {
  $plugins['widget'] = array(
    'title' => t('widget'),
    'icon file' => 'mathjax.png',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/widget',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
