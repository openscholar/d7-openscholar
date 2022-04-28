<?php
/**
 * @file
 * os_image_gallery.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_image_gallery_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_gallery';
  $feeds_importer->config = array(
    'name' => 'OS gallery',
    'description' => 'Import gallery from CSV file',
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
        'file_field' => 'media_gallery_file',
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
            'target' => 'media_gallery_description',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Rows',
            'target' => 'media_gallery_rows',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Columns',
            'target' => 'media_gallery_columns',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Images',
            'target' => 'file',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Created date',
            'target' => 'created',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'Path',
            'target' => 'path_alias',
            'pathauto_override' => 1,
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'filtered_html',
        'skip_hash_check' => 0,
        'content_type' => 'media_gallery',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => 1,
  );
  $export['os_gallery'] = $feeds_importer;

  return $export;
}
