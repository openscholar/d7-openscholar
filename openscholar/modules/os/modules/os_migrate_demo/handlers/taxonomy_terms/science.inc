<?php

/**
 * Migrate "Science" terms.
 */
class OsMigrateScienceTaxonomyTerm extends OsMigrate {

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'science';

  public $ogVocab = TRUE;
}
