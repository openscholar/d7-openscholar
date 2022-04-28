<?php

/**
 * Migrate Blog.
 */
class OsMigrateBlogNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'blog';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array(OG_AUDIENCE_FIELD, 'Site'),
    array(OG_VOCAB_FIELD, 'Terms'),
    array('uid', 'UID'),
    array('status','Status'),
  );

  public $dependencies = array(
    'OsMigrateScienceTaxonomyTerm',
    'OsMigratePersonalNode',
    'OsMigrateProjectNode',
    'OsMigrateUsers',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration(array('OsMigrateProjectNode', 'OsMigratePersonalNode'));
    $this->addFieldMapping(OG_VOCAB_FIELD, OG_VOCAB_FIELD)
      ->sourceMigration(array('OsMigrateScienceTaxonomyTerm'))
      ->separator('|');

    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');

    $this->addFieldMapping('status', 'status')
      ->defaultValue(NODE_PUBLISHED);
  }

  public function prepare($entity, $row) {
    // Make sure node has comments.
    $entity->comment = COMMENT_NODE_OPEN;
  }
}
