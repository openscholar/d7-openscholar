<?php
// $Id$


/**
 * @file
 * os_blog.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_blog_default_box() {
  $export = array();

  $box = new stdClass();
  /* Edit this to true to make a default box disabled initially */
  $box->disabled    = FALSE;
  $box->api_version = 1;
  $box->delta       = 'blog_rss_feed';
  $box->plugin_key  = 'os_boxes_rss';
  $box->title       = '';
  $box->description = 'Blog RSS Feed';
  $box->options     = array(
    'content' => array('blog'),
    'show_all' => TRUE,
  );
  $export['blog_rss_feed'] = $box;

  return $export;
}

