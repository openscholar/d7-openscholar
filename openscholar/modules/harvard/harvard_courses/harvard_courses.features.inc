<?php
/**
 * @file
 * harvard_courses.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function harvard_courses_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "boxes" && $api == "box") {
    return array("version" => "1");
  }
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function harvard_courses_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function harvard_courses_node_info() {
  $items = array(
    'harvard_api_importer' => array(
      'name' => t('Harvard API importer'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'harvard_course' => array(
      'name' => t('Harvard Course'),
      'base' => 'node_content',
      'description' => t('OpenScholar course import module.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
