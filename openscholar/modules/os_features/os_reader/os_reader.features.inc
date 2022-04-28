<?php
/**
 * @file
 * os_reader.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_reader_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "boxes" && $api == "box") {
    return array("version" => "1");
  }
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "elysia_cron" && $api == "default_elysia_cron_rules") {
    return array("version" => "1");
  }
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }

  if ($module == "feeds_tamper" && $api == "feeds_tamper_default") {
    return array("version" => "2");
  }

  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function os_reader_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function os_reader_node_info() {
  $items = array(
    'feed' => array(
      'name' => t('Feed'),
      'base' => 'node_content',
      'description' => t('Used to show the contents of and aggregate external feeds.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'feed_importer' => array(
      'name' => t('Feed importer'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
