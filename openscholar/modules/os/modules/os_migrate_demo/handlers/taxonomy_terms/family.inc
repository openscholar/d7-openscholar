<?php

/**
 * Migrate "Family" terms.
 */
class OsMigrateFamilyTaxonomyTerm extends OsMigrate {

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'family';

  public $ogVocab = TRUE;
}
