<?php

/**
 * Provide the absolute url of the node.
 */
class os_rss_link extends views_handler_field_entity {

  public function render($values) {
    return url('node/' . $values->nid, array('absolute' => TRUE));
  }
}
