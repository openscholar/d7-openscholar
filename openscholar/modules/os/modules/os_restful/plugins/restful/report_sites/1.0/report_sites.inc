<?php

$plugin = array(
  'label' => t('OS Site Reports'),
  'resource' => 'report_sites',
  'name' => 'report_sites',
  'description' => t('End point for OS Site reports.'),
  'class' => 'OsRestfulSiteReport',
  'data_provider_options' => array(
    'table_name' => 'purl',
    'id_column' => array('id'),
  ),
  'authentication_types' => TRUE,
  'authentication_optional' => FALSE,
);
