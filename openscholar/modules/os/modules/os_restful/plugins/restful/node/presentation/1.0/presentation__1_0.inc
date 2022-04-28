<?php

$plugin = array(
  'label' => t('Presentation'),
  'resource' => 'presentation',
  'name' => 'presentation__1_0',
  'entity_type' => 'node',
  'bundle' => 'presentation',
  'description' => t('Expose presentation bundle via restufl'),
  'class' => 'PresentationNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
