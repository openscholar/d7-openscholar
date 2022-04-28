<?php

/**
 * @file
 * google scholar class provides publication methods to theme biblio nodes with
 * google metadata
 */

$plugin = array(
  'class' => 'citation_distribute_service_googlescholar',
  'file' => 'citation_distribute_service_googlescholar.inc',
  'parent' => 'citation_distribute_service',
  'title' => t('Google Scholar'),
  'href' => 'http://scholar.google.com',
  'name' => 'googlescholar',
  'description' => 'Google\'s searchable index of citations',
  'type' => 'metadata'
);
class citation_distribute_service_googlescholar extends citation_distribute_service {
  public function save($nid, $plugin) {
    /*
     * google_scholar themes a node if it has an entry in {citation_distribute}
     * with type=google_scholar to reach this point that must have happened, so
     * the change is already saved.
     */
    return true;
  }
  public function map_metadata($nid) {
    $node = node_load($nid);

    /* returns array of author names formatted for google scholar */
    function _googlescholar_list_authors($contributors = array()) {
      $authors = array();
      foreach ( $contributors as $cont ) {
        $authors[] = htmlspecialchars(strip_tags($cont['name']), ENT_COMPAT, 'ISO-8859-1', FALSE);
      }
      return $authors;
    }

    /*
     * returns $date in YYYY/M/D if possible. just year if not. 'submitted' is
     * not a year
     */
    function _googlescholar_date($year, $date) {
      if ($date) {
        return date('Y/m/d', strtotime($date));
      }

      if ($year) {
        return $year;
      }

      return false;
    }

    $metadata = array(
      'citation_journal_title' => 'biblio_secondary_title',
      'citation_publisher' => 'biblio_publisher',
      'citation_title' => 'title',
      'citation_year' => 'biblio_year',
      'citation_volume' => 'biblio_volume',
      'citation_issue' => 'biblio_issue',
      'citation_issn' => 'biblio_issn',
      'citation_isbn' => 'biblio_isbn',
      'citation_language' => 'biblio_language',
      'citation_abstract' => 'biblio_abstract',
      'citation_abstract_html_url' => 'biblio_url',
    );

    foreach ($metadata as $key => $value) {
      $metadata[$key] = (isset($node->{$value})) ? htmlspecialchars(strip_tags($node->{$value}), ENT_COMPAT, 'ISO-8859-1', FALSE) : NULL;
    }

    if (isset($node->biblio_keywords)) {
      $metadata['citation_keywords'] = htmlspecialchars(strip_tags(implode(';', $node->biblio_keywords)), ENT_COMPAT, 'ISO-8859-1', FALSE);
    }

    if (isset($node->biblio_year, $node->biblio_date)) {
      $metadata['citation_publication_date'] = _googlescholar_date(( int ) $node->biblio_year, $node->biblio_date);
    }

    if (isset($node->biblio_contributors) && count($node->biblio_contributors)) {
      $metadata['citation_author'] = _googlescholar_list_authors($node->biblio_contributors);
    }

    return $metadata;
  }
  public function render($variables) {
    $metadata = $this->map_metadata($variables['nid']);

    /**
     * Themable function to generate message after user submits
     * cite_distribute widget selections
     *
     * @param array $metadata
     *          associative array of GS metadata
     * @return unknown HTML string of that metadata
     */
    $output = "";
    foreach ( $metadata as $key => $value ) {
      if ($value) {
        if (is_array($value)) {
          foreach ( $value as $subvalue ) {
            $output .= '<meta name="' . $key . '" content="' . $subvalue . '" />' . "\n";
          }
        } else {
          $output .= '<meta name="' . $key . '" content="' . $value . '" />' . "\n";
        }
      }
    }
    return $output;
  }
}

