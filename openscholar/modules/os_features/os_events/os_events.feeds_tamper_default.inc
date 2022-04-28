<?php
/**
 * @file
 * os_events.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function os_events_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_events_csv-body-utf8_convert';
  $feeds_tamper->importer = 'os_events_csv';
  $feeds_tamper->source = 'Body';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_events_csv-body-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_events_csv-created_date-adjust_date_to_timezone';
  $feeds_tamper->importer = 'os_events_csv';
  $feeds_tamper->source = 'Created date';
  $feeds_tamper->plugin_id = 'adjust_date_to_timezone';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Adjust date to timezone';
  $export['os_events_csv-created_date-adjust_date_to_timezone'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_events_csv-end_date-strtotime';
  $feeds_tamper->importer = 'os_events_csv';
  $feeds_tamper->source = 'End date';
  $feeds_tamper->plugin_id = 'strtotime';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'String to Unix timestamp';
  $export['os_events_csv-end_date-strtotime'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_events_csv-location-utf8_convert';
  $feeds_tamper->importer = 'os_events_csv';
  $feeds_tamper->source = 'Location';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_events_csv-location-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_events_csv-registration-utf8_convert';
  $feeds_tamper->importer = 'os_events_csv';
  $feeds_tamper->source = 'Registration';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_events_csv-registration-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_events_csv-start_date-strtotime';
  $feeds_tamper->importer = 'os_events_csv';
  $feeds_tamper->source = 'Start date';
  $feeds_tamper->plugin_id = 'strtotime';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'String to Unix timestamp';
  $export['os_events_csv-start_date-strtotime'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_events_csv-title-utf8_convert';
  $feeds_tamper->importer = 'os_events_csv';
  $feeds_tamper->source = 'Title';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_events_csv-title-utf8_convert'] = $feeds_tamper;

  return $export;
}
