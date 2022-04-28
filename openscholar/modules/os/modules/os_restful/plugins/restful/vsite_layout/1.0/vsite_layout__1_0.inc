<?php

$plugin = array(
  'label' => t('vsite layout'),
  'description' => t('End point for vsite layout.'),
  'resource' => 'layouts',
  'class' => 'OsRestfulLayout',
  'data_provider_options' => array(
    'table_name' => 'spaces_overrides',
    'id_column' => array('object_type', 'object_id', 'id'),
  ),
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
);
