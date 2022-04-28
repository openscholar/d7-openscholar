<?php

/**
 * @file
 * Drush script for converting private people pictures to public pictures.
 *
 * Script parameters:
 *   --fids
 *     File IDs to move from private to public.
 */

/**
 * Returning the destined path for the file.
 *
 *  @param $uri
 *    The original URI of the file.
 *
 *  @return
 *    Destined path for the file.
 *
 *  Usage:
 *    $ cd docroot/profiles/openscholar
 *    $ drush scr modules/os/modules/os_files/scripts/deprivatize.php --fids=12345,12346,12347
 */
function _os_files_deprivatize_get_dest_path($uri) {
  $exploded_path = explode('/', $uri);
  $i = count($exploded_path) - 1;
  unset($exploded_path[$i]);

  return implode('/', $exploded_path);
}

/**
 * Attempts to move a file from private:// to public://.
 *
 * @return bool
 *  - Returns TRUE if any modifications were made.
 */
function _os_files_deprivatize($fid, $uri = NULL) {

  // Adds newline for readable terminal output.
  drush_log(' ', 'warning');

  // Checks for URI if not specified.
  if (!$uri) {
    $result = db_select('file_managed', 'f')
      ->fields('f', array('uri'))
      ->condition('fid', (int) $fid)
      ->execute()
      ->fetchAssoc();
    if (isset($result['uri'])) {
      drush_log("Success!!");
      $uri = $result['uri'];
      drush_log(dt('Found file @fid with URI @uri. Processing...', array('@fid' => $fid, '@uri' => $uri)), 'warning');
    }
    else {
      drush_log(dt('[?] Skipping @fid. File not found.', array('@fid' => $fid)), 'warning');
      return FALSE;
    }
  }

  // Only updates file if URI begins with "private://".
  if (strpos($uri, 'private://') !== 0) {
    drush_log(dt('[X] Skipping file @fid. Not a private file.', array('@fid' => $fid)), 'warning');
    return FALSE;
  }

  // Attempts to move file.
  drush_log(dt("Attempting to move file at @uri...", array('@uri' => $uri)), 'notice');
  return _os_files_deprivatize_file($fid);
}

/**
 * Moves a private managed file to the public directory.
 */
function _os_files_deprivatize_file($fid) {
  $file = file_load($fid);

  // Builds new public path.
  $dest_uri = str_replace('private://', 'public://', $file->uri);
  $dest_path = _os_files_deprivatize_get_dest_path($dest_uri);
  // Creates the destination folder if it doesn't exist.
  if (!is_dir($dest_path)) {
    // Creates the folder.
    drupal_mkdir($dest_path, NULL, TRUE);
  }

  $moved_file = @file_move($file, $dest_uri);
  if ($moved_file) {
    drush_log(dt('File @name moved successfully.', array('@name' => $file->filename)), 'success');
  }
  else {
    drush_log(dt('Error moving file @name.', array('@name' => $file->filename)), 'error');
    return FALSE;
  }

  $file->uri = $moved_file->uri;
  $file_moved = file_save($file);
  if (isset($file_moved->fid)) {
    drush_log(dt('[O] File @name updated successfully.', array('@name' => $file->filename)), 'success');
    return TRUE;
  }
  else {
    drush_log(dt('[!] Error updating file @name.', array('@name' => $file->filename)), 'error');
    return FALSE;
  }
}

/**
 * Main function.
 */
function _os_files_privatize_main($fids) {
  $flush_cache = FALSE;

  // Loops over all file IDs and tries to modify each.
  $fids = explode(',', $fids);
  drush_log(dt('Checking @count files...', array('@count' => count($fids))), 'warning');
  foreach ($fids as $fid) {
    $modified = _os_files_deprivatize($fid);
    if (!$flush_cache && $modified) {
      $flush_cache = TRUE;
    }
  }

  // Adds newline for readable terminal output.
  drush_log(' ', 'warning');

  // Flushes caches if any changes were made.
  if ($flush_cache) {
    drush_log(dt('Flushing the cache to update files field data...'), 'success');
    drupal_flush_all_caches();
    drush_log(dt('Done.'), 'success');
  }
  else {
    drush_log(dt('Skipping cache flush because no changes made.'), 'warning');
    drush_log(dt('Done.'), 'warning');
  }
}

// Runs main function.
$fids = drush_get_option('fids', 0);

_os_files_privatize_main($fids);