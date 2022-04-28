<?php

/**
 * @file
 * Media module integration for the Media module.
 */

/**
 * Implements hook_media_browser_plugin_info().
 */
function media_media_browser_plugin_info() {
  $info['upload'] = array(
    'title' => t('Upload'),
    'weight' => -10,
    'class' => 'MediaBrowserUpload',
  );

  // Add a plugin for each View display using the 'media_browser' display type.
  $view_weight = 10;
  foreach (views_get_enabled_views() as $view) {
    foreach ($view->display as $display) {
      if ($display->display_plugin == 'media_browser') {
        $title = $display->display_title;
        if (!empty($display->display_options['title'])) {
          $title = $display->display_options['title'];
        }
        $info["{$view->name}--{$display->id}"] = array(
          'title' => $title,
          // @TODO make this configurable.
          'weight' => $view_weight++,
          'class' => 'MediaBrowserView',
          'view_name' => $view->name,
          'view_display_id' => $display->id,
        );
      }
    }
  }

  return $info;
}

/**
 * Implements hook_query_media_browser_alter().
 */
function media_query_media_browser_alter($query) {
  // Ensure that the query is against the file_managed table.
  $tables = $query->getTables();
  if (empty($tables['file_managed'])) {
    throw new Exception(t('Media browser being queried without the file_managed table.'));
  }
  $alias = $tables['file_managed']['alias'];

  module_load_include('inc', 'media', 'includes/media.browser');
  $params = media_get_browser_params();

  $types = !empty($params['types']) ? $params['types'] : NULL;
  $url_include_patterns = !empty($params['url_include_patterns']) ? $params['url_include_patterns'] : NULL;
  $url_exclude_patterns = !empty($params['url_exclude_patterns']) ? $params['url_exclude_patterns'] : NULL;
  $allowed_schemes = !empty($params['schemes']) ? array_filter($params['schemes']) : array();
  $extensions = !empty($params['file_extensions']) ? array_filter(explode(' ', $params['file_extensions'])) : array();

  $and_condition = db_and();

  if (!empty($types)) {
    // Type filtering.
    $local_condition = db_or();
    $local_condition->condition($alias . '.type', $types, 'IN');
    $and_condition->condition($local_condition);
  }
  if (!empty($allowed_schemes)) {
    // Include local files with the allowed extensions and types.
    $local_wrappers = array_intersect_key(media_get_local_stream_wrappers(), $allowed_schemes);
    $scheme_condition = db_or();
    if (!empty($local_wrappers)) {
      
      if (!empty($extensions)) {
        // Extension filtering.
        $local_condition = db_or();
        foreach (array_keys($local_wrappers) as $scheme) {
          foreach ($extensions as $extension) {
            $local_condition->condition($alias . '.uri', db_like($scheme . '://') . '%' . db_like('.' . $extension), 'LIKE');
          }
        }
        $scheme_condition->condition($local_condition);
      }
      else {
        // Any extension
        $local_condition = db_or();
        foreach (array_keys($local_wrappers) as $scheme) {
          $local_condition->condition($alias . '.uri', db_like($scheme . '://') . '%', 'LIKE');
        }
        $scheme_condition->condition($local_condition);
      }
    }

    // Include remote files with the allowed file types.
    // We cant filter extensions here, because remote file filenames usually
    // are a url or a parameter of a query.
    $remote_wrappers = array_intersect_key(media_get_remote_stream_wrappers(), $allowed_schemes);
    if (!empty($types) && !empty($remote_wrappers)) {
      $remote_condition = db_and();
      $wrapper_condition = db_or();
      foreach (array_keys($remote_wrappers) as $scheme) {
        $wrapper_condition->condition($alias . '.uri', db_like($scheme . '://') . '%', 'LIKE');
      }
      $remote_condition->condition($wrapper_condition);
      $scheme_condition->condition($remote_condition);
    }
    $and_condition->condition($scheme_condition);
  }
  else {
    if (!empty($extensions)) {
      $local_condition = db_or();
      foreach ($extensions as $extension) {
        $local_condition->condition($alias . '.uri', db_like('.' . $extension), 'LIKE');
      }
      $and_condition->condition($local_condition);
    }
  }

  if ($and_condition->count()) {
    $query->condition($and_condition);
  }

  if ($url_include_patterns) {
    $query->condition($alias . '.uri', '%' . db_like($url_include_patterns) . '%', 'LIKE');
    // Insert stream related restrictions here.
  }
  if ($url_exclude_patterns) {
    $query->condition($alias . '.uri', '%' . db_like($url_exclude_patterns) . '%', 'NOT LIKE');
  }

  if (!user_access('administer files')) {
    $query->condition($alias . '.uri', db_like('private://') . '%', 'NOT LIKE');
  }

  // @todo This is possibly redundant since it's already filtered in the view.
  $query->condition($alias . '.status', FILE_STATUS_PERMANENT);

  foreach (array_keys(media_get_hidden_stream_wrappers()) as $name) {
    $query->condition($alias . '.uri', db_like($name . '://') . '%', 'NOT LIKE');
  }
}
