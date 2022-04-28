<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_bidi_plugin() {
  $plugins['bidi'] = array(
    'title' => t('Bidi'),
    'icon file' => 'mathjax.png',
    'icon title' => t('Text directions'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/bidi',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
