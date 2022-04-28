<?php
/**
 * @file
 * os_search_solr.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_search_solr_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'solr_search_box';
  $box->plugin_key = 'os_search_solr_search_box';
  $box->title = '';
  $box->description = 'Search box';
  $box->options = array();
  $export['solr_search_box'] = $box;

  return $export;
}
