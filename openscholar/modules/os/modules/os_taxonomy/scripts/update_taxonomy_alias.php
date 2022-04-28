<?php

/**
 * @file
 * Assert all taxonomy term aliases are prefixed with PURL.
 *
 * Usage:
 *   drush scr update_taxonomy_alias.php
 */

if (!drupal_is_cli()) {
  // Prevent invoking script from browser.
  return;
}

$results = db_select('url_alias', 'u')
  ->fields('u')
  ->condition('source', '%taxonomy/term%', 'LIKE')
  ->execute()
  ->fetchAllAssoc('pid');

foreach ($results as $result) {
  $info = explode('/', $result->source);
  $purl = os_taxonomy_vsite_path(end($info));

  if (!$purl) {
    // The vsite the vocab relate to don't have a purl. Return.
    continue;
  }

  if (strpos($result->alias, $purl) !== FALSE) {
    // The term already have the purl at the start of the alias. Return.
    continue;
  }

  $alias = $purl . '/' . $result->alias;

  $new_path = (array) $result + array(
    'alias' => $alias,
  );

  $params = array(
    '@alias' => $result->alias,
    '@new-alias' => $alias,
  );

  // Update the alias and display a nice message.
  drush_log(dt('The alias @alias has been updated to @new-alias', $params), 'success');
  path_save($new_path);
}

/**
 * Get the path of the vsite which the vocab belong.
 *
 * @param $tid
 *  The term ID.
 *
 * @return
 *  The path of the vsite.
 */
function os_taxonomy_vsite_path($tid) {
  $term = taxonomy_term_load($tid);
  $purls = &drupal_static(__FUNCTION__, array());

  if (in_array($term->vid, $purls)) {
    // We already found purl for this vocab. Return the purl from the static
    // cache.
    return $purls[$term->vid];
  }

  $relation = og_vocab_relation_get($term->vid);
  $vsite = vsite_get_vsite($relation->gid);
  $purls[$term->vid] = $vsite->group->purl;

  return $vsite->group->purl;
}
