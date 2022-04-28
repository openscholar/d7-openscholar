<?php
/**
 * @file
 * os_classes.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_classes_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'classes_classes';
  $context->description = '';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'class' => 'class',
        'class_material' => 'class_material',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'classes' => 'classes',
        'classes/*' => 'classes/*',
        'browse/classes' => 'browse/classes',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_classes' => 'os_classes',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-os_addthis' => array(
          'module' => 'boxes',
          'delta' => 'os_addthis',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
    'menu' => 'classes',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OS features');
  $export['classes_classes'] = $context;

  return $export;
}
