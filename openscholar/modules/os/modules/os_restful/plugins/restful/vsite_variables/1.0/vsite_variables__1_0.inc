<?php

$plugin = array(
  'label' => t('vsite variables'),
  'description' => t('End point for vsite variables.'),
  'resource' => 'variables',
  'class' => 'OsRestfulVariables',
  'data_provider_options' => array(
    'table_name' => 'spaces_overrides',
    'id_column' => array('object_type', 'object_id', 'id'),
  ),
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
);
