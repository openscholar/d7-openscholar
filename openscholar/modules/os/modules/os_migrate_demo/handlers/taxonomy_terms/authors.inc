<?php

/**
 * Migrate "Authors" terms.
 */
class OsMigrateAuthorsTaxonomyTerm extends OsMigrate {

  public $dependencies = array(
    'OsMigratePersonalNode',
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'authors';

  public $ogVocab = TRUE;
}
