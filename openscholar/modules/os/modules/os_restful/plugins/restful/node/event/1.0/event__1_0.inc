<?php

$plugin = array(
  'label' => t('Event'),
  'resource' => 'event',
  'name' => 'event__1_0',
  'entity_type' => 'node',
  'bundle' => 'event',
  'description' => t('Expose event bundle via restufl'),
  'class' => 'EventNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
