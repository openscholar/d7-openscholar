<?php

/**
 * Migrating books.
 */
class OsMigratePublicationNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'biblio';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('biblio_place_published', 'Publisher'),
    array('biblio_year', 'Year'),
    array('biblio_place_published', 'Publish location'),
    array('biblio_type_name', 'Biblio type name'),
    array('biblio_type', 'Biblio type'),
    array('biblio_secondary_title', 'Secondary title'),
    array(OG_AUDIENCE_FIELD, 'Site'),
    array(OG_VOCAB_FIELD, 'Terms'),
    array('uid', 'UID'),
    array('created', 'Created'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
    'OsMigrateProjectNode',
    'OsMigrateScienceTaxonomyTerm',
    'OsMigrateUsers',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('biblio_place_published', 'biblio_place_published');
    $this->addFieldMapping('biblio_year', 'biblio_year');
    $this->addFieldMapping('Publish location', 'biblio_place_published');
    $this->addFieldMapping('biblio_type_name', 'biblio_type_name');
    $this->addFieldMapping('biblio_secondary_title', 'biblio_secondary_title');
    $this->addFieldMapping('biblio_type', 'biblio_type');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration(array('OsMigratePersonalNode', 'OsMigrateProjectNode'));
    $this->addFieldMapping(OG_VOCAB_FIELD, OG_VOCAB_FIELD)
      ->sourceMigration(array('OsMigrateScienceTaxonomyTerm'))
      ->separator('|');
    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');
    $this->addFieldMapping('created', 'created');
  }


  public function prepareRow($row) {
    // The 'Created' column is optional and defaults to now.
    $row->created = !empty($row->created) ? $row->created : time();
  }

  /**
   * When creating a new publication, the biblio module mange the fields
   * differently and consider them as properties and not fields. The prepare
   * function will deal with that.
   */
  public function prepare($entity, $row) {
    foreach ($row as $row_key => $row_value) {
      if (in_array($row_key, array('id', 'title', 'og_group_ref', 'uid', OG_VOCAB_FIELD))) {
        continue;
      }

      $entity->{$row_key} = $row_value;
    }
  }
}
