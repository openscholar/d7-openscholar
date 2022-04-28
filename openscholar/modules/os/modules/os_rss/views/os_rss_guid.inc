<?php

/**
 * Provide the guid of the entity.
 */
class os_rss_guid extends views_handler_field_entity {

  public function render($values) {
    global $base_url;
    return $values->nid . ' at ' . $base_url;
  }
}
