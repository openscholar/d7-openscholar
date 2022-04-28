<?php

$plugin = array(
  'label' => t('Files'),
  'resource' => 'files',
  'name' => 'files__1_0',
  'entity_type' => 'file',
  'description' => t('Export the file entity type with "cookie" authentication.'),
  'class' => 'OsFilesResource',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
