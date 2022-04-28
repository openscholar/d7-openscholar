<?php


/**
 * @file
 * Provides support for the Views module.
 */


/**
 * Implements hook_views_data().
 */
function cp_content_views_data() {
  $data['file_managed']['referencing_nodes'] = array(
    'group' => t('File'),
    'title' => t('Referencing content'),
    'help' => t('Show the content referencing files.'),
    // This is a dummy field, so point it to a real field that we need - the
    // group ID
    'field' => array(
      'field' => 'fid',
      'handler' => 'cp_content_handler_field_referencing_nodes',
    ),
  );

  return $data;
}

