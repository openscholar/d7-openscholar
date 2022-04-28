<?php
/**
 * @file
 * os_reader.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function os_reader_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'os_feed_importers';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'OS Feed Importers';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['spaces'] = array(
    'type' => 'spaces_og',
    'frontpage' => 0,
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'os-reader/[nid]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  $handler->display->display_options['fields']['title']['spaces'] = array(
    'type' => 'spaces_og',
    'frontpage' => 0,
  );
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
    'feed_importer' => 'feed_importer',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $export['os_feed_importers'] = $view;

  $view = new view();
  $view->name = 'os_reader_feeds';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'os_feed_item';
  $view->human_name = 'OS Reader feeds';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'OS Reader';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'description' => 'title',
    'copy_feed_to_news' => 'copy_feed_to_news',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'description' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'copy_feed_to_news' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No results.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: OS feed item: Importer_nid */
  $handler->display->display_options['relationships']['importer_nid']['id'] = 'importer_nid';
  $handler->display->display_options['relationships']['importer_nid']['table'] = 'os_feed_item';
  $handler->display->display_options['relationships']['importer_nid']['field'] = 'importer_nid';
  $handler->display->display_options['relationships']['importer_nid']['label'] = 'Importer';
  /* Field: OS feed item: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'os_feed_item';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<div class="feed-item-title">+ [title]</div>';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: OS feed item: Description */
  $handler->display->display_options['fields']['description']['id'] = 'description';
  $handler->display->display_options['fields']['description']['table'] = 'os_feed_item';
  $handler->display->display_options['fields']['description']['field'] = 'description';
  $handler->display->display_options['fields']['description']['label'] = '';
  $handler->display->display_options['fields']['description']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['description']['alter']['text'] = '<div class="feed-item-description">[description]</div>';
  $handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
  /* Field: OS feed item: Copy Feed to News */
  $handler->display->display_options['fields']['copy_feed_to_news']['id'] = 'copy_feed_to_news';
  $handler->display->display_options['fields']['copy_feed_to_news']['table'] = 'os_feed_item';
  $handler->display->display_options['fields']['copy_feed_to_news']['field'] = 'copy_feed_to_news';
  $handler->display->display_options['fields']['copy_feed_to_news']['label'] = '';
  $handler->display->display_options['fields']['copy_feed_to_news']['element_label_colon'] = FALSE;
  /* Sort criterion: OS feed item: Created */
  $handler->display->display_options['sorts']['time']['id'] = 'created';
  $handler->display->display_options['sorts']['time']['table'] = 'os_feed_item';
  $handler->display->display_options['sorts']['time']['field'] = 'created';
  $handler->display->display_options['sorts']['time']['order'] = 'DESC';
  /* Contextual filter: OS feed item: Importer_nid */
  $handler->display->display_options['arguments']['importer_nid']['id'] = 'importer_nid';
  $handler->display->display_options['arguments']['importer_nid']['table'] = 'os_feed_item';
  $handler->display->display_options['arguments']['importer_nid']['field'] = 'importer_nid';
  $handler->display->display_options['arguments']['importer_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['importer_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['importer_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['importer_nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: OS feed item: Contains */
  $handler->display->display_options['filters']['feed_by_text']['id'] = 'feed_by_text';
  $handler->display->display_options['filters']['feed_by_text']['table'] = 'os_feed_item';
  $handler->display->display_options['filters']['feed_by_text']['field'] = 'feed_by_text';
  $handler->display->display_options['filters']['feed_by_text']['group'] = 1;
  $handler->display->display_options['filters']['feed_by_text']['exposed'] = TRUE;
  $handler->display->display_options['filters']['feed_by_text']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['feed_by_text']['expose']['label'] = 'Contains';
  $handler->display->display_options['filters']['feed_by_text']['expose']['operator'] = 'feed_by_text_op';
  $handler->display->display_options['filters']['feed_by_text']['expose']['identifier'] = 'feed_by_text';
  $handler->display->display_options['filters']['feed_by_text']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: OS feed item: Is Imported */
  $handler->display->display_options['filters']['feed_is_imported']['id'] = 'feed_is_imported';
  $handler->display->display_options['filters']['feed_is_imported']['table'] = 'os_feed_item';
  $handler->display->display_options['filters']['feed_is_imported']['field'] = 'feed_is_imported';
  $handler->display->display_options['filters']['feed_is_imported']['value'] = '0';
  $handler->display->display_options['filters']['feed_is_imported']['group'] = 1;
  $handler->display->display_options['filters']['feed_is_imported']['exposed'] = TRUE;
  $handler->display->display_options['filters']['feed_is_imported']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['feed_is_imported']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['feed_is_imported']['expose']['operator'] = 'feed_is_imported_op';
  $handler->display->display_options['filters']['feed_is_imported']['expose']['identifier'] = 'feed_is_imported';
  $handler->display->display_options['filters']['feed_is_imported']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';
  $handler->display->display_options['filters']['current']['relationship'] = 'importer_nid';
  $export['os_reader_feeds'] = $view;

  $view = new view();
  $view->name = 'reader';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Reader feed';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Reader';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'spaces_feature';
  $handler->display->display_options['access']['spaces_feature'] = 'os_reader';
  $handler->display->display_options['cache']['type'] = 'time_per_gorup';
  $handler->display->display_options['cache']['results_lifespan'] = '518400';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '518400';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['og_access'] = 0;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Redirect */
  $handler->display->display_options['fields']['field_url']['id'] = 'field_url';
  $handler->display->display_options['fields']['field_url']['table'] = 'field_data_field_url';
  $handler->display->display_options['fields']['field_url']['field'] = 'field_url';
  $handler->display->display_options['fields']['field_url']['label'] = '';
  $handler->display->display_options['fields']['field_url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_url']['click_sort_column'] = 'url';
  $handler->display->display_options['fields']['field_url']['settings'] = array(
    'num_to_show' => '3',
  );
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
    'feed' => 'feed',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'reader';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Reader';
  $handler->display->display_options['menu']['name'] = 'primary-menu';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Latest Feed Items';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['reader'] = $view;

  return $export;
}
