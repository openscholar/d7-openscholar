<?php
/**
 * @file
 * os_mailchimp.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_mailchimp_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_box_mailchimp';
  $box->plugin_key = 'os_boxes_mailchimp';
  $box->title = '';
  $box->description = 'Subscribe to MailChimp mailing list';
  $box->options = array(
    'visibility' => 'feature',
    'api' => FALSE,
    'list' => FALSE,
    'text' => 'Subscribe to our mailing list',
    'display' => 'link',
  );
  $export['os_box_mailchimp'] = $box;

  return $export;
}
