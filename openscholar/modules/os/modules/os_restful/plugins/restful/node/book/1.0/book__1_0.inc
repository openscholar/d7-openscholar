<?php

$plugin = array(
  'label' => t('Book'),
  'resource' => 'book',
  'name' => 'book__1_0',
  'entity_type' => 'node',
  'bundle' => 'book',
  'description' => t('Expose book bundle via restufl'),
  'class' => 'BookNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
