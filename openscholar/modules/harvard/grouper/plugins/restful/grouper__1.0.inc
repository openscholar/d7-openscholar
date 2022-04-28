<?php

$plugin = array(
  'label' => t('Grouper endpoint'),
  'description' => t('End point for Grouper interactions'),
  'resource' => 'grouper',
  'class' => 'Grouper',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
//  'render_cache' => array(
//    'render' => TRUE,
//    'granularity' => DRUPAL_CACHE_PER_USER,
//  ),
);