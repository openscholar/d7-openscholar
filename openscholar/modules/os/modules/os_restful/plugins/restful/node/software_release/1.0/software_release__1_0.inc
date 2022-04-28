<?php

$plugin = array(
  'label' => t('Software release'),
  'resource' => 'software_release',
  'name' => 'software_release__1_0',
  'entity_type' => 'node',
  'bundle' => 'software_release',
  'description' => t('Expose software release bundle via restufl'),
  'class' => 'SoftwareReleaseNodeRestful',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
