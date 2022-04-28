<?php
/**
 * @file
 * os_reader.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_reader_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'reader_reader';
  $context->description = '';
  $context->tag = 'OS features';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'feed' => 'feed',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'reader' => 'reader',
        'reader/*' => 'reader/*',
      ),
    ),
    'views' => array(
      'values' => array(
        'reader' => 'reader',
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
        'boxes-boxes-os_boxes_feedreader' => array(
          'module' => 'boxes',
          'delta' => 'boxes-os_boxes_feedreader',
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
  $export['reader_reader'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_rss_feed';
  $context->description = 'RSS feed';
  $context->tag = 'OS features';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'rss-feed' => 'rss-feed',
        'rss-feed/*' => 'rss-feed/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-os_rss_feed-block' => array(
          'module' => 'views',
          'delta' => 'os_rss_feed-block',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('OS features');
  t('RSS feed');
  $export['os_rss_feed'] = $context;

  return $export;
}
