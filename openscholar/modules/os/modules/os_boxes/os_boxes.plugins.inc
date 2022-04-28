<?php

function _os_boxes_boxes_plugins() {
  $info = array();
  $path = drupal_get_path('module', 'os_boxes').'/plugins/';

  $info['os_boxes_default'] = array(
    'handler' => array(
      'class' => 'os_boxes_default',
      'file' => 'os_boxes_default.inc',
      'path' => $path,
      'parent' => 'box',
    ),
  );

  $info['os_boxes_widget_collection'] = array(
    'handler' => array(
      'class' => 'os_boxes_widget_collection',
      'file' => 'os_boxes_widget_collection.inc',
      'path' => $path.'/os_boxes_widget_collection',
      'parent' => 'os_boxes_default',
    )
  );

  $info['os_boxes_addthis'] = array(
    'title' => 'AddThis',
    'description' => 'Social networking links',
    'tags' => array('Social'),
    'handler' => array(
      'class' => 'os_boxes_addthis',
      'file' => 'os_boxes_addthis.inc',
      'path' => $path.'/os_boxes_addthis',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_manual_list'] = array(
    'title' => 'Featured Posts',
    'description' => 'Create your own list of posts',
    'tags' => array('Content'),
    'handler' => array(
      'class' => 'os_boxes_manual_list',
      'file' => 'os_boxes_manual_list.inc',
      'path' => $path.'/os_boxes_manual_list',
      'parent' => 'os_boxes_default',
    ),
    'weight' => -1
  );

  $info['os_boxes_media'] = array(
    'title' => 'Embed Media',
    'description' => 'Embed a video, audio or other internet resource',
    'tags' => array('Media'),
    'handler' => array(
      'class' => 'os_boxes_media',
      'file' => 'os_boxes_media.inc',
      'path' => $path.'/os_boxes_media',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_slideshow'] = array(
   'title' => 'Slideshow',
    'description' => 'Displays a slideshow of your uploaded pictures.',
    'module dependencies' => array('os_image_gallery'),
    'tags' => array('Media'),
    'handler' => array(
      'class' => 'os_boxes_slideshow',
      'file' => 'os_boxes_slideshow.inc',
      'path' => $path . '/os_boxes_slideshow',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_booktoc'] = array(
    'title' => 'Booklet: Table of Contents',
    'description' => 'The table of contents for a booklet',
    'tags' => array('Content'),
    'handler' => array(
      'class' => 'os_boxes_booktoc',
      'file' => 'os_boxes_booktoc.inc',
      'path' => $path.'/os_boxes_booktoc',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_twitterfeed'] = array(
    'title' => 'Twitter',
    'description' => 'Shows recent tweets',
    'tags' => array('Social'),
    'module dependencies' => array('twitter_pull'),
    'handler' => array(
      'class' => 'os_boxes_twitterfeed',
      'file' => 'os_boxes_twitterfeed.inc',
      'path' => $path.'/os_boxes_twitterfeed',
      'parent' => 'os_boxes_default',
    ),
  );

  // Publication Boxes
  $info['os_boxes_pub_ca'] = array(
    'title' => 'Publications: Authors',
    'description' => 'Displays a list of authors of your publications',
    'tags' => array('Content'),
    'module dependencies' => array('os_publications'),
    'handler' => array(
      'class' => 'os_boxes_pub_ca',
      'file' => 'os_boxes_pub_ca.inc',
      'path' => $path.'/os_boxes_pub_ca',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_pub_type'] = array(
    'title' => 'Publications: Types',
    'description' => 'Displays a list of publication types you have published to',
    'tags' => array('Content'),
    'module dependencies' => array('os_publications'),
    'handler' => array(
      'class' => 'os_boxes_pub_type',
      'file' => 'os_boxes_pub_type.inc',
      'path' => $path.'/os_boxes_pub_type',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_pub_year'] = array(
    'title' => 'Publications: Years',
    'description' => 'Displays a list of years you have published something in',
    'tags' => array('Content'),
    'module dependencies' => array('os_publications'),
    'handler' => array(
      'class' => 'os_boxes_pub_year',
      'file' => 'os_boxes_pub_year.inc',
      'path' => $path.'/os_boxes_pub_year',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_follow'] = array(
    'title' => 'Follow Me',
    'description' => 'Displays a list of links to your social media profiles.',
    'tags' => array('Social'),
    'handler' => array(
      'class' => 'os_boxes_follow',
      'file' => 'os_boxes_follow.inc',
      'path' => $path.'/os_boxes_follow',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_site_info'] = array(
    'title' => 'Site Info',
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_site_info',
      'file' => 'os_boxes_site_info.inc',
      'path' => $path.'/os_boxes_site_info',
      'parent' => 'os_boxes_default',
    ),
    'factory' => FALSE,
  );

  $info['os_boxes_site_logo'] = array(
    'title' => 'Site Logo',
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_site_logo',
      'file' => 'os_boxes_site_logo.inc',
      'path' => $path.'/os_boxes_site_logo',
      'parent' => 'os_boxes_default',
    ),
    'factory' => FALSE,
  );

  $info['os_taxonomy_fbt'] = array(
    'title' => 'Taxonomy',
    'description' => 'Filter content by category',
    'tags' => array('Content'),
    'handler' => array(
      'class' => 'os_taxonomy_fbt',
      'file' =>  'os_taxonomy_fbt.inc',
      'path' => $path . '/os_taxonomy_fbt',
      'parent' => 'os_boxes_default',
    ),
    'weight' => -2,
  );

  $info['os_boxes_tabs'] = array(
    'title' => 'Tabs',
    'description' => 'Place several widgets into a tabbed container',
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_tabs',
      'file' => 'os_boxes_tabs.inc',
      'path' => $path .'/os_boxes_tabs',
      'parent' => 'os_boxes_widget_collection',
    ),
  );

  $info['os_boxes_accordion'] = array(
    'title' => 'Accordion',
    'description' => 'Place several widgets into a tabbed container',
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_accordion',
      'file' => 'os_boxes_accordion.inc',
      'path' => $path .'/os_boxes_accordion',
      'parent' => 'os_boxes_widget_collection',
    ),
  );

  $info['os_boxes_rand_widget'] = array(
    'title' => 'Random Widget',
    'description' => 'Randomly show one widget from a list.',
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_rand_widget',
      'file' => 'os_boxes_rand_widget.inc',
      'path' => $path .'/os_boxes_rand_widget',
      'parent' => 'os_boxes_widget_collection',
    ),
  );

  $info['os_boxes_widget_slider'] = array(
    'title' => t('Widget Slider'),
    'description' => t('Compile a list of widgets to display in a slider/slideshow format.'),
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_widget_slider',
      'file' => 'os_boxes_widget_slider.inc',
      'path' => $path . '/os_boxes_widget_slider',
      'parent' => 'os_boxes_widget_collection',
    ),
  );

  $info['os_boxes_html'] = array(
    'title' => 'Custom Text/HTML',
    'description' => 'A blank slate to fill with what',
    'tags' => array('Content'),
    'handler' => array(
      'class' => 'os_boxes_html',
      'file' => 'os_boxes_html.inc',
      'path' => $path.'/os_boxes_html',
      'parent' => 'os_boxes_default',
    ),
    'weight' => -4
  );

  $info['os_boxes_rss'] = array(
    'title' => 'RSS feed',
    'description' => 'A feed of your posts allowing visitors to follow you.',
    'tags' => array('Social'),
    'handler' => array(
      'class' => 'os_boxes_rss',
      'file' => 'os_boxes_rss.inc',
      'path' => $path . '/os_boxes_rss',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_feedreader'] = array(
    'title' => 'Feed Reader',
    'description' => 'List of feed items',
    'tags' => array('Content','Social'),
    'module dependencies' => array("os_reader"),
    'handler' => array(
      'class' => 'os_boxes_feedreader',
      'file' => 'os_boxes_feedreader.inc',
      'path' => $path  . '/os_boxes_feedreader',
      'parent' => 'os_boxes_default',
    ),
  );

  $info['os_boxes_sectionnav'] = array(
    'title' => 'Section Nav',
    'description' => 'The navigation for a section',
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_sectionnav',
      'file' => 'os_boxes_sectionnav.inc',
      'path' => $path  . '/os_boxes_sectionnav',
      'parent' => 'os_boxes_default',
    ),
    'factory' => false,
  );

  $info['os_boxes_columns'] = array(
    'title' => 'Columns',
    'description' => 'Allows creation of new columns for widget placement',
    'tags' => array('Misc'),
    'handler' => array(
      'class' => 'os_boxes_columns',
      'file' => 'os_boxes_columns.inc',
      'path' => $path . '/os_boxes_columns',
      'parent' => 'os_boxes_default',
    )
  );

  if (module_exists('os_search_solr')) {
    $info['os_boxes_facetapi_vocabulary'] = array(
      'title' => 'Faceted taxonomy',
      'handler' => array(
        'class' => 'os_boxes_facetapi_vocabulary',
        'file' => 'os_boxes_facetapi_vocabulary.inc',
        'path' => $path . '/os_boxes_facetapi_vocabulary',
        'parent' => 'os_boxes_default',
      ),
    );
  }

  return $info;
}
