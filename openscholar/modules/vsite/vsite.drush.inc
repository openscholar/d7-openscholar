<?php

/**
 * @file
 * Drush functionality file for the vsite module.
 */


/**
 * Implements hook_drush_command().
 */
function vsite_drush_command() {
  $vrd_help = "Disable a vsite custom url(VRD - vsite redirect disable).\n";
  $vrd_help .= "After disabling the custom URLs you'll need to set the variable purl_base_domain to your local machine address. ";
  $vrd_help .= "You can use use the 'drush vset' command for that.";
  $items['vsite-redirect-disable'] = array(
    'description' => dt($vrd_help),
    'options' => array(
      'nid' => dt('The nid of the vsite.'),
      'no-purl-warning' => dt('Hide the confirmation about purl_base_domain'),
    ),
    'examples' => array(
      'drush vrd --nid=1' => dt('Delete the url for the vsite with the 1 identifier'),
      'drush vrd' => dt('Disable all of the custom URL.'),
    ),
    'aliases' => array('vrd'),
  );
  $items['vsite-redirect-list'] = array(
    'description' => dt('List custom vsite URLs.'),
    'options' => array(),
    'examples' => array('drush vrl'),
    'aliases' => array('vrl'),
  );
  $items['vsite-import-properly'] = array(
    'description' => dt('Import databases dumps properly'),
    'arguments' => array(
      'path' => dt('The path of the dump location'),
    ),
    'options' => array(
      'password' => dt('The password of the user 1. The default is admin.'),
    ),
    'examples' => array(
      'drush vip ~/Downloads/hwpi-25061989.sql' => dt('Importing specific file'),
    ),
    'aliases' => array('vip'),
  );
  $items['vsite-domain-prefix'] = array(
    'description' => dt('Prefix all custom domains and the purl_base_domain with a prefix'),
    'arguments' => array(
      'prefix' => dt('The prefix to add'),
    ),
    'examples' => array(
      'drush vdp dev' => dt('Prefixing with dev'),
    ),
    'aliases' => array('vdp'),
  );
  return $items;
}

/**
 * Return a list of custom vsite URLs
 */
function drush_vsite_redirect_list() {
  $query = db_select('purl')
    ->fields('purl', array('value', 'id'))
    ->condition('provider', 'vsite_domain');
  $query->join('node', 'n', 'n.nid = purl.id');
  $query->fields('n', array('title'));
  $results = $query->execute();

  foreach ($results->fetchAllAssoc('id') as $result) {
    drush_print(dt('@id: @domain -> @title', array(
      '@title' => $result->title,
      '@id' => $result->id,
      '@domain' => $result->value,
    )));
  }
}

/**
 * Command callback.
 */
function drush_vsite_redirect_disable() {
  $nid = drush_get_option('nid', 0);

  $spaces_query = db_delete('spaces_overrides')
    ->condition('object_type', 'variable')
    ->condition('object_id', 'vsite_domain_name');

  $purl_query = db_delete('purl')
    ->condition('provider', 'vsite_domain');

  if ($nid) {
    $spaces_query->condition('id', $nid);
    $purl_query->condition('id', $nid);
  }

  $spaces_query->execute();
  $purl_query->execute();

  if ($nid) {
    // Get the vsite path.
    $result = db_select('purl')
      ->fields('purl', array('value'))
      ->condition('provider', 'spaces_og')
      ->condition('id', $nid)
      ->execute()
      ->fetchAssoc();
  }

  if (!(drush_get_option('no-purl-warning', FALSE))) {
    $purl_base_domain = FALSE;

    if ($uri = drush_get_option('uri')) {
      $purl_base_domain = $uri;
    }
    // Asking the user if he would like to change the purl_base_domain variable.
    elseif (drush_confirm(dt('Would you like to change the purl base domain variable value?'))) {
      $purl_base_domain = drush_prompt(dt('Please enter the new purl base domain of your OS installation'));
      variable_set('purl_base_domain', $purl_base_domain);
      drush_log(dt("The variable 'purl_base_domain' has changed to @domain", array('@domain' => $purl_base_domain)), 'success');
    }

    if ($purl_base_domain) {
      variable_set('purl_base_domain', $purl_base_domain);
      drush_log(dt("The variable 'purl_base_domain' has changed to @domain", array('@domain' => $purl_base_domain)), 'success');
    }
  }

  // Clearing the cache for the change to take place.
  drupal_flush_all_caches();

  if ($nid) {
    $node = node_load($nid);
    $params = array(
      '@name' => $node->title,
      '@address' => variable_get('purl_base_domain') . '/' . $result['value'],
    );
    drush_log(dt('The custom url for @name has disabled. The address is @address', $params), 'success');
  }
  else {
    drush_log(dt('All of the custom URLs were deleted.'), 'success');
  }
}

/**
 * Import a database properly in a couple if steps:
 *
 * 1. Import the a dump from a file
 * 2. Run database upadate.
 * 3. Run the vrd command
 * 4. Change the password of the uid 1
 * 5. Download the acquia connect module
 * 6. Clear the cache
 * 7. Login the user as the user 1.
 */
function drush_vsite_import_properly($path) {
  // Import a dump.
  $path_info = explode("/", $path);
  drush_log(dt('Starting to import the file @file', array('@file' => end($path_info))), 'warning');
  drush_shell_exec("`drush sql-connect` < {$path}");
  drush_log(dt('The file @file imported successfully', array('@file' => end($path_info))), 'success');

  // Run update for the database if the user approved that.
  drush_invoke('updatedb');

  // Run the vrd command.
  drush_invoke('vsite-redirect-disable');

  // Change the password of the user 1.
  $user = user_load(1);
  user_save($user, array('pass' => 'admin'));

  $args = array(
    '@name' => $user->name,
    '@pass' => 'admin',
  );
  drush_log(dt('The user @name password has changed to @pass', $args), 'success');

  // Download the acquia_connector module, as it is not part of the make file.
  drush_invoke_process('@self', 'pm-download', array('acquia_connector‏'));
  module_enable(array('acquia_search'));

  // Checking files.
  $public_path = variable_get('file_public_path');
  $temp_path = variable_get('file_temporary_path');

  // Handle the public/private folders.
  _drush_vsite_import_properly($public_path, "sites/default/files", 'file_public_path');
  _drush_vsite_import_properly($temp_path, "/tmp", 'file_temporary_path');

  // Clear the cache.
  drush_invoke_process('@self', 'cache-clear', array('all'));

  // Open the browser for logging in.
  drush_user_login();
}

/**
 * Handle the folders of the OS installation.
 *
 * @param $path
 *  The current path of the temp/private folder.
 * @param $default
 *  The default value of the temp/private folder.
 * @param $variable_name
 *  The variable name which holds the path of the temp/private folder.
 */
function _drush_vsite_import_properly($path, $default, $variable_name) {
  if (!file_exists($path)) {
    $path = $default;
    drush_shell_exec("mkdir {$path} -p");
    variable_set($variable_name, $path);
  }

  if (!is_writeable($path)) {
    drush_shell_exec("chmod -R 777 {$path}");
  }
}

/**
 * Command callback.
 */
function drush_vsite_domain_prefix($prefix) {

  $spaces_query = db_select('spaces_overrides')
    ->fields('spaces_overrides')
    ->condition('object_type', 'variable')
    ->condition('object_id', 'vsite_domain_name');


  $purl_query = db_update('purl')
    ->expression('value', "CONCAT(:prefix,'.',value)", array(':prefix' => $prefix))
    ->condition('provider', 'vsite_domain');

  $results = $spaces_query->execute();
  $purl_query->execute();

  while($record = $results->fetchAssoc()) {

    $domain = unserialize($record['value']);

    if (empty($domain)) {
      continue;
    }

    $domain = $prefix . "." . $domain;

    db_update('spaces_overrides')
      ->fields(array(
        'value' => serialize($domain)
      ))
      ->condition('object_type', 'variable')
      ->condition('id', $record['id'])
      ->condition('object_id', 'vsite_domain_name')
      ->execute();
  }

  $purl_base_domain = variable_get('purl_base_domain');
  variable_set('purl_base_domain', $prefix . "." . $purl_base_domain);

  // Clearing the cache for the change to take place.
  drupal_flush_all_caches();

  $params = array(
    '@prefix' => $prefix,
  );
  drush_log(dt('The domains have been prefixed with @prefix', $params), 'success');
}
