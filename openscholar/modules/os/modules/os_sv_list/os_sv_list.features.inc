<?php
/**
 * @file
 * os_sv_list.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_sv_list_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "boxes" && $api == "box") {
    return array("version" => "1");
  }
}
