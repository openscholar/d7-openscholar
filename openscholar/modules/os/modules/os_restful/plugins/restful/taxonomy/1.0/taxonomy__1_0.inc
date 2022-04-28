<?php

$plugin = array(
  'label' => t('Taxonomy'),
  'resource' => 'taxonomy',
  'name' => 'taxonomy__1_0',
  'entity_type' => 'taxonomy_term',
  'bundle' => FALSE,
  'description' => t('Expose terms bundle via restufl'),
  'class' => 'OsTaxonomyTerm',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
