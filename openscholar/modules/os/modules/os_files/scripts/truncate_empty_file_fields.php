<?php

/**
 * @file
 * Delete file reference to files which no longer exists.
 *
 * -- Arguments:
 *  - nid: The node ID you want to start from or you processed last.
 *  - batch: How much nodes to process every time, default is 250.
 */

// Get a list of fields that may contain files: files and image crop.
$file_fields = os_files_file_fields();

// Run through the nodes.
$nid = drush_get_option('nid', variable_get('os_files_last_nid', 0));
$batch = drush_get_option('batch', 250);
$query = new entityFieldQuery();
$result = $query
  ->entityCondition('entity_type', 'node')
  ->propertyCondition('nid', $nid, '>=')
  ->propertyOrderBy('nid')
  ->range(0, $batch)
  ->execute();

if (empty($result['node'])) {
  // All nodes were processed, delete the flag variable.
  variable_del('os_files_last_nid');
  return;
}

$nodes = node_load_multiple(array_keys($result['node']));
foreach ($nodes as $node) {
  $changed = FALSE;
  $deleted_files = 0;
  foreach ($file_fields as $file_field) {

    if (empty($node->{$file_field}[LANGUAGE_NONE])) {
      continue;
    }

    foreach ($node->{$file_field}[LANGUAGE_NONE] as $delta => $value) {
      $file_info = @file_load($value['fid']);

      if (empty($file_info)) {
        unset($node->{$file_field}[LANGUAGE_NONE][$delta]);
        $deleted_files++;
        $changed = TRUE;
      }
    }
  }

  // The node has changed, saving him.
  if ($changed) {
    node_save($node);
    drush_log(dt("Updating the node @title. @deleted_files were deleted.", array(
      '@title' => $node->title,
      '@deleted_files' => $deleted_files,
    )), 'success');
  }
}

// Saving the last NID we processed.
variable_set('os_files_last_nid', $node->nid);
drush_log(dt('Last processed node: @nid', array('@nid' => $node->nid)), 'warning');
