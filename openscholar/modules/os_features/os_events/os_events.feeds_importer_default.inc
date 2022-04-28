<?php
/**
 * @file
 * os_events.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_events_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_events_csv';
  $feeds_importer->config = array(
    'name' => 'OS events csv',
    'description' => 'Import events from CSV file.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
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
        'file_field' => 'field_upload',
        'content_type' => 'event',
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
            'source' => 'Body',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Start date',
            'target' => 'field_date:start',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'End date',
            'target' => 'field_date:end',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Location',
            'target' => 'field_event_location',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Registration',
            'target' => 'registration',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'Files',
            'target' => 'file',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'Created date',
            'target' => 'created',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'Path',
            'target' => 'path_alias',
            'pathauto_override' => 1,
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'filtered_html',
        'skip_hash_check' => 0,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => 1,
  );
  $export['os_events_csv'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_ical';
  $feeds_importer->config = array(
    'name' => 'OS iCal',
    'description' => 'Importing events from CSV.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml ics',
        'direct' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'DateIcalIcalcreatorParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'OsImporterNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'summary',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'dtstart',
            'target' => 'field_date:start',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'dtend',
            'target' => 'field_date:end',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Files',
            'target' => 'file',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'location',
            'target' => 'field_event_location',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'filtered_html',
        'skip_hash_check' => 0,
        'file_field' => 'field_upload',
        'content_type' => 'event',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['os_ical'] = $feeds_importer;

  return $export;
}
