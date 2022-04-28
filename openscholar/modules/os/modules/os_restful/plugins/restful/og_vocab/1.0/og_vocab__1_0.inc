<?php

$plugin = array(
  'label' => t('OG vocab'),
  'resource' => 'og_vocab',
  'name' => 'og_vocab__1_0',
  'entity_type' => 'og_vocab',
  'bundle' => FALSE,
  'description' => t('Expose OG vocabulary via restufl'),
  'class' => 'OsOgVocab',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
