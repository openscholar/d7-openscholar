<?php
/**
 * @file
 * os_faq.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_faq_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_faq_sv_list';
  $box->plugin_key = 'os_sv_list_box';
  $box->title = 'Recent FAQs';
  $box->description = 'Recent FAQs';
  $box->options = array(
    'content_type' => 'faq',
    'display' => 'title',
    'more' => 1,
    'more_text' => 'More',
    'more_link' => 'faq',
    'pager' => 0,
    'number_of_items' => '6',
    'sort_by' => 'sort_newest',
    'biblio_show_all' => 1,
    'biblio_whitelist' => array(
      'all' => 'all',
      100 => 0,
      101 => 0,
      102 => 0,
      103 => 0,
      104 => 0,
      105 => 0,
      106 => 0,
      107 => 0,
      108 => 0,
      109 => 0,
      110 => 0,
      111 => 0,
      112 => 0,
      113 => 0,
      114 => 0,
      115 => 0,
      116 => 0,
      117 => 0,
      118 => 0,
      119 => 0,
      120 => 0,
      121 => 0,
      122 => 0,
      123 => 0,
      124 => 0,
      125 => 0,
      126 => 0,
      127 => 0,
      128 => 0,
      129 => 0,
      130 => 0,
      131 => 0,
      132 => 0,
      133 => 0,
      134 => 0,
      135 => 0,
    ),
  );
  $export['os_faq_sv_list'] = $box;

  return $export;
}
