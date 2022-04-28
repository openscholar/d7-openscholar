<?php

/**
 * @file
 * Drush script for converting private people pictures to public pictures.
 *
 *  Script parameters:
 *    --nid - List of people nodes IDs separated via Comma: 1,2
 *    --vsites - List of vsites IDs separated via Comma: 1,2
 */

$nids = drush_get_option('nids', 0);
$vsites = drush_get_option('vsites', 0);

$query = new entityFieldQuery();
$query
  ->entityCondition('entity_type', 'node')
  ->propertyCondition('type', 'person');

// Adding node IDs to the query.
if ($nids != 0) {
  $nids = explode(",", $nids);
  if (!empty($nids)) {
    $query->propertyCondition('nid', $nids, 'IN');
  }
}

// Adding sites to the query.
if ($vsites != 0) {
  $vsites = explode(",", $vsites);
  if (!empty($vsites)) {
    $query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $vsites, 'IN');
  }
}

$result = $query->execute();

if (empty($result['node'])) {
  drush_log(dt('No records were found'), 'error');
  return;
}
$nodes = node_load_multiple(array_keys($result['node']));

// When updating the file field there is a need for flushing the cache. The
// variable will notify us later if needed.
$flush_cache = FALSE;
foreach ($nodes as $node) {
  $wrapper = entity_metadata_wrapper('node', $node);

  foreach ($wrapper->field_upload->value() as $value) {
    // Only private files will be handled during the script.
    if (strpos($value['uri'], 'public://') === 0) {
      drush_log(dt('File @name is already public - skipping.', array('@name' => $value['filename'])), 'warning');
      continue;
    }

    $file = file_load($value['fid']);

    // Building the new path of the file.
    $destination = str_replace('private://', 'public://', $file->uri);

    // Create the destined folder if it's not exists.
    if (!is_dir(get_destination_dir($destination))) {
      // Creating the folder.
      drupal_mkdir($destination_folder, NULL, TRUE);
    }

    $moved_file = @file_move($file, $destination);
    if ($moved_file) {
      drush_log(dt('File @name moved successfully.', array('@name' => $file->filename)), 'success');
    }

    $file->uri = $moved_file->uri;
    file_save($file);

    drush_log(dt('File @name updated successfully.', array('@name' => $file->filename)), 'success');
    $flush_cache = TRUE;
  }

  // Handling with the person photo.
  if (empty($node->field_person_photo[LANGUAGE_NONE][0])) {
    continue;
  }

  $personal_photo = $node->field_person_photo[LANGUAGE_NONE][0];
  if (strpos($personal_photo['uri'], 'private://') !== 0) {
    drush_log(dt('The person photo(@name) is already public - skipping.', array('@name' => $value['filename'])), 'warning');
    continue;
  }

  $file = file_load($personal_photo['fid']);

  // Building the new path of the file.
  $destination = str_replace('private://', 'public://', $file->uri);

  // Create the destined folder if it's not exists.
  if (!is_dir(get_destination_dir($destination))) {
    // Creating the folder.
    drupal_mkdir($destination_folder, NULL, TRUE);
  }

  $moved_file = @file_move($file, $destination);
  if ($moved_file) {
    drush_log(dt('File @name moved successfully.', array('@name' => $file->filename)), 'success');
  }

  $file->uri = $moved_file->uri;
  file_save($file);

  drush_log(dt('File @name updated successfully.', array('@name' => $file->filename)), 'success');
  $flush_cache = TRUE;
}

// Flushing caches for rebuild filed data.
if ($flush_cache) {
  drush_log(dt('Flushing the cache for update the files field data.'), 'warning');
  drupal_flush_all_caches();
}

/**
 * Returning the destined path for the file.
 *
 *  @param $uri
 *    The original URI of the file.
 *
 *  @return
 *    Destined path for the file.
 */
function get_destination_dir($uri) {
  $exploded_path = explode('/', $uri);
  $i = count($exploded_path) - 1;
  unset($exploded_path[$i]);

  return implode('/', $exploded_path);
}
