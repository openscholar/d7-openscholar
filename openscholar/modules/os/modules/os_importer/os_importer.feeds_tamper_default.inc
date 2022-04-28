<?php
/**
 * @file
 * os_importer.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function os_importer_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'os_menu-link_title-utf8_convert';
  $feeds_tamper->importer = 'os_menu';
  $feeds_tamper->source = 'Link Title';
  $feeds_tamper->plugin_id = 'utf8_convert';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Convert to UTF-8';
  $export['os_menu-link_title-utf8_convert'] = $feeds_tamper;

  return $export;
}
