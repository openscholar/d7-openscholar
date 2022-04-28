<?php
// $Id$

/**
 * Return the pathauto settings modified to work inside vsites
 */
function _vsite_get_pathauto_settings() {
  $settings = array();

  // Prepends vsite purl to all group content node patterns.
  $bundles = array_keys(os_get_bundles(TRUE));
  $entity = 'node';
  foreach ($bundles as $bundle) {
    $pattern = pathauto_pattern_load_by_entity($entity, $bundle);
    $settings["pathauto_{$entity}_{$bundle}_pattern"] = "[vsite:site-purl]/" . $pattern;
  }
  // Prepends vsite purl to taxonomy term pattern.
  $pattern = pathauto_pattern_load_by_entity('taxonomy_term');
  $settings["pathauto_taxonomy_term_pattern"] = "[vsite:site-purl]/" . $pattern;

  // Prepends vsite purl to vocabulary pattern.
  $pattern = pathauto_pattern_load_by_entity('taxonomy_vocabulary');
  $settings["pathauto_taxonomy_vocabulary_pattern"] = "[vsite:site-purl]/" . $pattern;

  return $settings;
}
