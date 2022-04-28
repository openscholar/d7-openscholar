<?php
/**
 * @file
 * iqss_scholars_front.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function iqss_scholars_front_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'os_stats_cache_expiration';
  $strongarm->value = 86400;
  $export['os_stats_cache_expiration'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'os_stats_enabled';
  $strongarm->value = array(
    'filesize' => 0,
    'users' => 0,
    'websites' => 1,
    'posts' => 1,
    'publications' => 1,
    'files' => 1,
  );
  $export['os_stats_enabled'] = $strongarm;

  return $export;
}
