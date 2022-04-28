<?php

/**
 * @file
 * Contains the field handler for nodes referencing files.
 */

class cp_content_handler_field_referencing_nodes extends views_handler_field {

  /**
   * Iterate over all values once, and get all the referencing nodes.
   */
  function pre_render(&$values) {
    $fids = array();
    foreach ($values as $value) {
      $fids[] = $value->fid;
    }

    // There is no files - return early.
    if (empty($fids)) {
      return;
    }

    $result = db_select('file_usage', 'f')
      ->fields('f', array('type', 'fid', 'id', 'count'))
      ->condition('fid', $fids, 'IN')
      ->condition('type', 'node')
      ->condition('count', 0, '>')
      ->execute()
      ->fetchAll();

    $nids = array();
    foreach ($result as $row) {
      $nids[$row->id] = $row->id;
    }

    $nodes = node_load_multiple($nids);

    // Create an array keyed by the file ID, and the node titles as the value.
    $fids_data = array();
    foreach ($result as $row) {
      $nid = $row->id;

      // Don't show links to a VSite.
      $fids_data[$row->fid][] = !og_is_group('node', $nid) ? l($nodes[$nid]->title, 'node/' . $nid, array('attributes' => array('ng-non-bindable' => ''))) : "";
    }

    $this->fids_data = $fids_data;
  }

  function render($values) {
    // Return the node titles of the referencing.
    $fid = $values->fid;
    return !empty($this->fids_data[$fid]) ? implode(', ', array_filter($this->fids_data[$fid])) : '';
  }
}
