<?php

$plugin = array(
  'label' => t('Class material'),
  'resource' => 'class_material',
  'name' => 'class_material__1_0',
  'entity_type' => 'node',
  'bundle' => 'class_material',
  'description' => t('Expose class material bundle via restufl'),
  'class' => 'ClassMaterialNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
