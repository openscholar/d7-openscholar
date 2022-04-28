<?php

$plugin = array(
  'label' => t('Control Panel Menus'),
  'description' => t('End point for administrative menus.'),
  'resource' => 'cp_menu',
  'class' => 'OsRestfulCPMenu',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
//  'render_cache' => array(
//    'render' => TRUE,
//    'granularity' => DRUPAL_CACHE_PER_USER,
//  ),
);
