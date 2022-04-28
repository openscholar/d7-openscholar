<?php
$plugin = array(
  'label' => t('Users'),
  'description' => t('Export the "User" entity.'),
  'resource' => 'users',
  'name' => 'users__1_0',
  'class' => 'OsRestfulUser',
  'entity_type' => 'user',
  'bundle' => 'user',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
