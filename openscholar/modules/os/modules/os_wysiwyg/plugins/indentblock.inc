<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_indentblock_plugin() {
  $plugins['indentblock'] = array(
    'title' => t('Indent block'),
    'icon file' => 'justifyblock.png',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/indentblock',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
