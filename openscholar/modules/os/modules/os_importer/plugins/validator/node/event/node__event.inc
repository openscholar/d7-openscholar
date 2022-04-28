<?php

$plugin = array(
  'label' => t('Event'),
  'description' => t('Validating the event content type.'),
  'bundle' => 'event',
  'entity_type' => 'node',
  'class' => 'OsImporterEventValidator',
);
