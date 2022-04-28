<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_justify_plugin() {
  $plugins['justify'] = array(
    'title' => t('justify'),
    'icon file' => 'justifyblock.png',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/justify',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
