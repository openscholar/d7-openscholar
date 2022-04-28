<?php

/**
 * @file
 * Views implementation for os files.
 */

/**
 * Implements hook_views_data().
 */
function os_files_views_data() {
  $data = array();

  // Filter by the file title or title real name.
  $data['file_managed']['file_name_title'] = array(
    'title' => t('File name or title'),
    'help' => t('Filter files by the title or the file name of display.'),
    'filter' => array(
      'handler' => 'os_files_filter_title_file_name',
    ),
  );

  return $data;
}