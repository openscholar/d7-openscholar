<?php
/**
 * @file
 * os_profiles.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function os_profiles_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'os_profiles';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OS Profiles';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'People';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'spaces_feature';
  $handler->display->display_options['access']['spaces_feature'] = '0';
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
  $handler->display->display_options['pager']['options']['items_per_page'] = '15';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
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
  /* Sort criterion: Field: Last Name (field_last_name) */
  $handler->display->display_options['sorts']['field_last_name_value']['id'] = 'field_last_name_value';
  $handler->display->display_options['sorts']['field_last_name_value']['table'] = 'field_data_field_last_name';
  $handler->display->display_options['sorts']['field_last_name_value']['field'] = 'field_last_name_value';
  /* Sort criterion: Field: First Name (field_first_name:format) */
  $handler->display->display_options['sorts']['field_first_name_format']['id'] = 'field_first_name_format';
  $handler->display->display_options['sorts']['field_first_name_format']['table'] = 'field_data_field_first_name';
  $handler->display->display_options['sorts']['field_first_name_format']['field'] = 'field_first_name_format';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
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
    'person' => 'person',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'people';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'People';
  $handler->display->display_options['menu']['description'] = 'List of people';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'primary-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'people.rss';
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );

  /* Display: Filter by alphabetical grouping of Last Name */
  $handler = $view->new_display('block', 'Filter by alphabetical grouping of Last Name', 'filter_by_alphabet');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Filter by alphabetical grouping of Last Name';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field: Last Name (field_last_name) */
  $handler->display->display_options['arguments']['field_last_name_value']['id'] = 'field_last_name_value';
  $handler->display->display_options['arguments']['field_last_name_value']['table'] = 'field_data_field_last_name';
  $handler->display->display_options['arguments']['field_last_name_value']['field'] = 'field_last_name_value';
  $handler->display->display_options['arguments']['field_last_name_value']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['field_last_name_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_last_name_value']['default_argument_options']['argument'] = 'A';
  $handler->display->display_options['arguments']['field_last_name_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_last_name_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_last_name_value']['summary_options']['base_path'] = 'people/glossary';
  $handler->display->display_options['arguments']['field_last_name_value']['summary_options']['count'] = FALSE;
  $handler->display->display_options['arguments']['field_last_name_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_last_name_value']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_last_name_value']['validate']['fail'] = 'summary';
  $handler->display->display_options['arguments']['field_last_name_value']['glossary'] = TRUE;
  $handler->display->display_options['arguments']['field_last_name_value']['limit'] = '1';
  $handler->display->display_options['arguments']['field_last_name_value']['case'] = 'upper';
  $handler->display->display_options['arguments']['field_last_name_value']['path_case'] = 'lower';
  $handler->display->display_options['arguments']['field_last_name_value']['transform_dash'] = TRUE;
  $handler->display->display_options['arguments']['field_last_name_value']['break_phrase'] = TRUE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field: Last Name (field_last_name) */
  $handler->display->display_options['arguments']['field_last_name_value']['id'] = 'field_last_name_value';
  $handler->display->display_options['arguments']['field_last_name_value']['table'] = 'field_data_field_last_name';
  $handler->display->display_options['arguments']['field_last_name_value']['field'] = 'field_last_name_value';
  $handler->display->display_options['arguments']['field_last_name_value']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_last_name_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_last_name_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_last_name_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_last_name_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_last_name_value']['glossary'] = TRUE;
  $handler->display->display_options['arguments']['field_last_name_value']['limit'] = '1';
  $handler->display->display_options['arguments']['field_last_name_value']['break_phrase'] = TRUE;
  $handler->display->display_options['path'] = 'people/glossary';
  $export['os_profiles'] = $view;

  return $export;
}
