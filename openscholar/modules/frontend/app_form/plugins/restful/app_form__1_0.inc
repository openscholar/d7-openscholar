<?php

$plugin = array(
  'label' => t('Enable / Disable Apps'),
  'description' => t('End point for app enablement'),
  'resource' => 'apps',
  'class' => 'RestfulAppForm',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
  'hook_menu' => TRUE,
//  'render_cache' => array(
//    'render' => TRUE,
//    'granularity' => DRUPAL_CACHE_PER_USER,
//  ),
);