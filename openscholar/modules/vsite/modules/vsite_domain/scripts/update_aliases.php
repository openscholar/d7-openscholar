<?php
/**
 * @file update_aliases.php
 *
 * Drush script for removing the prefixed / from the courses aliases.
 *
 * @example:
 *
 *   drush scr update_aliases.php
 */

if (!drupal_is_cli()) {
  // The file is not reachable via web browser.
  return;
}

// Iterate over the elements.
$results = db_select('url_alias', 'u')
  ->fields('u', array('pid'))
  ->condition('alias', '/courses%', 'LIKE')
  ->execute()
  ->fetchAllAssoc('pid');

if (!$results) {
  return;
}

foreach (array_keys($results) as $pid) {
  $path = path_load(array('pid' => $pid));
  $params['@url'] = $path['alias'];
  $path['alias'] = str_replace('/courses', 'courses', $path['alias']);
  path_save($path);
  $sandbox['pid'] = $params['@pid'] = $path['pid'];
  drush_log(dt('Updating @url(@pid)', $params), 'success');
}
