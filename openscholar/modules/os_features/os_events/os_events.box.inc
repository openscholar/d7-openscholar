<?php
/**
 * @file
 * os_events.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_events_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_events_upcoming';
  $box->plugin_key = 'os_sv_list_box';
  $box->title = 'Upcoming Events';
  $box->description = 'Upcoming Events';
  $box->options = array(
    'content_type' => 'event',
    'display' => 'sidebar_teaser',
    'more' => 1,
    'more_text' => 'More',
    'more_link' => 'calendar',
    'pager' => 0,
    'number_of_items' => '6',
    'sort_by' => 'sort_event_asc',
  );
  $export['os_events_upcoming'] = $box;

  return $export;
}
