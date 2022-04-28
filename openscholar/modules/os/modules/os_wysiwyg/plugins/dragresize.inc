<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_dragresize_plugin() {
  $plugins['dragresize'] = array(
    'title' => t('Drag and resize'),
    'icon file' => 'toolbars.gif',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/dragresize',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
