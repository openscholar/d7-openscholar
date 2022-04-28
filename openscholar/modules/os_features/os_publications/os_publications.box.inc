<?php
/**
 * @file
 * os_publications.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_publications_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_publications_recent';
  $box->plugin_key = 'os_sv_list_box';
  $box->title = ' Recent Publications';
  $box->description = ' Recent Publications';
  $box->options = array(
    'content_type' => 'biblio',
    'display' => 'title',
    'more' => 1,
    'more_text' => 'More',
    'more_link' => 'publications',
    'pager' => 0,
    'number_of_items' => '6',
    'sort_by' => 'sort_biblio',
  );
  $export['os_publications_recent'] = $box;

  return $export;
}
