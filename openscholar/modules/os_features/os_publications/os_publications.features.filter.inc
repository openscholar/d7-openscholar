<?php
/**
 * @file
 * os_publications.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function os_publications_filter_default_formats() {
  $formats = array();

  // Exported format: Title Filter.
  $formats['title_filter'] = array(
    'format' => 'title_filter',
    'name' => 'Title Filter',
    'cache' => 1,
    'status' => 1,
    'weight' => -9,
    'filters' => array(
      'filter_html' => array(
        'weight' => -10,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<i> <sup> <sub>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => 10,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
