<?php
/**
 * @file
 * vsite_preset.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vsite_preset_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "spaces" && $api == "spaces") {
    return array("version" => "3");
  }
  if ($module == "boxes" && $api == "box") {
    return array("version" => "1");
  }
}
