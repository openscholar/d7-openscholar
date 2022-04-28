<?php

$plugin = array(
  'label' => t('Control Panel Settings Forms'),
  'description' => t('End point for settings forms'),
  'resource' => 'settings',
  'class' => 'OsRestfulCPSettings',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
//  'render_cache' => array(
//    'render' => TRUE,
//    'granularity' => DRUPAL_CACHE_PER_USER,
//  ),
);