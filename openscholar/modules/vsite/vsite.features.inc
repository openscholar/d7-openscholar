<?php
/**
 * @file
 * vsite.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vsite_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vsite_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function vsite_node_info() {
  $items = array(
    'department' => array(
      'name' => t('Department Site'),
      'base' => 'node_content',
      'description' => t('An OpenScholar site for a larger organization that may contain sub-sites.'),
      'has_title' => '1',
      'title_label' => t('Site Name'),
      'help' => '',
    ),
    'personal' => array(
      'name' => t('Personal Site'),
      'base' => 'node_content',
      'description' => t('An OpenScholar site for personal and academic profiles.'),
      'has_title' => '1',
      'title_label' => t('Site Name'),
      'help' => '',
    ),
    'project' => array(
      'name' => t('Project Site'),
      'base' => 'node_content',
      'description' => t('An OpenScholar site for projects.'),
      'has_title' => '1',
      'title_label' => t('Site Name'),
      'help' => '',
    ),
  );
  return $items;
}
