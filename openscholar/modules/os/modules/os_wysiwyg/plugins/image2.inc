<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_image2_plugin() {
  $plugins['image2'] = array(
    'title' => t('Image2'),
    'icon title' => t('Image2'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/image2',
    'js file' => 'plugin.js',
    'settings' => array(),
    'extensions' => array(
      'imageresize' => t('Image resize'),
    ),
  );
  return $plugins;
}
