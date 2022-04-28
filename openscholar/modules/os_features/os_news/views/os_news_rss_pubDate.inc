<?php

/**
 * Display the date field as the pubDate when viewing an RSS feed. When the date
 * field is not populated use the created node's property.
 */
class os_news_rss_pubDate extends views_handler_field_entity {

  public function render($values) {
    $time = empty($values->field_field_news_date[0]['raw']['value']) ? $values->node_created : $values->field_field_news_date[0]['raw']['value'];

    return gmdate('r', $time);
  }
}
