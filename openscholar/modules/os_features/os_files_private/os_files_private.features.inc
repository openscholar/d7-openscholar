<?php
/**
 * @file
 * os_files_private.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_files_private_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function os_files_private_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}
