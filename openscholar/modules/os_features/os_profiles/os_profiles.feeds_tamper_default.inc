<?php
/**
 * @file
 * os_profiles.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function os_profiles_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-address-utf8_convert';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Address';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_people-address-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-prefix-utf8_convert';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Prefix';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_people-prefix-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-first_name-utf8_convert';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'First name';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_people-first_name-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-last_name-utf8_convert';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Last name';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_people-last_name-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-middle_name-utf8_convert';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Middle name';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_people-middle_name-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-short_bio-utf8_convert';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Short bio';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_people-short_bio-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-titles-explode';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Titles';
  $feeds_tamper->plugin_id = 'explode';
  $feeds_tamper->settings = array(
    'separator' => '|',
    'limit' => '',
    'real_separator' => '|',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Explode';
  $export['os_people-titles-explode'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-titles-utf8_convert';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Titles';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 1;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_people-titles-utf8_convert'] = $feeds_tamper;

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_people-created_date-adjust_date_to_timezone';
  $feeds_tamper->importer = 'os_people';
  $feeds_tamper->source = 'Created date';
  $feeds_tamper->plugin_id = 'adjust_date_to_timezone';
  $feeds_tamper->settings = '';
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Adjust date to timezone';
  $export['os_people-created_date-adjust_date_to_timezone'] = $feeds_tamper;

  return $export;
}
