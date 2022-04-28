<?php

$plugin = array(
  'label' => t('page'),
  'resource' => 'page',
  'name' => 'page__1_0',
  'entity_type' => 'node',
  'bundle' => 'page',
  'description' => t('Expose page bundle via restufl'),
  'class' => 'PageNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
