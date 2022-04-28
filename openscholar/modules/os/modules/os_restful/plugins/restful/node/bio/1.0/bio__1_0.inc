<?php

$plugin = array(
  'label' => t('Bio'),
  'resource' => 'bio',
  'name' => 'bio__1_0',
  'entity_type' => 'node',
  'bundle' => 'bio',
  'description' => t('Expose bio bundle via restufl'),
  'class' => 'BioNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
