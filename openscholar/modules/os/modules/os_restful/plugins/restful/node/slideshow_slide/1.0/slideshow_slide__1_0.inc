<?php

$plugin = array(
  'label' => t('Slideshow slide'),
  'resource' => 'slideshow_slide',
  'name' => 'slideshow_slide__1_0',
  'entity_type' => 'node',
  'bundle' => 'slideshow_slide',
  'description' => t('Expose slide show bundle via restufl'),
  'class' => 'SlideshowSlideNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
