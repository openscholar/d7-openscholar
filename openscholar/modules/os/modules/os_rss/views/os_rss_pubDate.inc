<?php

/**
 * Display the date field as the pubDate when viewing an RSS feed. When the date
 * field is not populated use the created node's property.
 */
class os_rss_pubDate extends views_handler_field_entity {

  public function render($values) {
    $wrapper = entity_metadata_wrapper('node', $values->nid);
    if ($wrapper->getBundle() == 'event') {
      $time = $wrapper->field_date->get(0)->value->value();
    }
    else {
      $time = empty($values->field_field_news_date[0]['raw']['value']) ? $values->node_created : $values->field_field_news_date[0]['raw']['value'];
    }

    return gmdate('r', $time);
  }
}
