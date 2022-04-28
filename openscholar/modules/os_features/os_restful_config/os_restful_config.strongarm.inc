<?php
/**
 * @file
 * os_restful_config.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function os_restful_config_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_default_output_formatter';
  $strongarm->value = 'json';
  $export['restful_default_output_formatter'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_enable_discovery_resource';
  $strongarm->value = 1;
  $export['restful_enable_discovery_resource'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_enable_users_resource';
  $strongarm->value = 0;
  $export['restful_enable_users_resource'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_enable_user_login_resource';
  $strongarm->value = 1;
  $export['restful_enable_user_login_resource'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_file_upload';
  $strongarm->value = 0;
  $export['restful_file_upload'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_file_upload_allow_anonymous_user';
  $strongarm->value = 0;
  $export['restful_file_upload_allow_anonymous_user'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_global_rate_limit';
  $strongarm->value = '0';
  $export['restful_global_rate_limit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_global_rate_period';
  $strongarm->value = 'P1D';
  $export['restful_global_rate_period'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_hijack_api_pages';
  $strongarm->value = 1;
  $export['restful_hijack_api_pages'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_hook_menu_base_path';
  $strongarm->value = 'api';
  $export['restful_hook_menu_base_path'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_page_cache';
  $strongarm->value = FALSE;
  $export['restful_page_cache'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restful_render_cache';
  $strongarm->value = 0;
  $export['restful_render_cache'] = $strongarm;

  return $export;
}
