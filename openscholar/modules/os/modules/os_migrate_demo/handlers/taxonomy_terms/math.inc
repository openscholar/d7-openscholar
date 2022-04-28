<?php

/**
 * Migrate "Math" terms.
 */
class OsMigrateMathTaxonomyTerm extends OsMigrate {

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'math';

  public $ogVocab = TRUE;
}
