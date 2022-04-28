<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_lineutils_plugin() {
  $plugins['lineutils'] = array(
    'title' => t('lineutils'),
    'icon file' => 'mathjax.png',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/lineutils',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
