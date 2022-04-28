<?php

/**
 * @file
 */

class OsSearchSolrOtherSitesLinks extends FacetapiWidgetLinks {

  function buildListItems($build) {
    if (($vsite = vsite_get_vsite()) && isset($build[$vsite->group_type . ':' . $vsite->id])) {
      unset($build[$vsite->group_type . ':' . $vsite->id]);
    }
    return parent::buildListItems($build);
  }
}
