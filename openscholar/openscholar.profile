<?php

/**
 * Implements hook_entity_info_alter().
 */
function huji_pdf_entity_info_alter(&$info) {
  $info['node']['view modes']['export'] = array(
    'label' => t('Export'),
    'custom settings' => TRUE,
  );
}

/**
 * Implements hook_install_tasks().
 */
function openscholar_install_tasks($install_state) {
  $tasks = array();

  // OS flavors (production, development, etc).
  $tasks['openscholar_flavor_form'] = array(
    'display_name' => t('Choose environment'),
    'type' => 'form'
  );

  // Simple form to select the installation type (single site or multitenant)
  $tasks['openscholar_install_type'] = array(
    'display_name' => t('Installation type'),
    'type' => 'form'
  );

  // If multitenant, we need to do some extra work, e.g. some extra modules
  // otherwise, skip this step
  $tasks['openscholar_vsite_modules_batch'] = array(
    'display_name' => t('Install supplemental modules'),
    'type' => 'batch',
    'run' => variable_get('os_profile_type', FALSE == 'vsite' || variable_get('os_profile_flavor', FALSE) == 'development') ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP
  );

  // Migrating content if needed.
  $tasks['openscholar_migrate_content'] = array(
    'display_name' => t('Importing content'),
    'type' => 'batch',
    'display' => variable_get('os_dummy_content') && variable_get('os_profile_flavor', FALSE) == 'development',
    'run' => variable_get('os_dummy_content') && variable_get('os_profile_flavor', FALSE) == 'development' ? INSTALL_TASK_RUN_IF_REACHED : INSTALL_TASK_SKIP,
  );

  return $tasks;
}

function openscholar_install_tasks_alter(&$tasks, $install_state) {
  $tasks['install_finished']['function'] = 'openscholar_install_finished';
  $tasks['install_finished']['display_name'] = t('Finished');
  $tasks['install_finished']['type'] = 'normal';
}

/**
 * Flavor selection form.
 */
function openscholar_flavor_form($form, &$form_state) {
  $options = array(
    'production' => t('Production Deployment'),
    'development' => t('Development'),
  );

  $form['os_profile_flavor'] = array(
    '#title' => t('Select a flavor'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => 'development'
  );

  $form['intranet_site'] = array(
    '#type' => 'checkbox',
    '#title' => t('Intranet Site'),
    '#description' => t('If checked, all sites created will be private and have private files, public websites will not be available.'),
  );

  $form['dummy_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add dummy content'),
    '#description' => t('If checked, dummy content will be added to your openscholar site.'),
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="os_profile_flavor"]' => array('value' => 'development'),
      ),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  if (defined('DRUSH_BASE_PATH')) {
    // Set some sane defaults for Drush/Aegir non-interactive install
    variable_set('os_profile_flavor', 'production');
    variable_set('os_dummy_content', FALSE);
  }
  else {
    return $form;
  }
}


/**
 * Install type selection form
 */
function openscholar_install_type($form, &$form_state) {
  $options = array(
    'novsite' => t('Single site install'),
    'vsite' => t('Multi-tenant install'),
  );

  $form['os_profile_type'] = array(
    '#title' => t('Installation type'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => 'vsite',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  if (defined('DRUSH_BASE_PATH')) {
    // Set some sane defaults for Drush/Aegir non-interactive install
    variable_set('os_profile_type', 'vsite');
  }
  else {
    return $form;
  }
}


/**
 * Form submit handler when selecting an installation type
 */
function openscholar_flavor_form_submit($form, &$form_state) {
  //Save the chosen flavor
  variable_set('os_profile_flavor', $form_state['input']['os_profile_flavor']);

  // Define dummy content migration.
  if (!empty($form_state['input']['dummy_content'])) {
    variable_set('os_dummy_content', TRUE);
  }

  // Define dummy content migration.
  if (!empty($form_state['input']['intranet_site'])) {
    variable_set('file_default_scheme', 'private');

    $private_path = variable_get('file_private_path', '/private');
    variable_set('file_private_path', $private_path);
  }
}


/**
 * Form submit handler when selecting an installation type
 */
function openscholar_install_type_submit($form, &$form_state) {
  if(in_array($form_state['input']['os_profile_type'], array('vsite','single-tenant'))){
    variable_set('os_profile_type', $form_state['input']['os_profile_type']);
  }
}

function openscholar_vsite_modules_batch(&$install_state){
  //@todo this should be in an .inc file or something.
  $modules = array();
  $profile = drupal_get_profile();

  if(variable_get('os_profile_type', false) == 'vsite'){
    $data = file_get_contents("profiles/$profile/$profile.vsite.inc");
    $info = drupal_parse_info_format($data);
    if(is_array($info['dependencies'])){
      $modules = array_merge($modules,$info['dependencies']);
    }
  }

  if(variable_get('os_profile_flavor', false) == 'development'){
    $data = file_get_contents("profiles/$profile/$profile.development.inc");
    $info = drupal_parse_info_format($data);
    if(is_array($info['dependencies'])){
      $modules = array_merge($modules,$info['dependencies']);
    }

    if (variable_get('os_dummy_content', FALSE)) {
      $modules[] = 'os_migrate_demo';
    }
    $modules[] = 'harvard_activity_reports';
  }

  return _openscholar_module_batch($modules);
}

/**
 * Migrating content from csv.
 */
function openscholar_migrate_content() {
  $migrations = migrate_migrations();
  foreach ($migrations as $machine_name => $migration) {
    $operations[] = array('_openscholar_migrate_content', array($machine_name, t('Importing content.')));
  }

  $batch = array(
    'title' => t('Importing content'),
    'operations' => $operations,
  );

  return $batch;
}

/**
 * Batch callback function - migrating the content from csv files.
 */
function _openscholar_migrate_content($class, $type, &$context) {
  $context['message'] = t('Importing @class', array('@class' => $class));
  $migration =  Migration::getInstance($class);
  $migration->processImport();
}

/**
 * Returns a batch operation definition that will install some $modules
 *
 * @param $modules
 *   An array of names of modules to install
 *
 * $return
 *   A batch definition.
 *
 * @see
 *   http://api.drupal.org/api/drupal/includes%21install.core.inc/function/install_profile_modules/7
 */
function _openscholar_module_batch($modules) {
  $t = get_t();

  $files = system_rebuild_module_data();

  // Always install required modules first. Respect the dependencies between
  // the modules.
  $required = array();
  $non_required = array();

  // Add modules that other modules depend on.
  foreach ( $modules as $key => $module ) {
    if (isset($files[$module]) && $files[$module]->requires) {
      $modules = array_merge($modules, array_keys($files[$module]->requires));
    }
  }
  $modules = array_unique($modules);
  foreach ( $modules as $module ) {
    if (! empty($files[$module]->info['required'])) {
      $required[$module] = $files[$module]->sort;
    }
    else {
      $non_required[$module] = $files[$module]->sort;
    }
  }
  arsort($required);
  arsort($non_required);

  $operations = array();
  foreach ( $required + $non_required as $module => $weight ) {
    if (isset($files[$module])) {
      $operations[] = array('_install_module_batch',
        array(
          $module,
          $files[$module]->info['name']
        )
      );
    }
  }

  $additions = "";
  if(variable_get('os_profile_type', false) == 'vsite'){
    $additions .= "Multi-Tenant";
  }

  if(variable_get('os_profile_flavor', false) == 'development'){
    if(strlen($additions)){
      $additions .= " and ";
    }
    $additions .= "Development";
  }

  $batch = array(
    'operations' => $operations,
    'title' => st('Installing @needed modules.', array('@needed' => $additions)),
    'error_message' => st('The installation has encountered an error.'),
    'finished' => '_openscholar_install_profile_modules_finished'
  );
  return $batch;
}


/**
 * 'Finished' callback after all modules have been installed.
 */
function _openscholar_install_profile_modules_finished($success, $results, $operations) {
  _install_profile_modules_finished($success, $results, $operations);

  if (variable_get('file_default_scheme', 'public') == 'private'){
    //Disallow indexing for this install. (Uses already enabled robotstxt module)
    variable_set('robotstxt', "User-agent: *\nDisallow: /");
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 **/
function openscholar_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}

function openscholar_install_finished(&$install_state) {
  drupal_set_title(st('Openscholar installation complete'));
  $messages = drupal_get_messages();
  $output = '<p>' . st('Congratulations, you\'ve successfully installed Openscholar!') . '</p>';
  if (isset($messages['error'])) {
    $output .= '<p>' . st('Review the messages above before visiting <a href="@url">your new site</a> or <a href="@settings" class="overlay-exclude">change Openscholar settings</a>.', array('@url' => url(''), '@settings' => url('admin/config/openscholar', array('query' => array('destination' => ''))))) . '</p>';
  }
  else {
    $output .= '<p>'. st('<a href="@url">Visit your new site</a> or <a href="@settings" class="overlay-exclude">change Openscholar settings</a>.', array('@url' => url(''), '@settings' => url('admin/config/openscholar', array('query' => array('destination' => ''))))) . '</p>';
  }

  // Remember the profile which was used.
  variable_set('install_profile', drupal_get_profile());

  // Install profiles are always loaded last
  db_update('system')
    ->fields(array('weight' => 1000))
    ->condition('type', 'module')
    ->condition('name', drupal_get_profile())
    ->execute();

  // Cache a fully-built schema.
  drupal_get_schema(NULL, TRUE);

  // Remove the variable we used during the installation.
  variable_del('os_dummy_content');

  // Grant permission to view unpublished group content.
  os_grant_unpublished_viewing_permission();

  // Run cron to populate update status tables (if available) so that users
  // will be warned if they've installed an out of date Drupal version.
  // Will also trigger indexing of profile-supplied content or feeds.
  drupal_cron_run();

  return $output;
}
