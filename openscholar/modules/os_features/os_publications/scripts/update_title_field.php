<?php
/**
 * @file
 * Triggering the process of populating the field title attached to the
 * content type publications.
 */

$nid = drush_get_option('nid') ? drush_get_option('nid') : 0;
$memory_limit = drush_get_option('memory_limit', 500);

$i = 0;
$batch = 250;

$query = new EntityFieldQuery();
$max = $query
  ->entityCondition('entity_type', 'node')
  ->entityCondition('bundle', 'biblio')
  ->propertyCondition('nid', $nid, '>=')
  ->count()
  ->execute();

while ($i < $max) {
  // Free up memory.
  drupal_static_reset();

  $query = new EntityFieldQuery();
  $results = $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'biblio')
    ->propertyCondition('nid', $nid, '>=')
    ->range(0, $batch)
    ->execute();

  if (empty($results['node'])) {
    return;
  }

  // We found items to process.
  $ids = array_keys($results['node']);
  title_field_replacement_init('node', 'biblio', 'title', $ids);
  $i += 250;
  $nid = end($ids);

  $params = array(
    '@start' => reset($ids),
    '@end' => end($ids),
    '@iterator' => $i,
    '@max' => $max,
  );
  drush_print(dt('Process node from @start to @end. Batch state: @iterator/@max', $params));


  if (round(memory_get_usage()/1048576) >= $memory_limit) {
    $params = array(
      '@memory' => round(memory_get_usage()/1048576),
      '@max_memory' => memory_get_usage(TRUE)/1048576,
    );

    drush_log(dt('Stopped before out of memory. Start process from the node ID @nid', array('@nid' => end($ids))), 'error');
    return;
  }
}
