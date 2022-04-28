<?php

$plugin = array(
  'label' => t('Media gallery'),
  'resource' => 'media_gallery',
  'name' => 'media_gallery__1_0',
  'entity_type' => 'node',
  'bundle' => 'media_gallery',
  'description' => t('Expose media gallery bundle via restufl'),
  'class' => 'MediaGalleryNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
