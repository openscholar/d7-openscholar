<?php

/**
 * Provides and CRUD functions for vsite/taxonomy
*
* @file taxonomy.inc
*
* Provides vsite management and crud functions for taxonomy
*
* vsite_vocab_add_vocab
*   Generic function to add a vocabulary to a vsite
*
*
*/
function vsite_vocab_get_vocabularies($vsite = false) {
  // Get the active space
  if (! $vsite) {
    $vsite = spaces_get_space();
  }

  if ($vsite) {
    $query = db_select('og_vocab_relation', 'ogr')
      ->fields('ogr')
      ->condition('group_type', $vsite->group_type)
      ->condition('gid', $vsite->id);
    $query->leftJoin('taxonomy_vocabulary', 'tv', 'ogr.vid = tv.vid');
    $query->orderBy('tv.weight');
    $query->orderBy('tv.name');
    $vocabs = $query->execute()
      ->fetchAllAssoc('vid');

    if (count($vocabs)) {
      return taxonomy_vocabulary_load_multiple(array_keys($vocabs));
    }
  }

  // No available vocabs
  return array();
}

/**
 * Add the passed vocab to a vsite
 *
 * The vid of the vocab to add
 *
 * @param int $vid
 *          The vsite object to add the entity to
 * @param space $vsite
 *
 */
function vsite_vocab_add_vocab($vid, $vsite = false){

  // Get the active space
  if (! $vsite) {
    $vsite = spaces_get_space();
  }

  if ($vid && $vsite) {
    // Create the vocab / viste relationship
    return og_vocab_relation_save($vid, $vsite->group_type, $vsite->id);
  }

  return false;
}

/**
 * Remove the passed vocab from it's group.
 * This will not delete
 * the vocabulary
 *
 * The vid of the vocab to remove
 *
 * @param int $vid
 *
 */
function vsite_vocab_remove_vocab($vid){

  // Remove the group relationship from the vocabulary
  return og_vocab_realtion_delete('taxonomy_vocabulary', $entity->vid);
}
