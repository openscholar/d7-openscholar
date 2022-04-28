<?php
/**
 * @file
 * os_shields.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_shields_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'shield';
  $box->plugin_key = 'os_shields_box';
  $box->title = '';
  $box->description = 'Shield';
  $box->options = array();
  $export['shield'] = $box;

  return $export;
}