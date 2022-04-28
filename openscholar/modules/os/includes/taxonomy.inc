<?php

/**
 * Provides and CRUD functions needed for adding or removing vocabularies or terms from your openscholar site
 *
 * @file taxonomy.inc
 *
 * Provides taxonomy management and crud functions
 *
 * os_vocab_create
 *   Creates a new vocabulary
 *
 * os_vocab_remove
 *  Removes a vocab from an Openscholar site
 *
 * os_get_vocabularies
 *   Get the current avalible vocabularies
 *
 */

/**
 * Return the current vocabularies
 *
 * @return array|bool
 *   An array of all available vocabulary objects, indexed by vid, or FALSE.
 */
function os_get_vocabularies(){

  // Checks if this is a multi-tenant install
  if (module_exists('vsite_vocab')) {
    module_load_include('inc', 'vsite_vocab', 'includes/taxonomy');
    return vsite_vocab_get_vocabularies();
  }

  return taxonomy_get_vocabularies();
}

/**
 * Utility to get vocabulary entities related to entity type and bundle.
 *
 * @param $entity_type
 *   Entity type name.
 * @param $bundle
 *   Bundle name.
 *
 * @return mixed
 *   Array of vocabulary entities.
 */
function os_get_vocabularies_by_bundle($entity_type, $bundle) {
  if (!$vsite = vsite_get_vsite()) {
    return;
  }

  $query = db_select('og_vocab', 'ogv');
  $query->join('og_vocab_relation', 'ogr', 'ogr.vid = ogv.vid');
  $result = $query
    ->fields('ogv')
    ->condition('ogv.entity_type', $entity_type)
    ->condition('ogv.bundle', $bundle)
    ->condition('ogr.gid', $vsite->id)
    ->execute()
    ->fetchAllAssoc('vid');

  return taxonomy_vocabulary_load_multiple(array_keys($result));
}

/**
 * Creates a new drupal vocabulary
 *
 * @param $vocabulary
 *   A vocabulary object with the following properties:
 *   - name: The human-readable name of the vocabulary.
 *   - machine_name: The machine name of the vocabulary.
 *   - description: (optional) The vocabulary's description.
 *   - hierarchy: The hierarchy level of the vocabulary.
 *   - module: (optional) The module altering the vocabulary.
 *   - weight: (optional) The weight of this vocabulary in relation to other
 *     vocabularies.
 *   - original: (optional) The original vocabulary object before any changes
 *     are applied.
 *   - old_machine_name: (optional) The original machine name of the
 *     vocabulary.
 *
 * @return boolean
 *   Success
 */
function os_vocab_create($vocabulary){

  //Save the vocabulary object
  if(taxonomy_vocabulary_save($vocabulary) == SAVED_NEW){

    if(module_exists('vsite_vocab') && $vocabulary->vid){

      module_load_include('inc', 'vsite_vocab', 'includes/taxonomy');
      vsite_vocab_add_vocab($vocabulary->vid);
    }

    return true;
  }

  return false;
}

/**
 * Deletes drupal vocabulary
 * Note that there is nothing to do here for multi-tenant delted vocabs
 * automatically have their relationship removed
 *
 * @param $vid
 *   A vocabulary ID.
 *
 * @return boolean
 *   Success
 */
function os_vocab_remove($vid){

  //Delete the vocabulary
  return (taxonomy_vocabulary_delete($vid) == SAVED_DELETED);
}
