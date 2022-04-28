<?php
/**
 * @file
 * cp.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function cp_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'redirect_page_cache';
  $strongarm->value = 1;
  $export['redirect_page_cache'] = $strongarm;

  return $export;
}
