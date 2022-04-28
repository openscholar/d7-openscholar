<?php

$plugin = array(
  'label' => t('Link'),
  'resource' => 'link',
  'name' => 'link__1_0',
  'entity_type' => 'node',
  'bundle' => 'link',
  'description' => t('Expose link bundle via restufl'),
  'class' => 'LinkNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
