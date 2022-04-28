<?php

$plugin = array(
  'label' => t('News'),
  'resource' => 'news',
  'name' => 'news__1_0',
  'entity_type' => 'node',
  'bundle' => 'news',
  'description' => t('Expose news bundle via restufl'),
  'class' => 'NewsNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
