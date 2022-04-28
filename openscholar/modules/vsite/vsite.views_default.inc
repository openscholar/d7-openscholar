<?php
/**
 * @file
 * vsite.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function vsite_views_default_views() {
  // Use the default logo from the os_boxes_site_logo image.
  $default_image_path = drupal_get_path('module', 'os_boxes')."/plugins/os_boxes_site_logo/default_logo.jpg";
  $default_img = !empty($default_image_path) ? '<img src="'.$default_image_path.'" alt="default logo">' : '';

  $export = array();

  $view = new view();
  $view->name = 'os_subsites';
  $view->description = 'A view showing the sites in a multi-tenant install';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'os_subsites';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Featured Sites';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'og_context';
  $handler->display->display_options['access']['perm'] = 'administer group';
  $handler->display->display_options['access']['group_id_arg'] = '';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '24';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No Results Text';
  $handler->display->display_options['empty']['area']['content'] = 'No sites have been created yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Group Homepage Path */
  $handler->display->display_options['fields']['field_group_path']['id'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['table'] = 'field_data_field_group_path';
  $handler->display->display_options['fields']['field_group_path']['field'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['label'] = '';
  $handler->display->display_options['fields']['field_group_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_group_path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_group_path']['click_sort_column'] = 'url';
  /* Field: Content: Site Logo */
  $handler->display->display_options['fields']['field_site_logo']['id'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['table'] = 'field_data_field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['field'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['label'] = '';
  $handler->display->display_options['fields']['field_site_logo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_site_logo']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['field_site_logo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_site_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_site_logo']['settings'] = array(
    'image_style' => 'square_thumbnail',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Site Description */
  $handler->display->display_options['fields']['field_site_description']['id'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['table'] = 'field_data_field_site_description';
  $handler->display->display_options['fields']['field_site_description']['field'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['label'] = '';
  $handler->display->display_options['fields']['field_site_description']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: group_parent (field_group_parent) */
  $handler->display->display_options['arguments']['field_group_parent_target_id']['id'] = 'field_group_parent_target_id';
  $handler->display->display_options['arguments']['field_group_parent_target_id']['table'] = 'field_data_field_group_parent';
  $handler->display->display_options['arguments']['field_group_parent_target_id']['field'] = 'field_group_parent_target_id';
  $handler->display->display_options['arguments']['field_group_parent_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_group_parent_target_id']['default_argument_type'] = 'og_context';
  $handler->display->display_options['arguments']['field_group_parent_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_group_parent_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_group_parent_target_id']['summary_options']['items_per_page'] = '25';
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
    'personal' => 'personal',
    'project' => 'project',
    'department' => 'department',
  );
  /* Filter criterion: Content: Site visibility (group_access) */
  $handler->display->display_options['filters']['group_access_value']['id'] = 'group_access_value';
  $handler->display->display_options['filters']['group_access_value']['table'] = 'field_data_group_access';
  $handler->display->display_options['filters']['group_access_value']['field'] = 'group_access_value';
  $handler->display->display_options['filters']['group_access_value']['value'] = array(
    0 => '0',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'subsites';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['cache'] = FALSE;
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '600';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '600';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Group Homepage Path */
  $handler->display->display_options['fields']['field_group_path']['id'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['table'] = 'field_data_field_group_path';
  $handler->display->display_options['fields']['field_group_path']['field'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['label'] = '';
  $handler->display->display_options['fields']['field_group_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_group_path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_group_path']['click_sort_column'] = 'url';
  /* Field: Content: Site Logo */
  $handler->display->display_options['fields']['field_site_logo']['id'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['table'] = 'field_data_field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['field'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['label'] = '';
  $handler->display->display_options['fields']['field_site_logo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_site_logo']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['field_site_logo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_site_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_site_logo']['settings'] = array(
    'image_style' => 'square_thumbnail',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Site Description */
  $handler->display->display_options['fields']['field_site_description']['id'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['table'] = 'field_data_field_site_description';
  $handler->display->display_options['fields']['field_site_description']['field'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['label'] = '';
  $handler->display->display_options['fields']['field_site_description']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  $export['os_subsites'] = $view;

  $view = new view();
  $view->name = 'os_websites';
  $view->description = 'A view showing the sites in a multi-tenant install';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'os_websites';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Featured Sites';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '24';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No Results Text';
  $handler->display->display_options['empty']['area']['content'] = 'No sites have been created yet.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Content: Group Homepage Path */
  $handler->display->display_options['fields']['field_group_path']['id'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['table'] = 'field_data_field_group_path';
  $handler->display->display_options['fields']['field_group_path']['field'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['label'] = '';
  $handler->display->display_options['fields']['field_group_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_group_path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_group_path']['click_sort_column'] = 'url';
  /* Field: Content: Site Logo */
  $handler->display->display_options['fields']['field_site_logo']['id'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['table'] = 'field_data_field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['field'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['label'] = '';
  $handler->display->display_options['fields']['field_site_logo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_site_logo']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['field_site_logo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_site_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_site_logo']['settings'] = array(
    'image_style' => 'square_thumbnail',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Site Description */
  $handler->display->display_options['fields']['field_site_description']['id'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['table'] = 'field_data_field_site_description';
  $handler->display->display_options['fields']['field_site_description']['field'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['label'] = '';
  $handler->display->display_options['fields']['field_site_description']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
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
    'personal' => 'personal',
    'project' => 'project',
    'department' => 'department',
  );
  /* Filter criterion: Content: Site visibility (group_access) */
  $handler->display->display_options['filters']['group_access_value']['id'] = 'group_access_value';
  $handler->display->display_options['filters']['group_access_value']['table'] = 'field_data_group_access';
  $handler->display->display_options['filters']['group_access_value']['field'] = 'group_access_value';
  $handler->display->display_options['filters']['group_access_value']['value'] = array(
    0 => '0',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Search';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Group Homepage Path */
  $handler->display->display_options['fields']['field_group_path']['id'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['table'] = 'field_data_field_group_path';
  $handler->display->display_options['fields']['field_group_path']['field'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['label'] = '';
  $handler->display->display_options['fields']['field_group_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_group_path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_group_path']['click_sort_column'] = 'url';
  /* Field: Content: Site Logo */
  $handler->display->display_options['fields']['field_site_logo']['id'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['table'] = 'field_data_field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['field'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['label'] = '';
  $handler->display->display_options['fields']['field_site_logo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_site_logo']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['field_site_logo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_site_logo']['empty'] = $default_img;
  $handler->display->display_options['fields']['field_site_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_site_logo']['settings'] = array(
    'image_style' => 'square_thumbnail',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Site Description */
  $handler->display->display_options['fields']['field_site_description']['id'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['table'] = 'field_data_field_site_description';
  $handler->display->display_options['fields']['field_site_description']['field'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['label'] = '';
  $handler->display->display_options['fields']['field_site_description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_site_description']['type'] = 'text_plain';
  $handler->display->display_options['path'] = 'websites';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['defaults']['cache'] = FALSE;
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '600';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '600';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '9';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Group Homepage Path */
  $handler->display->display_options['fields']['field_group_path']['id'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['table'] = 'field_data_field_group_path';
  $handler->display->display_options['fields']['field_group_path']['field'] = 'field_group_path';
  $handler->display->display_options['fields']['field_group_path']['label'] = '';
  $handler->display->display_options['fields']['field_group_path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_group_path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_group_path']['click_sort_column'] = 'url';
  /* Field: Content: Site Logo */
  $handler->display->display_options['fields']['field_site_logo']['id'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['table'] = 'field_data_field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['field'] = 'field_site_logo';
  $handler->display->display_options['fields']['field_site_logo']['label'] = '';
  $handler->display->display_options['fields']['field_site_logo']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_site_logo']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['field_site_logo']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_site_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_site_logo']['empty'] = $default_img;
  $handler->display->display_options['fields']['field_site_logo']['settings'] = array(
    'image_style' => 'square_thumbnail',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = '[field_group_path]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Site Description */
  $handler->display->display_options['fields']['field_site_description']['id'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['table'] = 'field_data_field_site_description';
  $handler->display->display_options['fields']['field_site_description']['field'] = 'field_site_description';
  $handler->display->display_options['fields']['field_site_description']['label'] = '';
  $handler->display->display_options['fields']['field_site_description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_site_description']['type'] = 'text_plain';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Sticky */
  $handler->display->display_options['sorts']['sticky']['id'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['table'] = 'node';
  $handler->display->display_options['sorts']['sticky']['field'] = 'sticky';
  $handler->display->display_options['sorts']['sticky']['order'] = 'DESC';
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';

  $export['os_websites'] = $view;

  return $export;
}
