<?php
/**
 * @file
 * os_search_solr.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_search_solr_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "apachesolr_search" && $api == "apachesolr_search_defaults") {
    return array("version" => "3");
  }
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "facetapi" && $api == "facetapi_defaults") {
    return array("version" => "1");
  }
  if ($module == 'boxes' && $api == 'box') {
    return array('version' => 1);
  }
}
