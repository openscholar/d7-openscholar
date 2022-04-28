<?php

/**
 * Migrate events.
 */
class OsMigrateEventsNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'event';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('field_date', 'Timestamp'),
    array(OG_AUDIENCE_FIELD, 'Site'),
    array(OG_VOCAB_FIELD, 'Terms'),
    array('uid', 'UID'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
    'OsMigrateProjectNode',
    'OsMigrateAuthorsTaxonomyTerm',
    'OsMigrateUsers',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_date', 'field_date');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration(array('OsMigratePersonalNode', 'OsMigrateProjectNode'));
    $this->addFieldMapping(OG_VOCAB_FIELD, OG_VOCAB_FIELD)
      ->sourceMigration(array('OsMigrateAuthorsTaxonomyTerm'))
      ->separator('|');

    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');
  }

  public function prepareRow($row) {
    parent::prepareRow($row);

    $date = explode(';', $row->field_date);
    $row->field_date = array($date[0]);

    // Adding the end date of the event.
    if (isset($date[1])) {
      $row->field_date['arguments'] = array('to' => $date[1]);
    }
  }
}
