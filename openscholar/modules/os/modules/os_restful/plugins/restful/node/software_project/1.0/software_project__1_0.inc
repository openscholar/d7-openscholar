<?php

$plugin = array(
  'label' => t('Software project'),
  'resource' => 'software_project',
  'name' => 'software_project__1_0',
  'entity_type' => 'node',
  'bundle' => 'software_project',
  'description' => t('Expose software project bundle via restufl'),
  'class' => 'SoftwareProjectNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
