<?php

/*
 * Handles autocomplete for page-specific fields
 */
function os_pages_autocomplete() {
  include_once (drupal_get_path('module', 'vsite') . '/includes/nodeautocomplete.inc');
  $matches = array();
  $args = func_get_args();
  $string = array_pop($args);

  if (trim($string)) {
    ctools_include('node', 'os');
    $query = new OSNodeFieldQuery();
    $query->propertyCondition('title', $string, 'CONTAINS');
    $query->propertyCondition('type', 'page');
    if (!empty($args)) {
      $query->propertyCondition('nid', $args, 'NOT IN');
    }
    $result = $query->execute();
    if (isset($result['node'])) {
      $matches = _vsite_node_autocomplete_matches(array_keys($result['node']));
    }
  }

  drupal_json_output($matches);
  exit;
}