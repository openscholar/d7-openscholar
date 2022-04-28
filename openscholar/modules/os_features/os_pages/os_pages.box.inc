<?php
/**
 * @file
 * os_pages.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_pages_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_pages_section_nav';
  $box->plugin_key = 'os_boxes_sectionnav';
  $box->title = '';
  $box->description = 'Section Navigation';
  $box->options = array();
  $export['os_pages_section_nav'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'vocabulary_filter_pages';
  $box->plugin_key = 'os_boxes_facetapi_vocabulary';
  $box->title = 'Filter by taxonomy for pages';
  $box->description = 'Filter by taxonomy for pages';
  $box->options = array(
    'bundle' => 'page',
    'vocabularies' => array(),
  );
  $export['vocabulary_filter_pages'] = $box;

  return $export;
}
