<?php
/**
 * @file
 * os_presentations.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_presentations_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_presentations_recent';
  $box->plugin_key = 'os_sv_list_box';
  $box->title = 'Recent Presentations';
  $box->description = 'Recent Presentations';
  $box->options = array(
    'content_type' => 'presentation',
    'display' => 'title',
    'more' => 1,
    'more_text' => 'More',
    'more_link' => 'presentations',
    'pager' => 0,
    'number_of_items' => '6',
    'sort_by' => 'sort_newest',
  );
  $export['os_presentations_recent'] = $box;

  return $export;
}
