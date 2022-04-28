<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_mathjax_plugin() {
  $plugins['mathjax'] = array(
    'title' => t('Math JAX'),
    'icon path' => drupal_get_path('module', 'os_wysiwyg') . '/plugins/mathjax/icons/hidpi/mathjax.png',
    'icon file' => 'loader.gif',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/mathjax',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
