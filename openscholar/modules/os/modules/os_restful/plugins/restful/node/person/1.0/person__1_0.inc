<?php

$plugin = array(
  'label' => t('Person'),
  'resource' => 'person',
  'name' => 'person__1_0',
  'entity_type' => 'node',
  'bundle' => 'person',
  'description' => t('Expose presentation bundle via restufl'),
  'class' => 'PersonNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
