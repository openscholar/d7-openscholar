<?php
/**
 * @file
 * rbuild.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function rbuild_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-software_project-field_rbuild_short_name'
  $field_instances['node-software_project-field_rbuild_short_name'] = array(
    'bundle' => 'software_project',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Note: Verify this unique rBuild short name corresponds to the Repository URL value above',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'links' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'sidebar_teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'title' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_rbuild_short_name',
    'label' => 'rBuild Short Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-software_project-field_software_repo'
  $field_instances['node-software_project-field_software_repo'] = array(
    'bundle' => 'software_project',
    'default_value' => 'http://r.iq.harvard.edu/src/contrib',
    'deleted' => 0,
    'description' => 'Example: http://r.iq.harvard.edu/src/contrib',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'links' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'sidebar_teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'title' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_software_repo',
    'label' => 'Repository URL',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Example: http://r.iq.harvard.edu/src/contrib');
  t('Note: Verify this unique rBuild short name corresponds to the Repository URL value above');
  t('Repository URL');
  t('rBuild Short Name');

  return $field_instances;
}
