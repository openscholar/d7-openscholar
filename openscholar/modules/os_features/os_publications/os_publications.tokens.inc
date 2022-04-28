<?php

/**
 * @file
 * Token integration for the message example module.
 */

/**
 * Implements hook_token_info().
 */
function os_publications_token_info() {
  $info['tokens']['node']['citation'] = array(
    'name' => t('Biblio citation'),
    'description' => t('provide the citation of the publication.'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function os_publications_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type != 'node') {
    return;
  }

  foreach ($tokens as $name => $original) {
    if ($name != 'citation') {
      continue;
    }

    if ($data['node']->type != 'biblio') {
      continue;
    }

    $replacements[$original] = theme('biblio_style', array(
      'node' => $data['node'],
      'base' => variable_get('biblio_base', 'biblio'),
      'style_name' => biblio_get_style(),
    ));
  }

  return $replacements;
}
