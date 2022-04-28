<?php

/**
 * Migrate "Biology" terms.
 */
class OsMigrateBiologyTaxonomyTerm extends OsMigrate {

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'biology';

  public $ogVocab = TRUE;
}
