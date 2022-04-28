<?php
/**
 * @file
 * os_booklets.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_booklets_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_booklets_recent_docs';
  $box->plugin_key = 'os_sv_list_box';
  $box->title = 'Recent Documents';
  $box->description = 'Recent Documents';
  $box->options = array(
    'content_type' => 'book',
    'display' => 'title',
    'more' => 1,
    'more_text' => 'More',
    'more_link' => 'documents',
    'pager' => 0,
    'number_of_items' => '6',
    'sort_by' => 'sort_newest',
  );
  $export['os_booklets_recent_docs'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_booktoc';
  $box->plugin_key = 'os_boxes_booktoc';
  $box->title = 'Table of Contents';
  $box->description = 'Active Book\'s TOC';
  $box->options = array(
    'visibility' => 'feature',
    'book_nid' => '-1',
    'toc_type' => 'full',
    'as_nav' => FALSE,
  );
  $export['os_booktoc'] = $box;

  return $export;
}
