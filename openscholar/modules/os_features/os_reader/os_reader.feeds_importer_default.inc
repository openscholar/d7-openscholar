<?php
/**
 * @file
 * os_reader.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_reader_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_feed';
  $feeds_importer->config = array(
    'name' => 'OS feed',
    'description' => 'Importing feed via CSV',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'ics csv',
        'direct' => FALSE,
        'directory' => 'public://feeds',
        'allowed_schemes' => array(
          0 => 'public',
        ),
      ),
    ),
    'parser' => array(
      'plugin_key' => 'OsFeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'OsImporterNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'Title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'URL',
            'target' => 'field_url:url',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Created date',
            'target' => 'created',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Path',
            'target' => 'path_alias',
            'pathauto_override' => 1,
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'filtered_html',
        'skip_hash_check' => 0,
        'content_type' => 'feed',
        'file_field' => NULL,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['os_feed'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_reader';
  $feeds_importer->config = array(
    'name' => 'OS reader',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'OsFeedFetcher',
      'config' => array(),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'OsFeedReaderFetcher',
      'config' => array(
        'type' => array(),
        'mappings' => array(
          0 => array(
            'source' => 'guid',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'description',
            'target' => 'description',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'timestamp',
            'target' => 'created',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'url',
            'target' => 'url',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => 2,
        'skip_hash_check' => 1,
      ),
    ),
    'content_type' => 'feed_importer',
    'update' => 0,
    'import_period' => '86400',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['os_reader'] = $feeds_importer;

  return $export;
}
