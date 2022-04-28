<?php

/**
 * @file
 * Content import task functions.
 */

/**
 * A generic task which attempts to import content from CSV.
 */
function vsite_preset_import_content_task($entity, $task, &$context) {
  $vsite = vsite_get_vsite($entity->nid, TRUE);
  if (!$vsite) {
    return FALSE;
  }
  // Save the current vsite.
  $current_vsite = vsite_get_vsite();
  // Temporary switch from the current vsite to created vsite.
  spaces_set_space($vsite);

  // Get default importable content types.
  $default_types = os_importer_importable_content();
  $types = [];

  $preset = spaces_preset_load($entity->spaces_preset_og, 'og');
  if (isset($preset->value['variable']['vsite_preset_import_content_types'])) {
    // Load the desired content types from preset's variable.
    $content_types = array_flip($preset->value['variable']['vsite_preset_import_content_types']);
    $types = array_intersect_key($default_types, $content_types);
  }
  if (empty($types)) {
    $types = $default_types;
  }

  $imported = vsite_preset_import_content($types, $preset);

  // Switch back the current vsite.
  spaces_set_space($current_vsite);

  return $imported;
}

/**
 * Attempts to import content of configured types from CSV files.
 *
 * @param array $types
 *   List of content types to import.
 * @param object $preset
 *   Preset object.
 *
 * @return bool $imported
 *   Whether or not the vsite's content was imported.
 */
function vsite_preset_import_content($types, $preset) {
  $imported = FALSE;
  $path = drupal_get_path('module', $preset->export_module);
  $content_path = $path . '/preset_assets/' . $preset->name . '/content/';
  foreach ($types as $type => $importer_info) {
    $file = $content_path . $type . '.csv';
    if (file_exists($file) && is_readable($file) && isset($importer_info['csv'])) {
      $source = feeds_source($importer_info['csv']);
      // Load the source fetcher config.
      $fetcher_config = $source->getConfigFor($source->importer->fetcher);
      // Add the new source file to the fetcher config.
      $fetcher_config['source'] = $file;
      $source->setConfigFor($source->importer->fetcher, $fetcher_config);
      // Explicitly set process in background.
      $config = ['process_in_background' => TRUE];
      $source->importer->addConfig($config);
      // Save the source.
      $source->save();
      // Execute the import.
      $source->startImport();
      $imported = TRUE;
    }
  }
  $pubs_path = $content_path.'/publications.bib';
  if (file_exists($pubs_path)) {
    $import_file = file_uri_to_object($pubs_path);
    $session_id = microtime();
    $filesize = sprintf("%01.1f", $import_file->filesize / 1000);
    $filesize = " ($filesize KB)";
    $batch_op = array(
      'title' => t('Importing @filename', array('@filename' => $import_file->filename . $filesize)),
      'operations' => array(
        array('biblio_import', array($import_file, 'biblio_bibtex', $GLOBALS['user']->uid, NULL, true, $session_id)),
        array('biblio_import_batch_operations', array($session_id, $GLOBALS['user'], $GLOBALS['user']->uid, null))
      ),
      'progressive' => TRUE,
      'finished' => 'biblio_import_batch_finished',
      'init_message' => t('Parsing file...'),
      'progress_message' => t('Saving nodes...'),
      'file' => './' . drupal_get_path('module', 'biblio') . '/includes/biblio.import.export.inc'
    );
    batch_set($batch_op);
  }
  return $imported;
}
