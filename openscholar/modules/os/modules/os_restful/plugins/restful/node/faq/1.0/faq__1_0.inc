<?php

$plugin = array(
  'label' => t('FAQ'),
  'resource' => 'faq',
  'name' => 'faq__1_0',
  'entity_type' => 'node',
  'bundle' => 'faq',
  'description' => t('Expose faq bundle via restufl'),
  'class' => 'FAQNodeRestfulBase',
  'authentication_types' => TRUE,
  'authentication_optional' => TRUE,
);
