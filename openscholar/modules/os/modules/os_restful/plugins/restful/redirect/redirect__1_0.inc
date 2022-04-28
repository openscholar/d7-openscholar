<?php

$plugin = array(
  'label' => t('Redirects'),
  'description' => t('End point for redirects'),
  'resource' => 'redirect',
  'entity_type' => 'redirect',
  'class' => 'OsRestfulRedirect',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
);