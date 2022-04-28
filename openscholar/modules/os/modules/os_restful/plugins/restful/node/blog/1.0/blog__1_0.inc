<?php

$plugin = array(
  'label' => t('Blog'),
  'resource' => 'blog',
  'name' => 'blog__1_0',
  'entity_type' => 'node',
  'bundle' => 'blog',
  'description' => t('Expose blog bundle via restufl'),
  'class' => 'BlogNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
