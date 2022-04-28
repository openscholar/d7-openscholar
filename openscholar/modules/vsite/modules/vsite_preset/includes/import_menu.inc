<?php

/**
 * @file
 * Menu items import task functions.
 */

/**
 * A generic task which attempts to import menu items from CSV.
 */
function vsite_preset_import_menu_task($entity, $task, &$context) {
  $vsite = vsite_get_vsite($entity->nid, TRUE);
  if (!$vsite) {
    return FALSE;
  }
  // Save the current vsite.
  $current_vsite = vsite_get_vsite();
  // Temporary switch from the current vsite to created vsite.
  spaces_set_space($vsite);

  $preset = spaces_preset_load($entity->spaces_preset_og, 'og');
  $imported = vsite_preset_import_menu($preset);

  // Switch back the current vsite.
  spaces_set_space($current_vsite);

  return $imported;
}

/**
 * Attempts to import menu items from CSV file.
 *
 * @param object $preset
 *   Preset object.
 *
 * @return bool $imported
 *   Whether or not the vsite's menu items were imported.
 */
function vsite_preset_import_menu($preset) {
  $imported = FALSE;
  $path = drupal_get_path('module', $preset->export_module);
  $file = $path . '/preset_assets/' . $preset->name . '/menu/menu.csv';
  if (file_exists($file) && is_readable($file)) {
    $source = feeds_source('os_menu');
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
  return $imported;
}
