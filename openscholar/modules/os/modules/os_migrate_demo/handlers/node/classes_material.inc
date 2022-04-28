<?php

/**
 * Migrate classes material.
 */
class OsMigrateClassesMaterialNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'class_material';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('field_class', 'Class'),
    array(OG_AUDIENCE_FIELD, 'Site'),
    array('uid', 'UID'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
    'OsMigrateProjectNode',
    'OsMigrateClassesNode',
    'OsMigrateUsers',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration(array('OsMigratePersonalNode', 'OsMigrateProjectNode'));
    $this->addFieldMapping('field_class', 'field_class')
      ->sourceMigration('OsMigrateClassesNode');

    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');
  }
}
