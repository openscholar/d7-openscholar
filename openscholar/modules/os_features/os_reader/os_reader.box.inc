<?php
/**
 * @file
 * os_reader.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_reader_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'boxes-os_boxes_feedreader';
  $box->plugin_key = 'os_boxes_feedreader';
  $box->title = 'Latest Feed Items';
  $box->description = 'Latest Feed Items';
  $box->options = array(
    'feed' => FALSE,
    'number_of_items' => 10,
    'feed_display' => 'title',
  );
  $export['boxes-os_boxes_feedreader'] = $box;

  return $export;
}
