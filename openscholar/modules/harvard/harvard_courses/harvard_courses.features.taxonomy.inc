<?php
/**
 * @file
 * harvard_courses.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function harvard_courses_taxonomy_default_vocabularies() {
  return array(
    'schools_names' => array(
      'name' => 'Schools names',
      'machine_name' => 'schools_names',
      'description' => 'List of Harvard courses schools names',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
