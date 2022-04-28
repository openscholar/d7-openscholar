<?php
/**
 * @file
 * os.box.inc
 */

/**
 * Implements hook_default_box().
 */
function os_default_box() {
  $export = array();

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_addthis';
  $box->plugin_key = 'os_boxes_addthis';
  $box->title = '';
  $box->description = 'AddThis';
  $box->options = array(
    'visibility' => 'feature',
    'widget_type' => 'toolbox_small',
    'toolbox_services' => 'facebook,twitter,email,linkedin,google_plusone',
    'additional_classes' => '',
  );
  $export['os_addthis'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'os_rss';
  $box->plugin_key = 'os_boxes_rss';
  $box->title = '';
  $box->description = 'Site RSS Feed';
  $box->options = array(
    'show_all' => 1,
    'content' => array(
      'link' => 0,
      'news' => 0,
      'media_gallery' => 0,
      'biblio' => 0,
      'page' => 0,
      'event' => 0,
      'feed' => 0,
      'blog' => 0,
      'presentation' => 0,
      'person' => 0,
    ),
    'additional_classes' => '',
  );
  $export['os_rss'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'site_info';
  $box->plugin_key = 'os_boxes_site_info';
  $box->title = '';
  $box->description = 'Site Info';
  $box->options = array(
    'site_title' => 'Your Name',
    'descrip' => 'Hover over here, and then click the gear to change me.',
    'address1' => '13 Somewhere Dr.',
    'address2' => 'That Town, USA',
  );
  $export['site_info'] = $box;

  $box = new stdClass();
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'site_logo';
  $box->plugin_key = 'os_boxes_site_logo';
  $box->title = '';
  $box->description = 'Site Logo';
  $box->options = array(
    'visibility' => 'feature',
    'fid' => NULL,
    'x' => 0,
    'y' => 0,
    'height' => 100,
    'width' => 150,
  );
  $export['site_logo'] = $box;

  return $export;
}
