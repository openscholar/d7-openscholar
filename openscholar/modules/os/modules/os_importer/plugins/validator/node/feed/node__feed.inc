<?php

$plugin = array(
  'label' => t('Feed'),
  'description' => t('Validating the feed content type.'),
  'entity_type' => 'node',
  'bundle' => 'feed',
  'class' => 'OsImporterFeedValidator',
);
