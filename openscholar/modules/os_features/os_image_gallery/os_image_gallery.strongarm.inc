<?php
/**
 * @file
 * os_image_gallery.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function os_image_gallery_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_media_gallery';
  $strongarm->value = 0;
  $export['comment_anonymous_media_gallery'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_media_gallery';
  $strongarm->value = 1;
  $export['comment_default_mode_media_gallery'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_media_gallery';
  $strongarm->value = '50';
  $export['comment_default_per_page_media_gallery'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_media_gallery';
  $strongarm->value = '0';
  $export['comment_media_gallery'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_show_preview_changes_node_media_gallery';
  $strongarm->value = 0;
  $export['diff_show_preview_changes_node_media_gallery'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeformscols_field_placements_media_gallery_default';
  $strongarm->value = array(
    'title' => array(
      'region' => 'main',
      'weight' => '0',
      'has_required' => TRUE,
      'title' => 'Title',
    ),
    'actions' => array(
      'region' => 'footer',
      'weight' => '23',
      'has_required' => FALSE,
      'title' => 'Save',
      'hidden' => 0,
    ),
    'revision_information' => array(
      'region' => 'right',
      'weight' => '6',
      'has_required' => FALSE,
      'title' => 'Revision information',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'author' => array(
      'region' => 'right',
      'weight' => '4',
      'has_required' => FALSE,
      'title' => 'Authoring information',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'options' => array(
      'region' => 'right',
      'weight' => '1',
      'has_required' => FALSE,
      'title' => 'Publishing options',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'media_gallery_description' => array(
      'region' => 'main',
      'weight' => '1',
      'has_required' => FALSE,
      'title' => 'Description',
      'hidden' => 0,
    ),
    'media_gallery_file' => array(
      'region' => 'main',
      'weight' => '14',
      'has_required' => FALSE,
      'title' => 'Gallery media',
      'hidden' => 0,
    ),
    'media_gallery_format' => array(
      'region' => 'main',
      'weight' => '2',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_lightbox_extras' => array(
      'region' => 'main',
      'weight' => '3',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_columns' => array(
      'region' => 'main',
      'weight' => '4',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_rows' => array(
      'region' => 'main',
      'weight' => '5',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_image_info_where' => array(
      'region' => 'main',
      'weight' => '6',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_allow_download' => array(
      'region' => 'main',
      'weight' => '7',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_expose_block' => array(
      'region' => 'main',
      'weight' => '8',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_block_columns' => array(
      'region' => 'main',
      'weight' => '9',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_block_rows' => array(
      'region' => 'main',
      'weight' => '10',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'og_group_ref' => array(
      'region' => 'main',
      'weight' => '15',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'og_vocabulary' => array(
      'region' => 'main',
      'weight' => '16',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'comment_settings' => array(
      'region' => 'right',
      'weight' => '5',
      'has_required' => FALSE,
      'title' => 'Comment settings',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'path' => array(
      'region' => 'right',
      'weight' => '2',
      'has_required' => FALSE,
      'title' => 'URL path settings',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'os_menu' => array(
      'region' => 'right',
      'weight' => '3',
      'has_required' => FALSE,
      'title' => 'Menu settings',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'media_gallery_weight' => array(
      'region' => 'main',
      'weight' => '11',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'media_gallery_collection' => array(
      'region' => 'main',
      'weight' => '12',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'settings_wrapper' => array(
      'region' => 'main',
      'weight' => '13',
      'has_required' => TRUE,
      'title' => 'Gallery settings',
    ),
    'actions_top' => array(
      'region' => 'right',
      'weight' => '0',
      'has_required' => FALSE,
      'title' => 'Save',
      'hidden' => 0,
    ),
  );
  $export['nodeformscols_field_placements_media_gallery_default'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_media_gallery';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_media_gallery'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_media_gallery';
  $strongarm->value = FALSE;
  $export['node_submitted_media_gallery'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_media_gallery_pattern';
  $strongarm->value = 'galleries/[node:title]';
  $export['pathauto_node_media_gallery_pattern'] = $strongarm;

  return $export;
}
