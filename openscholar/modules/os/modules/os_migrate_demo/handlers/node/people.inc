<?php

/**
 * Migrate people.
 */
class OsMigratePeopleNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'person';

  public $csvColumns = array(
    array('id', 'ID'),
    array('prefix', 'Prefix'),
    array('title', 'First name'),
    array('middle_name', 'Middle name'),
    array('last_name', 'Last name'),
    array('phone', 'Phone'),
    array('professional_titles', 'Professional titles'),
    array('address', 'Address'),
    array('email', 'E-mail'),
    array('websites', 'Websites'),
    array('body', 'Body'),
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

    $this->addFieldMapping('field_prefix', 'prefix');
    $this->addFieldMapping('field_middle_name_or_initial', 'middle_name');
    $this->addFieldMapping('field_last_name', 'last_name');
    $this->addFieldMapping('field_phone', 'phone');
    $this->addFieldMapping('field_email', 'email');
    $this->addFieldMapping('field_address', 'address');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_professional_title', 'professional_titles')->separator(';');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration(array('OsMigratePersonalNode', 'OsMigrateProjectNode'));
    $this->addFieldMapping(OG_VOCAB_FIELD, OG_VOCAB_FIELD)
      ->sourceMigration(array('OsMigrateScienceTaxonomyTerm'))
      ->separator('|');

    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');
  }

  public function prepare($entity, $row) {
    // Inserting the first name - title - into the first name field.
    $entity->field_first_name[LANGUAGE_NONE][0]['value'] = $entity->title;

    // Handeling websites.
    if (!empty($row->websites)) {
      $websites = explode(';', $row->websites);
      foreach ($websites as $key => $website) {
        $link = explode('|', $website);

        $entity->field_website[LANGUAGE_NONE][$key] = array(
          'title' => $link[0],
          'url' => $link[1],
        );
      }
    }
  }
}
