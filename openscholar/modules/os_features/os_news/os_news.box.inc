<?php
/**
 * @file
 * os_news.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_news_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_news_latest';
  $box->plugin_key = 'os_sv_list_box';
  $box->title = 'Latest News';
  $box->description = 'Latest News';
  $box->options = array(
    'content_type' => 'news',
    'display' => 'title',
    'more' => 1,
    'more_text' => 'More',
    'more_link' => 'news',
    'pager' => 0,
    'number_of_items' => '6',
    'sort_by' => 'sort_newest',
  );
  $export['os_news_latest'] = $box;

  return $export;
}
