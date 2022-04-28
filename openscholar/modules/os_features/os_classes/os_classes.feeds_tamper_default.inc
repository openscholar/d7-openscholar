<?php
/**
 * @file
 * os_classes.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function os_classes_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_classes-body-utf8_convert';
  $feeds_tamper->importer = 'os_classes';
  $feeds_tamper->source = 'Body';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_classes-body-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_classes-created_date-adjust_date_to_timezone';
  $feeds_tamper->importer = 'os_classes';
  $feeds_tamper->source = 'Created date';
  $feeds_tamper->plugin_id = 'adjust_date_to_timezone';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Adjust date to timezone';
  $export['os_classes-created_date-adjust_date_to_timezone'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_classes-link_title-utf8_convert';
  $feeds_tamper->importer = 'os_classes';
  $feeds_tamper->source = 'Link title';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_classes-link_title-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_classes-semester-convert_case';
  $feeds_tamper->importer = 'os_classes';
  $feeds_tamper->source = 'Semester';
  $feeds_tamper->plugin_id = 'convert_case';
  $feeds_tamper->settings = array(
    'mode' => '2',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert case';
  $export['os_classes-semester-convert_case'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_classes-semester-find_replace';
  $feeds_tamper->importer = 'os_classes';
  $feeds_tamper->source = 'Semester';
  $feeds_tamper->plugin_id = 'find_replace';
  $feeds_tamper->settings = array(
    'find' => 'n/a',
    'replace' => 'N/A',
    'case_sensitive' => 0,
    'word_boundaries' => 0,
    'whole' => 1,
    'regex' => TRUE,
    'regex_find' => '/^n\\/a$/ui',
  );
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Find replace';
  $export['os_classes-semester-find_replace'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_classes-title-utf8_convert';
  $feeds_tamper->importer = 'os_classes';
  $feeds_tamper->source = 'Title';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_classes-title-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_classes-year-year_to_full_date';
  $feeds_tamper->importer = 'os_classes';
  $feeds_tamper->source = 'Year';
  $feeds_tamper->plugin_id = 'year_to_full_date';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert year to full date';
  $export['os_classes-year-year_to_full_date'] = $feeds_tamper;

  return $export;
}
