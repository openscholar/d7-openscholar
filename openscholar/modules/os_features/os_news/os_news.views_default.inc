<?php
/**
 * @file
 * os_news.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function os_news_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'os_news';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OS News';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'News';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'spaces_feature';
  $handler->display->display_options['access']['spaces_feature'] = 'os_news';
  $handler->display->display_options['cache']['type'] = 'time_per_gorup';
  $handler->display->display_options['cache']['results_lifespan'] = '518400';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '518400';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['og_access'] = 0;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'infinite_scroll';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['no_autoscroll'] = '4';
  $handler->display->display_options['pager']['options']['load_previous'] = 1;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Date (field_news_date) */
  $handler->display->display_options['sorts']['field_news_date_value_1']['id'] = 'field_news_date_value_1';
  $handler->display->display_options['sorts']['field_news_date_value_1']['table'] = 'field_data_field_news_date';
  $handler->display->display_options['sorts']['field_news_date_value_1']['field'] = 'field_news_date_value';
  $handler->display->display_options['sorts']['field_news_date_value_1']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'news' => 'news',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'news';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'News';
  $handler->display->display_options['menu']['description'] = 'List of news';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'primary-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['row_options']['item_length'] = 'rss';
  $handler->display->display_options['path'] = 'news.rss';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );

  /* Display: Filter News Block by Month */
  $handler = $view->new_display('block', 'Filter News Block by Month', 'news_by_month_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Filter News by Month';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['pure_distinct'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Date (field_news_date) */
  $handler->display->display_options['arguments']['field_news_date_value']['id'] = 'field_news_date_value';
  $handler->display->display_options['arguments']['field_news_date_value']['table'] = 'field_data_field_news_date';
  $handler->display->display_options['arguments']['field_news_date_value']['field'] = 'field_news_date_value';
  $handler->display->display_options['arguments']['field_news_date_value']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['field_news_date_value']['default_argument_type'] = 'date';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_news_date_value']['summary_options']['base_path'] = 'news/archive';
  $handler->display->display_options['arguments']['field_news_date_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_news_date_value']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_news_date_value']['year_range'] = '1900:2100';
  $handler->display->display_options['arguments']['field_news_date_value']['use_fromto'] = 'no';
  $handler->display->display_options['arguments']['field_news_date_value']['title_format'] = 'custom';
  $handler->display->display_options['arguments']['field_news_date_value']['title_format_custom'] = 'F Y';

  /* Display: Filter News Block by Year */
  $handler = $view->new_display('block', 'Filter News Block by Year', 'news_by_year_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Filter News by Year';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Date (field_news_date) */
  $handler->display->display_options['arguments']['field_news_date_value']['id'] = 'field_news_date_value';
  $handler->display->display_options['arguments']['field_news_date_value']['table'] = 'field_data_field_news_date';
  $handler->display->display_options['arguments']['field_news_date_value']['field'] = 'field_news_date_value';
  $handler->display->display_options['arguments']['field_news_date_value']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['field_news_date_value']['default_argument_type'] = 'date';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_news_date_value']['summary_options']['base_path'] = 'news/archive';
  $handler->display->display_options['arguments']['field_news_date_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_news_date_value']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_news_date_value']['granularity'] = 'year';
  $handler->display->display_options['arguments']['field_news_date_value']['use_fromto'] = 'no';
  $handler->display->display_options['arguments']['field_news_date_value']['title_format'] = 'custom';
  $handler->display->display_options['arguments']['field_news_date_value']['title_format_custom'] = 'Y';

  /* Display: Page for News Archive */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Date (field_news_date) */
  $handler->display->display_options['arguments']['field_news_date_value']['id'] = 'field_news_date_value';
  $handler->display->display_options['arguments']['field_news_date_value']['table'] = 'field_data_field_news_date';
  $handler->display->display_options['arguments']['field_news_date_value']['field'] = 'field_news_date_value';
  $handler->display->display_options['arguments']['field_news_date_value']['default_argument_type'] = 'date';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_news_date_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_news_date_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_news_date_value']['year_range'] = '1900:2100';
  $handler->display->display_options['path'] = 'news/archive';
  $handler->display->display_options['menu']['title'] = 'News';
  $handler->display->display_options['menu']['description'] = 'List of news';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'primary-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $export['os_news'] = $view;

  return $export;
}
