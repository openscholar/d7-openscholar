<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_panelbutton_plugin() {
  $plugins['panelbutton'] = array(
    'title' => t('Panel button'),
    'vendor url' => 'http://www.neele.name/pdw_toggle_toolbars',
    'icon file' => 'toolbars.gif',
    'icon title' => t('Toggle additional toolbars'),
    'js path' => libraries_get_path('ckeditor') . '/plugins/panelbutton',
    'js file' => 'plugin.js',
    'settings' => array(),
  );
  return $plugins;
}
