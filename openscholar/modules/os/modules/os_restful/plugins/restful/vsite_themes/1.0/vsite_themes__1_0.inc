<?php

$plugin = array(
  'label' => t('vsite themes'),
  'description' => t('End point for vsite themes.'),
  'resource' => 'themes',
  'class' => 'OsRestfulThemes',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
);
