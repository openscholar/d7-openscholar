<?php
/**
 * @file
 * os_importer.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_importer_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_documents';
  $feeds_importer->config = array(
    'name' => 'OS Documents',
    'description' => 'Import documents from CSV file.',
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
      'plugin_key' => 'FeedsCSVParser',
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
            'source' => 'Body',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Files',
            'target' => 'file',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Terms',
            'target' => 'og_vocab',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'filtered_html',
        'skip_hash_check' => 0,
        'content_type' => 'book',
        'file_field' => 'field_upload',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['os_documents'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_menu';
  $feeds_importer->config = array(
    'name' => 'OS Menu',
    'description' => 'Import menu items from CSV',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'csv',
        'direct' => 0,
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
      'plugin_key' => 'OsImporterMenuProcessor',
      'config' => array(
        'mappings' => array(
          0 => array(
            'source' => 'Link Title',
            'target' => 'link_title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'Menu',
            'target' => 'menu_name',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Weight',
            'target' => 'weight',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Path',
            'target' => 'link_path',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Parent Path',
            'target' => 'parent_path',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'skip_link_path_validation' => 1,
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['os_menu'] = $feeds_importer;

  return $export;
}
