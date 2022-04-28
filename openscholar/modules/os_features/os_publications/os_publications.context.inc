<?php
/**
 * @file
 * os_publications.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_publications_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'publications_publications';
  $context->description = 'List of publications';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'biblio' => 'biblio',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'publications' => 'publications',
        'publications/*' => 'publications/*',
        'browse/publications' => 'browse/publications',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'publications',
    'block' => array(
      'blocks' => array(
        'boxes-os_publications_recent' => array(
          'module' => 'boxes',
          'delta' => 'os_publications_recent',
          'weight' => 21,
          'region' => 'sidebar_second',
          'label' => 'Recent publications',
        ),
        'boxes-os_addthis' => array(
          'module' => 'boxes',
          'delta' => 'os_addthis',
          'label' => 'AddThis Button',
          'weight' => 22,
          'region' => 'sidebar_second',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('List of publications');
  t('OS features');
  $export['publications_publications'] = $context;

  return $export;
}
