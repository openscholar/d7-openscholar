<?php
// $Id$


/**
 * @file
 * Callbacks and helpers for vsite (current space) sensitive node autocomplete.
 *
 * The menu item at 'vsite/autocomplete/node'
 *
 * @see cp_menu.wizard.inc
 * @see os_boxes_manual_list.inc
 */

/**
 * Builds and returns an autocomplete node field form elemtent.
 *
 * @param array $build
 *   Optional element to override element attributes. Note that you should not
 *   override the '#autocomplete_path' or '#element_validate' attributes.
 *
 * @return array $element
 *   The fully prepared autocomplete textfield element.
 */
function vsite_node_autocomplete_element($build = array()) {
  $element = $build + array(
    '#type' => 'textfield',
    '#title' =>  t('Select a post'),
    '#maxlength' => 180,
    '#autocomplete_path' => 'vsite/autocomplete/node',
    '#element_validate' => array('vsite_node_autocomplete_validate'),
    '#description' => t('Start typing the title of an existing post'),
  );

  return $element;
}

/**
 * Page callback; autocomplete that takes vsite (current space) into account.
 */
function vsite_node_autocomplete_callback($string = '') {
  $matches = array();

  if (trim($string)) {
    ctools_include('node', 'os');
    $query = new OSNodeFieldQuery();
    $query->propertyCondition('title', $string, 'CONTAINS');
    $result = $query->execute();
    if (isset($result['node'])) {
      $matches = _vsite_node_autocomplete_matches(array_keys($result['node']));
    }
  }

  drupal_json_output($matches);
}

/**
 * Validation callback; sets the nid value, or throws an error if none found.
 */
function vsite_node_autocomplete_validate($element, &$form_state) {
  $element_value = $element['#value'];
  if (!$element_value) {
    return;
  }
  $nid = _vsite_node_autocomplete_title_to_nid($element_value);

  if ($nid !== FALSE && $nid !== 0) {
    $form_state['vsite_autocomplete_nid'] = $nid;
  }
  else {
    form_error($element, t('The page does not exist.'));
  }
}

/**
 * Takes a "Title [123]" string and returns the int value, or NULL if none.
 *
 * @see vsite_node_autocomplete_validate()
 */
function _vsite_node_autocomplete_title_to_nid($title) {
  preg_match('/^(?:\s*|(.*) )?\[\s*(\d+)\s*\]$/', $title, $matches);
  if (!empty($matches) && isset($matches[2])) {
    return $matches[2];
  }
  if (intval($title)) {
    return intval($title);
  }

  return FALSE;
}

/**
 * Prepares a matches array for drupal autocomplete
 *
 * @param array $nids
 *   An un-indexed array of node ids.
 *
 * @return array $matches
 *   An indexed array containing elements like 'Title [123]' => 'Title'.
 */
function _vsite_node_autocomplete_matches($nids) {
  $matches = array();

  $exclude = array();
  foreach (os_app_info() as $info) {
    if (!empty($info['searchable'])) {
      // Option 'searchable' defaults to TRUE.
      continue;
    }

    // Add the unsearchable node types to the excluded list.
    $exclude = array_merge($exclude, $info['nodetypes']);
  }

  $query = db_select('node')
    ->fields('node', array('nid', 'title'))
    ->condition('nid', $nids, 'IN');

  if (!empty($exclude)) {
    $query->condition('type', $exclude, 'NOT IN');
  }

  $result = $query->execute();

  foreach ($result as $node) {
    $matches[$node->title . " [$node->nid]"] = check_plain($node->title);
  }

  return $matches;
}
