<?php

/**
 * Migrate presentation.
 */
class OsMigratePresntationsNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'presentation';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('date', 'Date'),
    array('location', 'Locations'),
    array(OG_AUDIENCE_FIELD, 'Site'),
    array(OG_VOCAB_FIELD, 'Terms'),
    array('uid', 'UID'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
    'OsMigrateProjectNode',
    'OsMigrateScienceTaxonomyTerm',
    'OsMigrateUsers',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_presentation_location', 'location');
    $this->addFieldMapping('field_presentation_date', 'date');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration(array('OsMigratePersonalNode', 'OsMigrateProjectNode'));

    $this->addFieldMapping(OG_VOCAB_FIELD, OG_VOCAB_FIELD)
      ->sourceMigration(array('OsMigrateScienceTaxonomyTerm'))
      ->separator('|');

    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');
  }

  public function prepare($entity, $row) {
    $entity->field_presentation_date[LANGUAGE_NONE][0] = array(
      'value' => $row->date,
      'date_type' => 'datetime',
    );
  }
}
