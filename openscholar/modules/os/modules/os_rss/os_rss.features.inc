<?php
/**
 * @file
 * os_rss.features.inc
 */

/**
 * Implements hook_views_api().
 */
function os_rss_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_ctools_plugin_api().
 */
function os_rss_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
