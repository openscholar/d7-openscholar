<?php
/**
 * @file
 * vsite_preset.box.inc
 */

/**
 * Implements hook_default_box().
 */
function vsite_preset_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'hwp_personal_contact_html';
  $box->plugin_key = 'os_boxes_html';
  $box->title = 'Contact';
  $box->description = 'Contact';
  $box->options = array(
    'text' => array(
      'value' => '<p>OpenScholar University<br> (800) 555-0000<br> Main Building<br> Your Town, State 00000<br> your_name@your_school.edu</p>',
      'format' => 'filtered_html',
    ),
    'format' => 1,
    'additional_classes' => '',
  );
  $export['hwp_personal_contact_html'] = $box;

  return $export;
}

