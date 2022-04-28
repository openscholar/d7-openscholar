<?php
/**
 * @file
 * os_search_db.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_search_db_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
