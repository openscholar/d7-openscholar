<?php
/**
 * @file
 * harvard_courses.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function harvard_courses_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'harvard_courses';
  $context->description = '';
  $context->tag = 'Harvard';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'harvard_course' => 'harvard_course',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'courses' => 'courses',
        'browse/courses' => 'browse/courses',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'harvard_courses-courses_filter' => array(
          'module' => 'harvard_courses',
          'delta' => 'courses_filter',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Harvard');
  $export['harvard_courses'] = $context;

  return $export;
}
