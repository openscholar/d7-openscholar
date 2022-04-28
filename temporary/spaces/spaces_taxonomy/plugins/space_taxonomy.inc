<?php

/**
 * Taxonomy integration for Spaces.
 */
class space_taxonomy extends space_type_purl {
  var $term = NULL;

  /**
   * Override of title().
   */
  function title() {
    return $this->term->name;
  }

  /**
   * Override of load().
   */
  function load() {
    // The 0 id means a new term is being saved. Instantiate a space
    // so preset values can become active.
    if ($this->id === 0) {
      return TRUE;
    }
    else if ($this->term = taxonomy_term_load($this->id)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Override of router().
   */
  function router($op, $object = NULL) {
    switch ($op) {
      case 'init':
        // NOTE: in D6 this used to handle multiple terms via taxonomy_terms_parse_string
        // but in D7 that has gone away for taxonomy term pages
        if (implode('/', array(arg(0), arg(1))) === 'taxonomy/term' && arg(2)) {
          $tid = arg(2);
          if ($term = taxonomy_term_load($tid)) {
            $this->router_term($term);
          }
        }
        break;
      case 'node':
        if ($term = spaces_taxonomy_get_term($object)) {
          $this->router_term($term);
        }
        break;
    }
    return;
  }

  /**
   * Router helper with logic for individual taxonomy terms.
   */
  protected function router_term($term) {
    if ($term->vocabulary_machine_name == variable_get('spaces_taxonomy_machine_name', 0)) {
      $space = spaces_load('taxonomy', $term->tid);
      // If there is no active taxonomy space or the spaces do not match, activate this term's space.
      if ($space && (!$this->active || ($this->id != $space->id))) {
        $space->activate();
      }
    }
  }

  /**
   * Override of views_filter().
   */
  function views_filter(&$query, $base_table = '', $relationship = '') {
    switch ($base_table) {
      case 'node':
        $table = $query->ensure_table('taxonomy_index', $relationship);
        $query->add_where(0, "$table.tid", $this->id);
        break;
    }
  }
}
