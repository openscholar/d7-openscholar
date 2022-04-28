<?php
/**
 * @file
 * os_profiles.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function os_profiles_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'os_people';
  $feeds_importer->config = array(
    'name' => 'OS people',
    'description' => 'Importing people from node.',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'ics csv',
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
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'Prefix',
            'target' => 'field_prefix',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'First name',
            'target' => 'field_first_name',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'Middle name',
            'target' => 'field_middle_name_or_initial',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'Last name',
            'target' => 'field_last_name',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Photo',
            'target' => 'person_photo',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Title 1',
            'target' => 'field_professional_title',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'Title 2',
            'target' => 'field_professional_title',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'Title 3',
            'target' => 'field_professional_title',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'Address',
            'target' => 'field_address',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'Phone',
            'target' => 'field_phone',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'Email',
            'target' => 'field_email',
            'unique' => FALSE,
          ),
          11 => array(
            'source' => 'Websites title 1',
            'target' => 'field_website:title',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'Websites url 1',
            'target' => 'field_website:url',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'Websites title 2',
            'target' => 'field_website:title',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'Websites url 2',
            'target' => 'field_website:url',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'Websites title 3',
            'target' => 'field_website:title',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'Websites url 3',
            'target' => 'field_website:url',
            'unique' => FALSE,
          ),
          17 => array(
            'source' => 'Short bio',
            'target' => 'body',
            'unique' => FALSE,
          ),
          18 => array(
            'source' => 'Created date',
            'target' => 'created',
            'unique' => FALSE,
          ),
          19 => array(
            'source' => 'Path',
            'target' => 'path_alias',
            'pathauto_override' => 1,
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'filtered_html',
        'skip_hash_check' => 0,
        'file_field' => 'field_upload',
        'content_type' => 'person',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['os_people'] = $feeds_importer;

  return $export;
}
