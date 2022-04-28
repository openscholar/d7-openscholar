<?php
/**
 * @file
 * Provides support for the Views module.
 */
class OsReaderViewsController extends EntityDefaultViewsController {

  /**
   * Override views_data().
   */
  public function views_data() {
    $data = parent::views_data();

    // Button for importing feed item to news.
    $data['os_feed_item']['copy_feed_to_news'] = array(
      'title' => t('Copy Feed to News'),
      'help' => t('Button for importing feed item to news.'),
      'field' => array(
        'handler' => 'os_reader_handler_copy_feed_to_news',
        'real field' => 'id',
      ),
    );

    // Filter feed items by whether or not they were imported to VSite.
    $data['os_feed_item']['feed_is_imported'] = array(
      'title' => t('Is Imported'),
      'help' => t('Filter feed items by whether or not they were imported to VSite.'),
      'filter' => array(
        'handler' => 'os_reader_filter_feed_is_imported',
      ),
    );

    // Filter feed items by text in the title or body.
    $data['os_feed_item']['feed_by_text'] = array(
      'title' => t('Contains'),
      'help' => t('Filter feed items by text in the title or body.'),
      'filter' => array(
        'handler' => 'os_reader_filter_feed_by_text',
      ),
    );

    // Allow some HTML.
    $data['os_feed_item']['description']['field']['handler'] = 'views_handler_field_markup';
    $data['os_feed_item']['description']['field']['format'] = 'filtered_html';

    return $data;
  }
}
