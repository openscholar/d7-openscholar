<?php

/**
 * Migrate OG-vocabs.
 *
 * @todo: Work in progress.
 */
class OsMigrateOgVocab extends OsMigrate {

  public $csvColumns = array(
    array('id', 'ID'),
    array('site', 'Site'),
    array('vocabulary', 'Vocabulary'),
    array('required', 'Required'),
    array('cardinality', 'Cardinality'),
    array('widget', 'Widget'),
  );

  public $entityType = 'og_vocab';
  public $bundle = 'og_vocab';

  public $dependencies = array(
    'OsMigrateAuthorsTaxonomyTerm',
    'OsMigrateBiologyTaxonomyTerm',
    'OsMigrateMathTaxonomyTerm',
    'OsMigrateScienceTaxonomyTerm',
  );

  protected $enabled = FALSE;

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('vocabulary', 'vocabulary');
    $this
      ->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration('OsMigratePersonalNode');

    $this->addFieldMapping('vid');
    $this->addFieldMapping('settings')->defaultValue(array());
  }

  public function prepareRow($row) {
    $name = $row->vocabulary . '_' . $row->{OG_AUDIENCE_FIELD};
    $vocabulary = taxonomy_vocabulary_machine_name_load($name);

    $row->vid = $vocabulary->vid;
    $row->entity_type = 'node';
    $row->bundle = 'class';

    $keys = array(
      'required',
      'cardinality',
      'widget',
    );
    foreach ($keys as $key) {
      $row->settings[$key] = $row->{$key};
    }
  }

  public function prepare($entity, $row) {
  }
}
