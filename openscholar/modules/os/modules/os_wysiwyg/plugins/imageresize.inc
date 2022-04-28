<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_imageresize_plugin() {
  $plugins['imageresize'] = array(
    'title' => t('Image resize'),
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/imageresize',
    'js file' => 'plugin.js',
    'settings' => array(),
    'extensions' => array(
      'imageresize' => t('Image resize'),
    ),
  );
  return $plugins;
}
