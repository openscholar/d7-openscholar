<?php

/**
 * @file
 *
 * Holds all the helper functions for the OS importer module.
 */

/**
 * Parsing the header.
 */
function _os_importer_parse_header(ParserCSV $parser, ParserCSVIterator $iterator, $mappings) {
  $parser->setLineLimit(1);
  $rows = $parser->parse($iterator);
  if (!count($rows)) {
    return array();
  }
  $header = array_shift($rows);
  foreach ($header as $i => $title) {
    $header[$i] = trim(drupal_strtolower($title));
  }
  return $header;
}

/**
 * Helper function to import a node on a behalf of another user.
 */
function _os_importer_import_behalf_of_user(FeedsSource $source, $entity, $item, $id) {
  global $user;

  // Assigning Organic group ID for created node.
  if (!empty($id)) {
    $entity->og_group_ref[LANGUAGE_NONE][0]['target_id'] = $id;
  }
  // When importing an iCal don't search for the author, just use the current
  // user UID.
  if ($source->importer->id == 'os_ical') {
    $entity->uid = $user->uid;
    return;
  }

  if (empty($item['author'])) {
    $account = user_load($user->uid);
  }
  else {
    if ($account = user_load_by_name($item['author'])) {
      // Check that the user is a group member and verify he can create node of
      // this type.
      $account = og_is_member('node', $id, 'user', $account) && node_access('create', $entity->type, $account) ? $account : user_load($user->uid);
    }
    else {
      // Loading the current user.
      $account = user_load($user->uid);
    }
  }

  $entity->uid = $account->uid;
}

/**
 * Helper function to import vocabularies and taxonomy terms on the fly.
 */
function _os_importer_import_vocab(FeedsSource $source, $entity, $item, $id) {
  $config = $source->importer->getConfig();
  $pattern = '/[^a-zA-Z0-9_]/';

  // Getting the mappings from the processor config.
  $mappings = array();
  foreach ($config['processor']['config']['mappings'] as $mapping) {
    $mappings[] = strtolower($mapping['source']);
  }

  // Now when we know what the original mapping was we can get the names of the
  // new vocabularies.
  $new_vocabs = array_diff(array_keys($item), $mappings);
  $tids = array();
  foreach ($new_vocabs as $vocab) {
    $original_name = $vocab;
    // Create the terms and the vocabulary.
    if (preg_match('@[^a-zA-Z0-9_ ]+@', $vocab)) {
      $params['@name'] = $vocab;
      $vocab = preg_replace($pattern, '', $vocab);
      $params['@new-name'] = $vocab;
      $params['@taxonomy-page'] = url('cp/build/build/taxonomy');
      $text = "The vocabulary name \"@name\" contains disallowed character(s), it has been changed to \"@new-name\".";
      $text .= "All special characters and spaces are removed from the original name during importing.";
      $text .= "You can change the vocabulary name in <a href='@taxonomy-page'>cp/build/build/taxonomy</a> in the future.";
      drupal_set_message(t($text, $params), 'notice');
    }
    $info = _os_importer_create_vocab($vocab, $item[$original_name], 'node', $id, 'node', $entity->type);

    // Build the taxonomy terms for the new node.
    $tids = array_merge($info['tids'], $tids);
  }

  if (!$tids) {
    return;
  }

  $wrapper = entity_metadata_wrapper('node', $entity);
  $wrapper->{OG_VOCAB_FIELD}->set($tids);
}

/**
 * Create OG vocab on the fly.
 *
 * @param $name
 *  The new vocabulary title.
 * @param $terms
 *  The taxonomy terms of the new vocabulary.
 * @param $group_type
 *  The group type
 * @param $gid
 *  The group ID.
 * @param $entity_type
 *  The entity type the OG vocab is attached to.
 * @param $bundle
 *  The bundle of the entity.
 *
 * @return array
 *  Array with the vocabulary ID and the his term IDs.
 */
function _os_importer_create_vocab($name, $terms, $group_type, $gid, $entity_type, $bundle) {
  $query = db_select('og_vocab_relation', 'ogr');
  $query->join('taxonomy_vocabulary', 'v', 'v.vid = ogr.vid');

  // We need to check if a vocabulary with that name exists in the group.
  $result = $query
    ->fields('v')
    ->condition('group_type', $group_type)
    ->condition('gid', $gid)
    ->condition('v.name', $name)
    ->execute()
    ->fetchAllAssoc('vid');

  if ($result) {
    // The vocabulary is already exists, we can use him.
    $vocabulary = reset($result);
  }
  else {
    // We didn't found any vocabulary - create a new one.
    $i = 0;
    $machine_name = str_replace(array(' ', '-', ','), array('_', '_', ''), strtolower($name));
    while (taxonomy_vocabulary_machine_name_load($machine_name)) {
      $machine_name = substr($machine_name, 0, 32 - strlen($i)) . $i;
      $i++;
    }

    // Creating the new vocabulary.
    $vocabulary = new stdClass();
    $vocabulary->name = $name;
    $vocabulary->machine_name = $machine_name;
    taxonomy_vocabulary_save($vocabulary);
  }

  $tids = array();

  // Creating the terms for the new vocabulary if terms are provided.
  $taxonomy_terms = explode("|", $terms);
  foreach ($taxonomy_terms as $taxonomy_term) {
    if (empty($taxonomy_term)) {
      continue;
    }
    $term = taxonomy_get_term_by_name($taxonomy_term, $vocabulary->machine_name);

    if (!$term) {
      $term = new stdClass();
      $term->name = $taxonomy_term;
      $term->vid = $vocabulary->vid;
      taxonomy_term_save($term);
      $tids[] = $term->tid;
    }
    else {
      $tids[] = reset($term)->tid;
    }
  }

  // Create the OG vocabulary and the relation to the group.
  if (!og_vocab_load_og_vocab($vocabulary->vid, $entity_type, $bundle)) {
    og_vocab_create_og_vocab($vocabulary->vid, $entity_type, $bundle)->save();
    og_vocab_relation_save($vocabulary->vid, $group_type, $gid);
  }

  return array(
    'vid' => $vocabulary->vid,
    'tids' => $tids,
  );
}
