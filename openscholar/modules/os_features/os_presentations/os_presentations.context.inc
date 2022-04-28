<?php
/**
 * @file
 * os_presentations.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_presentations_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'presentations_presentations';
  $context->description = 'Adds blocks to OS Presentation pages';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'presentation' => 'presentation',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'presentations' => 'presentations',
        'presentations/*' => 'presentations/*',
        'browse/presentations' => 'browse/presentations',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_presentations' => 'os_presentations',
        'os_presentations:page' => 'os_presentations:page',
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
          'weight' => '-9',
        ),
        'boxes-os_presentations_recent' => array(
          'module' => 'boxes',
          'delta' => 'os_presentations_recent',
          'region' => 'sidebar_second',
          'weight' => '-8',
          'label' => 'Recent Presentations',
        ),
      ),
    ),
    'menu' => 'presentations',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Adds blocks to OS Presentation pages');
  t('OS features');
  $export['presentations_presentations'] = $context;

  return $export;
}
