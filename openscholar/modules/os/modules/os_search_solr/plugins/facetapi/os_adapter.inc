<?php

/**
 * @file
 * Contains OSApacheSolrFacetapiAdapter.
 */

class OsApacheSolrFacetapiAdapter extends ApacheSolrFacetapiAdapter {

  /**
   * Overrides ApacheSolrFacetapiAdapter::getFacetSettingsGlobal().
   */
  public function getFacetSettingsGlobal(array $facet) {
    $result = parent::getFacetSettingsGlobal($facet);

    if ($facet['name'] == 'sm_og_vocabulary') {
      // Set the operator by variable
      $operator = variable_get('os_search_og_vocabulary_operator', FACETAPI_OPERATOR_AND);
      $result->settings['operator'] = $operator;
    }

    return $result;
  }

}
