<?php
/**
 * @file
 * os_news.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function os_news_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'news_news';
  $context->description = 'Context that is active in the news section of your site';
  $context->tag = 'OS features';
  $context->conditions = array(
    'menu' => array(
      'values' => array(
        'news' => 'news',
      ),
    ),
    'node' => array(
      'values' => array(
        'news' => 'news',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'news' => 'news',
        'news/*' => 'news/*',
        'browse/news' => 'browse/news',
      ),
    ),
    'views' => array(
      'values' => array(
        'os_news:page' => 'os_news:page',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'news',
    'block' => array(
      'blocks' => array(
        'boxes-os_news_latest' => array(
          'module' => 'boxes',
          'delta' => 'os_news_latest',
          'weight' => -21,
          'region' => 'sidebar_second',
          'label' => 'Latest News',
        ),
        'boxes-os_addthis' => array(
          'module' => 'boxes',
          'delta' => 'os_addthis',
          'label' => 'AddThis Button',
          'weight' => 50,
          'region' => 'sidebar_second',
        ),
        'views-os_news-news_by_month_block' => array(
          'module' => 'views',
          'delta' => 'os_news-news_by_month_block',
          'region' => 'sidebar_second',
          'weight' => -8,
          'info' => t('Filter News by Month'),
        ),
        'views-os_news-news_by_year_block' => array(
          'module' => 'views',
          'delta' => 'os_news-news_by_year_block',
          'region' => FALSE,
          'weight' => -8,
          'info' => t('Filter News by Year'),
        ),
      ),
    ),
  );

  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context that is active in the news section of your site');
  t('OS features');
  $export['news_news'] = $context;

  return $export;
}
