<?php

/**
 * @file
 * OpenScholar's auto complete functions.
 */

/**
 * Returns all nodes in space, all nodes if outside of space.
 */
function os_node_autocomplete($str) {
  $matches = array();

  if (!empty($str)) {
    // Loading the OSNodeFieldQuery class.
    ctools_include('node', 'os');

    $query = new OSNodeFieldQuery();
    $result = $query
      ->propertyCondition('title', '%' . $str . '%', 'LIKE')
      ->execute();

    if (!empty($result['node'])) {
      // Load the nodes into array.
      $nodes = node_load_multiple(array_keys($result['node']));
      foreach ($nodes as $node) {
        $matches[$node->title . ' [nid:' . $node->nid . ']'] = check_plain($node->title);
      }
    }
  }

  // Print in JSON the nodes we found.
  drupal_json_output($matches);
}
