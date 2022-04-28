<?php

/**
 * @file
 * Administrative page callbacks for Os Reports module.
 */


/**
 * OS Site report form
 */
function os_reports_site_form($form, &$form_state) {
  os_common_angular_apps('ReportModule');

  $form['#attached'] = array(
    'js' => array(
      array(
        'data' => array(
          'osRestModulePath' => $GLOBALS['base_url'] . '/' . drupal_get_path('module', 'os_reports'),
          'restBasePath' => url('api'),
        ),
        'type' => 'setting',
      ),
      array (
        'data' => drupal_get_path('module', 'os_reports').'/js/ReportModule.js',
        'type' => 'file',
      ),
    ),
    'css' => array(
      drupal_get_path('module', 'os_reports') . '/os_reports.css',
      drupal_get_path('theme', 'seven') . '/style.css',
    ),
    'library' => array(
      array('os_common', 'EntityService'),
      array('os_common', 'auth'),
    )
  );

  $form['#attributes'] = array(
    'ng-controller' => 'SiteReportQuery',
    'ng-app' => 'ReportModule',
    'name' => 'reportForm',
  );
  $form['keyword_set'] = array(
    '#type' => 'fieldset',
    '#title' => 'Keyword search'
  );
  $form['keyword_set']['keyword'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => 'Keyword',
    '#attributes' => array(
      'ng-model' => 'params.keyword',
    ),
  );
  $form['keyword_set']['kfields'] = array(
    '#prefix' => '<div class="container-inline"><label for="edit-kfields">Search in fields</label>',
    '#suffix' => '</div>',
    '#type' => 'checkboxes',
    '#options' => array(),
    '#validated' => TRUE,
  );
  $form['keyword_set']['kfields']['site_owner_email'] = array(
    '#title' => 'site owner email',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.kfields.site_owner_email',
      'ng-click' => "updateCheckedValues('kfields', 'site_owner_email')",
      'value' => 'u.mail',
    ),
  );
  $form['keyword_set']['kfields']['username'] = array(
    '#title' => 'site owner username',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.kfields.username',
      'ng-click' => "updateCheckedValues('kfields', 'username')",
      'value' => 'u.name',
    ),
  );
  $form['keyword_set']['kfields']['site_title'] = array(
    '#title' => 'site title',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.kfields.site_title',
      'ng-click' => "updateCheckedValues('kfields', 'site_title')",
      'value' => 'n.title',
    ),
  );
  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => 'Site Content'
  );
  $form['content']['lastupdatebefore'] = array(
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
    '#type' => 'textfield',
    '#title' => 'Content last updated before',
    '#size' => 10,
    '#description' => 'format: YYYYMMDD',
    '#attributes' => array(
      'ng-model' => 'params.lastupdatebefore',
      'ng-blur' => 'updateValues(params.lastupdatebefore)',
    ),
  );
  $form['content']['includesites'] = array(
    '#prefix' => '<div class="container-inline"><label for="edit-includesites">Sites to include</label>',
    '#suffix' => '</div>',
    '#options' => array(),
  );
  $form['content']['includesites']['all'] = array(
    '#name' => 'includesites',
    '#title' => 'all sites',
    '#type' => 'radio',
    '#attributes' => array(
      'ng-model' => 'params.includesites',
      'value' => 'all',
      'ng-click' => 'updateValues(params.includesites)',
    ),
  );
  $form['content']['includesites']['content'] = array(
    '#name' => 'includesites',
    '#title' => 'sites with content',
    '#type' => 'radio',
    '#attributes' => array(
      'ng-model' => 'params.includesites',
      'value' => 'content',
      'ng-click' => 'updateValues(params.includesites)',
    ),
  );
  $form['content']['includesites']['noncontent'] = array(
    '#name' => 'includesites',
    '#title' => 'sites with non-content changes',
    '#type' => 'radio',
    '#attributes' => array(
      'ng-model' => 'params.includesites',
      'value' => 'noncontent',
      'ng-click' => 'updateValues(params.includesites)',
    ),
  );
  $form['content']['includesites']['nocontent'] = array(
    '#name' => 'includesites',
    '#title' => 'sites with no content or non-content changes',
    '#type' => 'radio',
    '#attributes' => array(
      'ng-model' => 'params.includesites',
      'value' => 'nocontent',
      'ng-click' => 'updateValues(params.includesites)',
    ),
  );
  $form['createdrange'] = array(
    '#type' => 'fieldset',
    '#title' => 'Site creation',
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['createdrange']['creationstart'] = array(
    '#type' => 'textfield',
    '#title' => 'Between',
    '#size' => 10,
    '#attributes' => array(
      'ng-model' => 'params.creationstart',
      'ng-blur' => 'updateValues(params.creationstart)',
    ),
  );
  $form['createdrange']['creationend'] = array(
    '#type' => 'textfield',
    '#title' => 'and',
    '#size' => 10,
    '#description' => 'format: YYYYMMDD',
    '#attributes' => array(
      'ng-model' => 'params.creationend',
      'ng-blur' => 'updateValues(params.creationend)',
    ),
  );
  $form['columns_set'] = array(
    '#type' => 'fieldset',
    '#title' => 'Columns',
  );
  $form['columns_set']['only_vsite_ids'] = array(
    '#type' => 'checkbox',
    '#title' => 'Vsite IDs only',
    '#description' => 'Checking this means all "optional columns" below will be ignored.',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.only_vsite_ids',
      'ng-click' => "updateCheckedValues('columns', 'only_vsite_ids')",
      'value' => 'only_vsite_ids',
    ),
  );
  $form['columns_set']['columns'] = array(
    '#prefix' => '<label for="edit-columns">Optional Columns</label><div class="container-inline">',
    '#suffix' => '</div>',
    '#type' => 'checkboxes',
    '#validated' => TRUE,
    '#options' => array(),
  );
  $form['columns_set']['columns']['vsite_id'] = array(
    '#title' => 'Vsite ID',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.vsite_id',
      'ng-click' => "updateCheckedValues('columns', 'vsite_id')",
      'value' => 'vsite_id',
    ),
  );
  if (module_exists('os_pinserver_auth') && variable_get('pinserver_pin_url', FALSE)) {
    $form['columns_set']['columns']['site_owner_linked_huid'] = array(
      '#title' => 'Site owner has linked HKey',
      '#type' => 'checkbox',
      '#attributes' => array(
        'ng-model' => 'queryform.columns.site_owner_linked_huid',
        'ng-click' => "updateCheckedValues('columns', 'site_owner_linked_huid')",
        'value' => 'site_owner_linked_huid',
      ),
    );
  }
  if (module_exists('os_pinserver_auth') && variable_get('pinserver_pin_url', FALSE)) {
    $form['columns_set']['columns']['site_owner_huid'] = array(
      '#title' => 'Site owner HUID',
      '#type' => 'checkbox',
      '#attributes' => array(
        'ng-model' => 'queryform.columns.site_owner_huid',
        'ng-click' => "updateCheckedValues('columns', 'site_owner_huid')",
        'value' => 'site_owner_huid',
      ),
    );
  }
  $form['columns_set']['columns']['site_created'] = array(
    '#title' => 'Creation date',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.site_created',
      'ng-click' => "updateCheckedValues('columns', 'site_created')",
      'value' => 'site_created',
    ),
  );
  $form['columns_set']['columns']['site_created_by'] = array(
    '#title' => 'Created by',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.site_created_by',
      'ng-click' => "updateCheckedValues('columns', 'site_created_by')",
      'value' => 'site_created_by',
    ),
  );
  $form['columns_set']['columns']['content_last_updated'] = array(
    '#title' => 'Last content update',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.content_last_updated',
      'ng-click' => "updateCheckedValues('columns', 'content_last_updated')",
      'value' => 'content_last_updated',
    ),
  );
  $form['columns_set']['columns']['other_site_changes'] = array(
    '#title' => 'Non-content changes',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.other_site_changes',
      'ng-click' => "updateCheckedValues('columns', 'other_site_changes')",
      'value' => 'other_site_changes',
    ),
  );
  $form['columns_set']['columns']['site_privacy_setting'] = array(
    '#title' => 'Privacy level',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.site_privacy_setting',
      'ng-click' => "updateCheckedValues('columns', 'site_privacy_setting')",
      'value' => 'site_privacy_setting',
    ),
  );
  $form['columns_set']['columns']['custom_domain'] = array(
    '#title' => 'Custom domain',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.custom_domain',
      'ng-click' => "updateCheckedValues('columns', 'custom_domain')",
      'value' => 'custom_domain',
    ),
  );
  $form['columns_set']['columns']['custom_theme_uploaded'] = array(
    '#title' => 'Custom theme uploaded',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.custom_theme_uploaded',
      'ng-click' => "updateCheckedValues('columns', 'custom_theme_uploaded')",
      'value' => 'custom_theme_uploaded',
    ),
  );
  $form['columns_set']['columns']['preset'] = array(
    '#title' => 'Site type/preset',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.preset',
      'ng-click' => "updateCheckedValues('columns', 'preset')",
      'value' => 'preset',
    ),
  );
  $form['columns_set']['columns']['num_nodes'] = array(
    '#title' => 'Number of posts in site',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.num_nodes',
      'ng-click' => "updateCheckedValues('columns', 'num_nodes')",
      'value' => 'num_nodes',
    ),
  );
  $form['columns_set']['columns']['num_files'] = array(
    '#title' => 'Number of files associated with site',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.num_files',
      'ng-click' => "updateCheckedValues('columns', 'num_files')",
      'value' => 'num_files',
    ),
  );
  $form['columns_set']['columns']['num_widgets'] = array(
    '#title' => 'Number of widgets in site',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.num_widgets',
      'ng-click' => "updateCheckedValues('columns', 'num_widgets')",
      'value' => 'num_widgets',
    ),
  );
  $form['columns_set']['columns']['num_members'] = array(
    '#title' => 'Number of site members',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.num_members',
      'ng-click' => "updateCheckedValues('columns', 'num_members')",
      'value' => 'num_members',
    ),
  );
  $form['columns_set']['columns']['num_redirects'] = array(
    '#title' => 'Number of redirects in site',
    '#type' => 'checkbox',
    '#attributes' => array(
      'ng-model' => 'queryform.columns.num_redirects',
      'ng-click' => "updateCheckedValues('columns', 'num_redirects')",
      'value' => 'num_redirects',
    ),
  );
  $handler = restful_get_restful_handler('report_sites');
  $form['range'] = array(
    '#value' => $handler->getRange(),
    '#type' => 'hidden',
    '#attributes' => array(
      'ng-model' => 'params.range',
      'ng-init' => "params.range = " . $handler->getRange(),
    ),
  );

  $form['buttons'] = array(
    '#prefix' => '<div class="buttons">',
    '#suffix' => '</div>',
  );
  $form['buttons']['submit'] = array(
    '#markup' => '<a class="button" ng-click="update()">Run Report</a>',
  );
  $form['buttons']['clear'] = array(
    '#markup' => '<a class="button" ng-click="reset()">Clear</a>',
  );
  $form['buttons']['download'] = array(
    '#type' => 'submit',
    '#value' => 'Download CSV of Full Report',
  );

  $buttons = '<div class="pager"><div class="container-inline"><div class="previous"><a ng-click="pager(\'previous\')">‹ previous</a></div><div class="status_count">{{ status }}</div><div class="next"><a ng-click="pager(\'next\')">next ›</a></div></div></div>';

  $form['results'] = array(
    '#markup' => $buttons . '<div class="results"><table><thead><tr><th ng-repeat="header in headers" ng-class="isActive(header)"><a ng-click="sort(this)" class="{{ header }}" ng-bind-html="formatHeader(header)"></a></th></tr></thead><tbody><tr ng-repeat="row in rows" ng-class-even="\'even\'" ng-class-odd="\'odd\'"><td ng-repeat="header in headers"><span ng-bind-html="row[header] | makelink:header:row"></span></td></tr></tbody></table></div>' . $buttons,
  );

  return $form;
}

// download CSV of report results
function os_reports_site_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $params = $form_state['input'];

  // clean up parameters before rest call
  foreach ($params as $key => $value) {
    if (!$value) {
      unset($params[$key]);
    }
    if (is_array($value)) {
      $values = array_filter(array_values($value));
      $params[$key] = $values;
    }
  }
  $params['range'] = 0;
  $params['exclude'] = array('feed_importer', 'profile', 'harvard_course');

  // handle vsite ids only checkbox
  if (isset($params['only_vsite_ids'])) {
    unset($params['only_vsite_ids']);
    $params['columns'][] = 'only_vsite_ids';
  }

  // make restful call to get data
  $handler = restful_get_restful_handler('report_sites');
  $results = $handler->post('', $params);

  if ($results) {
    $headers = array_keys($results[0]);
    $operations = array(array("os_reports_assemble_header_row", array($headers)));

    while($results) {
      $rows = array_splice($results, 0, 50);
      $operations[] = array("os_reports_assemble_file_rows", array($rows));
    }
    $operations[] = array("os_reports_print_file", array());
    $batch = array(
      'operations' =>  $operations,
      'title' => t('Creating the CSV file'),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('File creation has encountered an error.'),
    );

    batch_set($batch);
    $batch =& batch_get();
    $batch['progressive'] = FALSE;
    batch_process($batch);
  }
  else {
    drupal_set_message('No results in report.', 'warning');
  }
}

