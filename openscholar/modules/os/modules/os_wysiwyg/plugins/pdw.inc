<?php

/**
 * Implementation of hook_wysiwyg_plugin().
 */
function os_wysiwyg_pdw_plugin() {
  $plugins['pdw'] = array(
    'title' => t('Toggle Toolbars'),
    'vendor url' => 'http://www.neele.name/pdw_toggle_toolbars',
    'icon file' => 'toolbars.gif',
    'icon title' => t('Toggle additional toolbars'),
    'js file' => 'editor_plugin_src.js',
    'settings' => array(
      'pdw_toggle_on' => 1,
      'pdw_toggle_toolbars' => '2,3,4',
    ),
  );
  return $plugins;
}
