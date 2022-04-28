<?php

class OsSolrBaseQuery extends SolrBaseQuery {

  protected function defaultSorts() {
    $default = parent::defaultSorts();
    unset($default['sort_name']);
    return $default;
  }

}
