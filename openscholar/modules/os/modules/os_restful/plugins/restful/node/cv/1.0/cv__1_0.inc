<?php

$plugin = array(
  'label' => t('CV'),
  'resource' => 'cv',
  'name' => 'cv__1_0',
  'entity_type' => 'node',
  'bundle' => 'cv',
  'description' => t('Expose CV bundle via restufl'),
  'class' => 'CVNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
