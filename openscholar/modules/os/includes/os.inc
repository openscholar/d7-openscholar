<?php
// $Id$

/**
 * @file os.inc
 * Provides generic OS management and feature functions. This file is included
 * by default.
 */

/**
 * Define disabled app.
 */
define('OS_DISABLED_APP', 0);

/**
 * Define a public app.
 *
 * An app which her content and pages are public to all.
 */
define('OS_PUBLIC_APP', 1);

/**
 * Define a private app.
 *
 * An app which her content and pages are private.
 */
define('OS_PRIVATE_APP', 2);

/**
 * Returns available bundles, with option to filter by spaces_features status.
 *
 * @param $include_status
 *  Can be array:
 *    - 0: disabled
 *    - 2: private
 *    - 1: public
 *  TRUE returns all
 *
 * @return
 *  Array keyed with the bundle ID and the bundle name as value.
 */
function os_get_bundles($include_status = array(OS_PUBLIC_APP)) {
  $return_all = FALSE;
  if ($include_status === TRUE) {
    $return_all = TRUE;
  }
  elseif (!is_array($include_status)) {
    $include_status = array($include_status);
  }

  $bundles = array();
  $app_info = os_app_info();
  $spaces_features = variable_get('spaces_features', array());

  foreach ($app_info as $app_name => $info) {
    if (!isset($info['nodetypes'])) {
      continue;
    }
    $module = $info['module'];
    if (!$return_all && (!isset($spaces_features[$module]) || !in_array($spaces_features[$module], $include_status))) {
      continue;
    }

    $_node_types = _node_types_build();
    foreach ($info['nodetypes'] as $nodetype) {
      if (isset($_node_types->names[$nodetype])) {
        $bundles[$nodetype] = $_node_types->names[$nodetype];
      }
      else {
        $bundles[$nodetype] = $module;
      }
    }
  }

  // Sort the bundles in alphabetical order.
  asort($bundles);

  // Allow other modules to alter the os bundle list.
  drupal_alter('os_get_bundles', $bundles);

  return $bundles;
}

/**
 * Returns available bundles per OS app, with option to filter by status.
 *
 * @param $include_status
 *  spaces_features status. Can be array:
 *    - 0: disabled
 *    - 2: private
 *    - 1: public
 *  TRUE returns all
 *
 * @return
 *  Array keyed with the application name, with an array of bundle IDs and
 *  their bundle name as values.
 */
function os_get_bundles_per_app($include_status = array(OS_PUBLIC_APP)) {
  $return_all = FALSE;
  if ($include_status === TRUE) {
    $return_all = TRUE;
  }
  elseif (!is_array($include_status)) {
    $include_status = array($include_status);
  }

  $bundles_per_app = array();
  $app_info = os_app_info();
  $spaces_features = variable_get('spaces_features', array());

  foreach ($app_info as $app_name => $info) {
    if (!isset($info['nodetypes'])) {
      continue;
    }
    $module = $info['module'];
    if (!$return_all && (!isset($spaces_features[$module]) || !in_array($spaces_features[$module], $include_status))) {
      continue;
    }

    $_node_types = _node_types_build();
    foreach ($info['nodetypes'] as $nodetype) {
      if (empty($bundles_per_app[$app_name])) {
        $bundles_per_app[$app_name] = array();
      }
      if (isset($_node_types->names[$nodetype])) {
        $bundles_per_app[$app_name][$nodetype] = $_node_types->names[$nodetype];
      }
      else {
        $bundles_per_app[$app_name][$nodetype] = $module;
      }
    }
  }

  // Sort the bundles in alphabetical order.
  asort($bundles_per_app);

  return $bundles_per_app;
}

/**
 * Returns the current active feature based on active contexts.
 *
 * @return object
 * The active feature.
 */
function os_get_active_feature() {
  static $active_feature;

  if (!isset($active_feature)) {
    $features = spaces_features('og');
    $contexts = context_active_contexts();
    if (!empty($contexts) && is_array($contexts)) {
      foreach ($features as $feature) {
        $info = $feature->info['features'];
        if (isset($info['context'])) {
          foreach ($info['context'] as $ctx) {
            if (array_key_exists($ctx, $contexts)) {
              $active_feature = $feature;
              break (2);
            }
          }
        }
      }
    }
  }

  return $active_feature;
}

/**
 * @function os_get_app_path
 *
 * Gets the base context path for a feature.  This is the path that will match
 * to this features context.
 *
 * @param string $app_name (optional)
 *   The machine name of the app (a feature module).  If left blank, all app paths will be returned.
 *   Providing $app_name will return that app's path, or FALSE if none.
 *
 * @return string $path
 */
function os_get_app_path($app_name = NULL) {
  $app_info = ($app_name) ? os_app_info($app_name) : os_app_info();

  if (isset($app_info['path'])) {
    return (isset($app_info['path']) && strlen($app_info['path'])) ? $app_info['path'] : FALSE;
  }

  //return multiple type => path
  $apps = array();
  foreach ($app_info as $app) {
    if (isset($app['nodetypes']) && $app['nodetypes'] && isset($app['path']) && $app['path']) {
      $apps += array_fill_keys($app['nodetypes'], $app['path']);
    }
  }

  return ($apps) ? $apps : FALSE;
}

/**
 * Creates and saves (inserts) a new node.
 *
 * @see os_create_node()
 */
function _os_create_node($options) {
  // Does nothing if any of the required options are missing.
  if (!isset($options['title'], $options['type'], $options['uid'])) {
    return FALSE;
  }
  $defaults = array(
    'path' => array(
      'pathauto' => FALSE,
    ),
  );
  $options += $defaults;

  // Creates a new node entity with the specified author user ID.
  $node = entity_create('node', array('type' => $options['type']));
  $node->uid = $options['uid'];
  $node->path = $options['path'];

  // Updates parent and child node for creating subpage (book) nodes.
  if (isset($options['parent_node'])) {
    os_pages_make_subpage($node, $options['parent_node']);
  }

  // Creates a wrapper and further modifies the entity.
  $wrapper = entity_metadata_wrapper('node', $node);

  // Handles additional options such as body value.
  $properties = array_keys($wrapper->getPropertyInfo());
  $skip = array('type');
  foreach ($options as $key => $value) {
    if (in_array($key, $properties) && !in_array($key, $skip)) {
      $wrapper->{$key}->set($value);
    }
  }
  // @todo add a function in api.php describing the correct $options array
  // to modify most common values (i.e. language, comment, comment_source,
  // pathauto, (published) status, created date, sticky, promoted, etc...

  // Saves the node.
  $wrapper->save();

  // Returns the new node object.
  return $node;
}

/**
 * Creates and saves (inserts) a new node. Wrapper for _os_create_node().
 *
 * @param string $title
 *   The new node title.
 * @param string $type
 *   The new node's type (machine name).
 * @param int $uid
 *   The author's uid.
 * @param string $body
 *   (Optional) A translated text value for the body field. Use t() before call.
 * @param array $options
 *   (Optional) Override defaults, or add additional properties or values.
 */
function os_create_node($title, $type, $uid, $body = NULL, $options = array()) {
  // Inserts the required properties into the node options array.
  $options['title'] = $title;
  $options['type'] = $type;
  $options['uid'] = $uid;
  if ($body !== NULL) {
    $options['body'] = array('value' => $body);
  }

  // Creates and loads the new node.
  $node = _os_create_node($options);

  // Returns the new node.
  return $node;
}

