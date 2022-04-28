<?php

$plugin = array(
  'label' => t('vsite boxes'),
  'description' => t('End point for vsite boxes.'),
  'resource' => 'boxes',
  'class' => 'OsRestfulBoxes',
  'data_provider_options' => array(
    'table_name' => 'spaces_overrides',
    'id_column' => array('object_type', 'object_id', 'id'),
  ),
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
);
