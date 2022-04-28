<?php
/**
 * @file
 * os_pages.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_pages_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_browse_pages';
  $context->description = '';
  $context->tag = 'OS features';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'browse/pages' => 'browse/pages',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-vocabulary_filter' => array(
          'module' => 'boxes',
          'delta' => 'vocabulary_filter',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OS features');
  $export['os_browse_pages'] = $context;

  return $export;
}
