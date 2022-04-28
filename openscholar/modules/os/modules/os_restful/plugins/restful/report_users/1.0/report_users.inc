<?php

$plugin = array(
  'label' => t('OS User Reports'),
  'description' => t('End point for OS User reports.'),
  'name' => 'report_users',
  'resource' => 'report_users',
  'class' => 'OsRestfulUserReport',
  'data_provider_options' => array(
    'table_name' => 'users',
    'id_column' => array('uid'),
  ),
  'authentication_types' => TRUE,
  'authentication_optional' => FALSE,
);
