<?php

$plugin = array(
  'label' => t('Feed'),
  'resource' => 'feed',
  'name' => 'feed__1_0',
  'entity_type' => 'node',
  'bundle' => 'feed',
  'description' => t('Expose feed bundle via restufl'),
  'class' => 'FeedNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
