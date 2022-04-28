<?php

$plugin = array(
  'label' => t('Group'),
  'resource' => 'group',
  'name' => 'group__1_0',
  'entity_type' => 'node',
  'bundle' => FALSE,
  'description' => t('Expose group bundle via restufl'),
  'class' => 'GroupNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
