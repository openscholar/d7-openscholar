<?php
/**
 * @file
 * os_software.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function os_software_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'os_software_projects';
  $view->description = 'Os Software > Software';
  $view->tag = 'os_feature';
  $view->base_table = 'node';
  $view->human_name = 'OS Software Projects';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Software';
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
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty text';
  $handler->display->display_options['empty']['area']['content'] = 'There are currently no projects available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
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
    'software_project' => 'software_project',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'software';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Software';
  $handler->display->display_options['menu']['name'] = 'primary-menu';

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['os_software_projects'] = $view;

  $view = new view();
  $view->name = 'os_software_releases';
  $view->description = 'Provides release displays for project nodes.';
  $view->tag = 'os_feature';
  $view->base_table = 'node';
  $view->human_name = 'OS Software Releases';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'All Releases';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'more releases';
  $handler->display->display_options['link_display'] = 'custom_url';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
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
  );
  /* Relationship: Content: Package (field_software_package:fid) */
  $handler->display->display_options['relationships']['field_software_package_fid']['id'] = 'field_software_package_fid';
  $handler->display->display_options['relationships']['field_software_package_fid']['table'] = 'field_data_field_software_package';
  $handler->display->display_options['relationships']['field_software_package_fid']['field'] = 'field_software_package_fid';
  $handler->display->display_options['relationships']['field_software_package_fid']['required'] = TRUE;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
  $handler->display->display_options['fields']['uri']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['fields']['uri']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uri']['file_download_path'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['fields']['filesize']['exclude'] = TRUE;
  /* Field: Content: Version */
  $handler->display->display_options['fields']['field_software_version']['id'] = 'field_software_version';
  $handler->display->display_options['fields']['field_software_version']['table'] = 'field_data_field_software_version';
  $handler->display->display_options['fields']['field_software_version']['field'] = 'field_software_version';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Package';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="[uri]">Download</a> ([filesize])';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = '<a href="[path]">Release info</a>';
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = '/';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'M j Y';
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
    'software_release' => 'software_release',
  );
  /* Filter criterion: Content: Recommended (field_software_recommended) */
  $handler->display->display_options['filters']['field_software_recommended_value']['id'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['table'] = 'field_data_field_software_recommended';
  $handler->display->display_options['filters']['field_software_recommended_value']['field'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['value'] = array(
    0 => '0',
  );

  /* Display: Recent */
  $handler = $view->new_display('entity_view', 'Recent', 'entity_view_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recent Releases';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['link_url'] = 'software/!1/releases';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Software Project (field_software_project) */
  $handler->display->display_options['arguments']['field_software_project_target_id']['id'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['table'] = 'field_data_field_software_project';
  $handler->display->display_options['arguments']['field_software_project_target_id']['field'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'software_release' => 'software_release',
  );
  /* Filter criterion: Content: Recommended (field_software_recommended) */
  $handler->display->display_options['filters']['field_software_recommended_value']['id'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['table'] = 'field_data_field_software_recommended';
  $handler->display->display_options['filters']['field_software_recommended_value']['field'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'software_project',
  );
  $handler->display->display_options['show_title'] = 1;

  /* Display: Recommended */
  $handler = $view->new_display('entity_view', 'Recommended', 'entity_view_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recommended Release';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['link_url'] = 'software/!1/releases';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Software Project (field_software_project) */
  $handler->display->display_options['arguments']['field_software_project_target_id']['id'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['table'] = 'field_data_field_software_project';
  $handler->display->display_options['arguments']['field_software_project_target_id']['field'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'software_release' => 'software_release',
  );
  /* Filter criterion: Content: Recommended (field_software_recommended) */
  $handler->display->display_options['filters']['field_software_recommended_value']['id'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['table'] = 'field_data_field_software_recommended';
  $handler->display->display_options['filters']['field_software_recommended_value']['field'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'software_project',
  );
  $handler->display->display_options['show_title'] = 1;

  /* Display: All */
  $handler = $view->new_display('page', 'All', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '15';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
  $handler->display->display_options['fields']['uri']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['fields']['uri']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uri']['file_download_path'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['fields']['filesize']['exclude'] = TRUE;
  /* Field: Content: Version */
  $handler->display->display_options['fields']['field_software_version']['id'] = 'field_software_version';
  $handler->display->display_options['fields']['field_software_version']['table'] = 'field_data_field_software_version';
  $handler->display->display_options['fields']['field_software_version']['field'] = 'field_software_version';
  $handler->display->display_options['fields']['field_software_version']['label'] = 'Release';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Package';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="[uri]">Download</a> ([filesize])';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = '';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Release info';
  $handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['nothing_1']['alter']['path'] = '/[path]';
  $handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'M j Y';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Software Project (field_software_project) */
  $handler->display->display_options['arguments']['field_software_project_target_id']['id'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['table'] = 'field_data_field_software_project';
  $handler->display->display_options['arguments']['field_software_project_target_id']['field'] = 'field_software_project_target_id';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_software_project_target_id']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['field_software_project_target_id']['title'] = '<em>%1</em> All Releases';
  $handler->display->display_options['arguments']['field_software_project_target_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_software_project_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_software_project_target_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_software_project_target_id']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_software_project_target_id']['validate_options']['types'] = array(
    'software_project' => 'software_project',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'software_release' => 'software_release',
  );
  $handler->display->display_options['path'] = 'software/%/releases';

  /* Display: Filesize Field */
  $handler = $view->new_display('entity_view', 'Filesize Field', 'entity_view_3');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'software_release' => 'software_release',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['entity_type'] = 'node';
  $handler->display->display_options['bundles'] = array(
    0 => 'software_release',
  );

  /* Display: Widget */
  $handler = $view->new_display('block', 'Widget', 'block_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Software Releases';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['use_more'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_always'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['defaults']['use_more_text'] = FALSE;
  $handler->display->display_options['use_more_text'] = 'more releases';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'field_software_version' => 'field_software_version',
    'created' => 'created',
    'uri' => 'uri',
    'filesize' => 'filesize',
  );
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_field_software_project_node']['id'] = 'reverse_field_software_project_node';
  $handler->display->display_options['relationships']['reverse_field_software_project_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_software_project_node']['field'] = 'reverse_field_software_project_node';
  $handler->display->display_options['relationships']['reverse_field_software_project_node']['label'] = 'Release';
  /* Relationship: Content: Package (field_software_package:fid) */
  $handler->display->display_options['relationships']['field_software_package_fid']['id'] = 'field_software_package_fid';
  $handler->display->display_options['relationships']['field_software_package_fid']['table'] = 'field_data_field_software_package';
  $handler->display->display_options['relationships']['field_software_package_fid']['field'] = 'field_software_package_fid';
  $handler->display->display_options['relationships']['field_software_package_fid']['relationship'] = 'reverse_field_software_project_node';
  $handler->display->display_options['relationships']['field_software_package_fid']['label'] = 'package file';
  $handler->display->display_options['relationships']['field_software_package_fid']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['spaces'] = array(
    'type' => 'spaces_og',
    'frontpage' => 0,
  );
  /* Field: Release Node Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'node';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['relationship'] = 'reverse_field_software_project_node';
  $handler->display->display_options['fields']['path']['ui_name'] = 'Release Node Path';
  $handler->display->display_options['fields']['path']['label'] = '';
  $handler->display->display_options['fields']['path']['exclude'] = TRUE;
  $handler->display->display_options['fields']['path']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['path']['absolute'] = TRUE;
  /* Field: Content: Version */
  $handler->display->display_options['fields']['field_software_version']['id'] = 'field_software_version';
  $handler->display->display_options['fields']['field_software_version']['table'] = 'field_data_field_software_version';
  $handler->display->display_options['fields']['field_software_version']['field'] = 'field_software_version';
  $handler->display->display_options['fields']['field_software_version']['relationship'] = 'reverse_field_software_project_node';
  $handler->display->display_options['fields']['field_software_version']['label'] = '';
  $handler->display->display_options['fields']['field_software_version']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['field_software_version']['alter']['path'] = '[path]';
  $handler->display->display_options['fields']['field_software_version']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['field_software_version']['element_label_colon'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'reverse_field_software_project_node';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['created']['alter']['text'] = '[created]<br/>';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'M j Y';
  /* Field: File: Path */
  $handler->display->display_options['fields']['uri']['id'] = 'uri';
  $handler->display->display_options['fields']['uri']['table'] = 'file_managed';
  $handler->display->display_options['fields']['uri']['field'] = 'uri';
  $handler->display->display_options['fields']['uri']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['fields']['uri']['label'] = '';
  $handler->display->display_options['fields']['uri']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['uri']['alter']['text'] = '<a href="[uri]">Download</a>';
  $handler->display->display_options['fields']['uri']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uri']['file_download_path'] = TRUE;
  /* Field: File: Size */
  $handler->display->display_options['fields']['filesize']['id'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['table'] = 'file_managed';
  $handler->display->display_options['fields']['filesize']['field'] = 'filesize';
  $handler->display->display_options['fields']['filesize']['relationship'] = 'field_software_package_fid';
  $handler->display->display_options['fields']['filesize']['label'] = '';
  $handler->display->display_options['fields']['filesize']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['filesize']['alter']['text'] = '([filesize])';
  $handler->display->display_options['fields']['filesize']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['relationship'] = 'reverse_field_software_project_node';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'software_project' => 'software_project',
  );
  /* Filter criterion: Spaces: Content in current space */
  $handler->display->display_options['filters']['current']['id'] = 'current';
  $handler->display->display_options['filters']['current']['table'] = 'spaces';
  $handler->display->display_options['filters']['current']['field'] = 'current';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'reverse_field_software_project_node';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  /* Filter criterion: Content: Recommended (field_software_recommended) */
  $handler->display->display_options['filters']['field_software_recommended_value']['id'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['table'] = 'field_data_field_software_recommended';
  $handler->display->display_options['filters']['field_software_recommended_value']['field'] = 'field_software_recommended_value';
  $handler->display->display_options['filters']['field_software_recommended_value']['relationship'] = 'reverse_field_software_project_node';
  $handler->display->display_options['filters']['field_software_recommended_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_software_recommended_value']['reduce_duplicates'] = TRUE;
  $export['os_software_releases'] = $view;

  return $export;
}
