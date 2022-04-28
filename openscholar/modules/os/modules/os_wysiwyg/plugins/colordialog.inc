<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_colordialog_plugin() {
  $plugins['colordialog'] = array(
    'title' => t('Color dialog'),
    'icon file' => 'toolbars.gif',
    'icon title' => t('Color dialog'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/colordialog',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
