#!/user/bin/env drush
<?php
/**
 * Fixes publication sort titles that were broken by a faulty update
 */

// get the rows we need to manipulate
$q = db_select('biblio', 'b');
$q->join('field_data_title_field', 'tf', 'b.nid = tf.entity_id AND b.vid = tf.revision_id');
$or = db_or()
  ->condition('b.biblio_sort_title', 'p%p', 'LIKE')
  ->condition('b.biblio_sort_title', "");
$q = $q->fields('b', array('nid', 'biblio_sort_title'))
  ->fields('tf', array('title_field_value'))
  ->condition('tf.title_field_value', '<p%</p>', 'LIKE')
  ->condition($or)
  ->execute();


$transaction = db_transaction();
foreach ($q as $r) {
  module_load_include('inc', 'biblio', 'includes/biblio.util');
  try {
    $title = strip_tags($r->title_field_value);  // all we want is the text
    $title = biblio_normalize_title($title);
    db_update('biblio')
      ->fields(array(
        'biblio_sort_title' => $title
      ))
      ->condition('nid', $r->nid)
      ->execute();
  }
  catch (Exception $e) {
    $transaction->rollback();
    throw $e;
  }
}