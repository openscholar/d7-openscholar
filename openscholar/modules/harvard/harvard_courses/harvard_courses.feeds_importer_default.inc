<?php
/**
 * @file
 * harvard_courses.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function harvard_courses_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'course';
  $feeds_importer->config = array(
    'name' => 'Course',
    'description' => 'Import courses',
    'fetcher' => array(
      'plugin_key' => 'HarvardFetcher',
      'config' => array(
        'debug_mode' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsXPathParserXML',
      'config' => array(
        'sources' => array(
          'xpathparser:14' => '@id',
          'xpathparser:0' => 'course_number',
          'xpathparser:1' => 'description',
          'xpathparser:2' => 'school_name',
          'xpathparser:4' => 'meeting_time',
          'xpathparser:5' => 'notes',
          'xpathparser:6' => 'course_number',
          'xpathparser:7' => 'prereq',
          'xpathparser:9' => 'term_description',
          'xpathparser:11' => 'location',
          'xpathparser:13' => 'title',
          'xpathparser:15' => 'calendar_period',
          'xpathparser:16' => 'url',
          'xpathparser:17' => 'crossreg',
          'xpathparser:18' => 'exam_group',
          'xpathparser:19' => 'faculty_description',
          'xpathparser:20' => 'department',
          'xpathparser:21' => 'credits',
          'xpathparser:22' => 'section_number',
        ),
        'rawXML' => array(
          'xpathparser:14' => 0,
          'xpathparser:0' => 0,
          'xpathparser:1' => 0,
          'xpathparser:2' => 0,
          'xpathparser:4' => 0,
          'xpathparser:5' => 0,
          'xpathparser:6' => 0,
          'xpathparser:7' => 0,
          'xpathparser:9' => 0,
          'xpathparser:11' => 0,
          'xpathparser:13' => 0,
          'xpathparser:15' => 0,
          'xpathparser:16' => 0,
          'xpathparser:17' => 0,
          'xpathparser:18' => 0,
          'xpathparser:19' => 0,
          'xpathparser:20' => 0,
          'xpathparser:21' => 0,
          'xpathparser:22' => 0,
        ),
        'context' => '//courses/course',
        'exp' => array(
          'errors' => 0,
          'debug' => array(
            'context' => 0,
            'xpathparser:14' => 0,
            'xpathparser:0' => 0,
            'xpathparser:1' => 0,
            'xpathparser:2' => 0,
            'xpathparser:4' => 0,
            'xpathparser:5' => 0,
            'xpathparser:6' => 0,
            'xpathparser:7' => 0,
            'xpathparser:9' => 0,
            'xpathparser:11' => 0,
            'xpathparser:13' => 0,
            'xpathparser:15' => 0,
            'xpathparser:16' => 0,
            'xpathparser:17' => 0,
            'xpathparser:18' => 0,
            'xpathparser:19' => 0,
            'xpathparser:20' => 0,
            'xpathparser:21' => 0,
            'xpathparser:22' => 0,
          ),
        ),
        'allow_override' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'harvard_course',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'xpathparser:14',
            'target' => 'guid',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'xpathparser:0',
            'target' => 'field_cat_number',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'xpathparser:1',
            'target' => 'field_course_description',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'xpathparser:2',
            'target' => 'field_faculty',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'xpathparser:4',
            'target' => 'field_meetings',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'xpathparser:5',
            'target' => 'field_notes',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'xpathparser:6',
            'target' => 'field_number',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'xpathparser:7',
            'target' => 'field_prerequisites',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'xpathparser:9',
            'target' => 'field_term',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'xpathparser:11',
            'target' => 'field_building',
            'unique' => FALSE,
          ),
          10 => array(
            'source' => 'xpathparser:13',
            'target' => 'title',
            'unique' => 0,
          ),
          11 => array(
            'source' => 'xpathparser:15',
            'target' => 'field_calendar_period',
            'unique' => FALSE,
          ),
          12 => array(
            'source' => 'xpathparser:16',
            'target' => 'field_course_url',
            'unique' => FALSE,
          ),
          13 => array(
            'source' => 'xpathparser:17',
            'target' => 'field_cross_registration',
            'unique' => FALSE,
          ),
          14 => array(
            'source' => 'xpathparser:18',
            'target' => 'field_exam_group',
            'unique' => FALSE,
          ),
          15 => array(
            'source' => 'xpathparser:19',
            'target' => 'field_faculty_description',
            'unique' => FALSE,
          ),
          16 => array(
            'source' => 'xpathparser:20',
            'target' => 'field_department_id',
            'unique' => FALSE,
          ),
          17 => array(
            'source' => 'xpathparser:21',
            'target' => 'field_credits',
            'unique' => FALSE,
          ),
          18 => array(
            'source' => 'xpathparser:22',
            'target' => 'field_section_number',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'filtered_html',
        'skip_hash_check' => FALSE,
        'authorize' => TRUE,
      ),
    ),
    'content_type' => 'harvard_api_importer',
    'update' => 0,
    'import_period' => '1800',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['course'] = $feeds_importer;

  return $export;
}
