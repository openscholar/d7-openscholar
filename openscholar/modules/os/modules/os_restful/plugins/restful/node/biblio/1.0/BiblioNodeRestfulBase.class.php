<?php

class BiblioNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['type'] = array(
      'callback' => array($this, 'getBibloType'),
    );

    $public_fields['html_title'] = array(
      'property' => 'title_field',
    );

    $public_fields['redirect'] = array(
      'property' => 'field_biblio_redirect',
    );

    $public_fields['biblio_image'] = array(
      'property' => 'field_biblio_image',
    );

    $public_fields['biblio_extra'] = array(
      'callback' => array($this, 'getBibloExtra'),
    );

    $public_fields['biblio_redirect'] = array(
      'property' => 'field_biblio_redirect',
    );

    $public_fields['biblio_citation_distribute'] = array(
      'callback' => array($this, 'getBibloCitationDistribute'),
    );

    $biblio_fields = array(
      'biblio_year',
      'biblio_secondary_title',
      'biblio_tertiary_title',
      'biblio_volume',
      'biblio_edition',
      'biblio_issue',
      'biblio_section',
      'biblio_number_of_volumes',
      'biblio_number',
      'biblio_pages',
      'biblio_date',
      'biblio_publisher',
      'biblio_place_published',
      'biblio_type_of_work',
      'biblio_lang',
      'biblio_other_author_affiliations',
      'biblio_issn',
      'biblio_isbn',
      'biblio_accession_number',
      'biblio_call_number',
      'biblio_other_number',
      'biblio_keywords',
      'biblio_abst_e',
      'biblio_abst_f',
      'biblio_notes',
      'biblio_url',
      'biblio_url_title',
      'biblio_doi',
      'biblio_research_notes',
      'biblio_custom1',
      'biblio_custom2',
      'biblio_custom3',
      'biblio_custom4',
      'biblio_custom5',
      'biblio_custom6',
      'biblio_custom7',
      'biblio_short_title',
      'biblio_alternate_title',
      'biblio_translated_title',
      'biblio_original_publication',
      'biblio_reprint_edition',
      'biblio_citekey',
      'biblio_remote_db_name',
      'biblio_coins',
      'biblio_remote_db_provider',
      'biblio_label',
      'biblio_auth_address',
      'biblio_access_date',
      'biblio_refereed',
      'biblio_contributors',
      'biblio_year_coded',
    );

    foreach ($biblio_fields as $biblio_field) {
      $public_fields[$biblio_field] = array(
        'callback' => array(
          array($this, 'getBiblioValue'),
          array($biblio_field),
        ),
      );
    }
    return $public_fields;
  }

  /**
   * Callback for getting biblio type.
   */
  public function getBibloType($wrapper) {
    $node = $wrapper->value();
    $query = db_select('biblio_types', 'bt')
    ->fields('bt', array('tid', 'name'))
    ->condition('tid', $node->biblio_type);
    $result = $query->execute()->fetchAssoc();

    return $result;
  }

  /**
   * Callback for getting biblio properties.
   */
  public function getBiblioValue($wrapper, $field_name) {
    $node = $wrapper->value();
    return $node->{$field_name};
  }

  /**
   * Callback for getting biblio extra field.
   */
  public function getBibloExtra($wrapper) {
    $node = $wrapper->value();
    return $node->field_biblio_extra[LANGUAGE_NONE];
  }

  /**
   * Callback for getting biblio citation distribute.
   */
  public function getBibloCitationDistribute($wrapper) {
    if (!db_table_exists('citation_distribute')) {
      return [];
    }
    
    $node = $wrapper->value();
    $query = db_select('citation_distribute', 'cd')
    ->condition('nid', $node->nid)
    ->fields('cd', array('plugin'))
    ->execute();
    $plugins = [];
    while ($row = $query->fetchAssoc()) {
      $plugins[] = $row['plugin'];
    }
    return $plugins;
  }

  /**
   * Get the pager range.
   *
   * @return int
   *  The range.
   */
  public function getRange() {
    return 500;
  }

}
