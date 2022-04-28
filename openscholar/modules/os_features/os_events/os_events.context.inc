<?php
/**
 * @file
 * os_events.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_events_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'events_events';
  $context->description = 'Calendar and Events';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'event' => 'event',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'calendar' => 'calendar',
        'calendar/*' => 'calendar/*',
        'browse/events' => 'browse/events',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_events' => 'os_events',
        'os_events:page' => 'os_events:page',
        'os_events:page_1' => 'os_events:page_1',
        'os_events:page_2' => 'os_events:page_2',
        'os_events:page_3' => 'os_events:page_3',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-os_events-block_1' => array(
          'module' => 'views',
          'delta' => 'os_events-block_1',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'boxes-os_events_upcoming' => array(
          'module' => 'boxes',
          'delta' => 'os_events_upcoming',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Calendar and Events');
  t('OS features');
  $export['events_events'] = $context;

  return $export;
}
