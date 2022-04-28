<?php

$plugin = array(
  'label' => t('Vocabulary'),
  'resource' => 'vocabulary',
  'name' => 'vocabulary__1_0',
  'entity_type' => 'taxonomy_vocabulary',
  'bundle' => FALSE,
  'description' => t('Expose vocabularies via restufl'),
  'class' => 'OsVocabulary',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
