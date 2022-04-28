<?php
/**
 * @file
 * iqss_projects_front.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function iqss_projects_front_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "boxes" && $api == "box") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
