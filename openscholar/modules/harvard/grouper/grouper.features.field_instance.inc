<?php
/**
 * @file
 * grouper.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function grouper_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'user-user-field_grouper_path'
  $field_instances['user-user-field_grouper_path'] = array(
    'bundle' => 'user',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'user',
    'field_name' => 'field_grouper_path',
    'label' => 'Grouper Path',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Grouper Path');

  return $field_instances;
}
