<?php

/**
 * @file
 * Script for removing vocabs created by feed importer when importing atom 1.0
 * RSS feed.
 *
 * Running the script:
 *  drush scr remove_vocab.inc
 */

$query = new EntityFieldQuery();
$result = $query
  ->entityCondition('entity_type', 'og_vocab')
  ->propertyCondition('bundle', 'feed_item')
  ->propertyCondition('entity_type', 'node')
  ->execute();

if (empty($result['og_vocab'])) {
  return;
}

$vocabs = entity_load('og_vocab', array_keys($result['og_vocab']));

foreach ($vocabs as $vocab) {
  // Deleting the relationship.
  $relation = og_vocab_relation_get($vocab->vid);
  db_delete('og_vocab_relation')
    ->condition('id', $relation->id)
    ->execute();

  $params['@vid'] = $vocab->vid;
  drush_log(dt('Deleting the relation for the vocab ID: @vid', $params), 'success');

  // Deleting the vocab.
  $vocab->delete();
  drush_log(dt('Deleting the vocab ID: @vid', $params), 'success');
}
