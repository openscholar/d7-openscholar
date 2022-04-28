<?php

/**
 * Implements hook_cp_settings().
 */
function os_search_solr_cp_settings() {
  if (!module_exists('vsite') || !vsite_get_vsite()) {
    // Not inside a VSite.
    return;
  }

  if (!user_access('administer search')) {
    // No permission to administer search.
    return;
  }

  $settings = array();

  $settings['os_search_solr'] = array(
    'group' => array(
      '#title' => t('Cache / Reindex'),
      '#id' => 'os-maintenance',
    ),
    'form' => array(
      '#type' => 'submit',
      '#title' => t('Re-index this site'),
      '#value' => t('Submit'),
      '#suffix' => "<div class=\"description\">".t('(Re)Index the sites searchable content.')."</div>",
      '#submit' => array('os_search_solr_reindex_site'),
      '#weight' => 10
    ),
    'rest_trigger' => 'os_search_solr_reindex_site'
  );

  return $settings;
}