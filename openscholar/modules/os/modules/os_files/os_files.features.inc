<?php
/**
 * @file
 * os_files.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_files_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
