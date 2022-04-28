<?php
/**
 * @file
 * os_profiles.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_profiles_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'profiles_profiles';
  $context->description = '';
  $context->tag = 'OS features';
  $context->conditions = array(
    'menu' => array(
      'values' => array(
        'people' => 'people',
      ),
    ),
    'node' => array(
      'values' => array(
        'person' => 'person',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'people' => 'people',
        'people/*' => 'people/*',
        'browse/people' => 'browse/people',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_profiles' => 'os_profiles',
        'os_profiles:page' => 'os_profiles:page',
      ),
    ),
  );

  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-os_profiles-filter_by_alphabet' => array(
          'module' => 'views',
          'delta' => 'os_profiles-filter_by_alphabet',
          'weight' => -8,
          'region' => FALSE,
          'info' => t('Filter Profiles by Alphabetical Groups'),
        ),
      ),
    ),
  );

  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OS features');
  $export['profiles_profiles'] = $context;

  return $export;
}
