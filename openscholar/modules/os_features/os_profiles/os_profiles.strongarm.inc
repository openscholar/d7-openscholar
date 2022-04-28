<?php
/**
 * @file
 * os_profiles.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function os_profiles_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_person';
  $strongarm->value = 0;
  $export['comment_anonymous_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_person';
  $strongarm->value = 1;
  $export['comment_default_mode_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_person';
  $strongarm->value = '50';
  $export['comment_default_per_page_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_person';
  $strongarm->value = 1;
  $export['comment_form_location_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_person';
  $strongarm->value = '0';
  $export['comment_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_person';
  $strongarm->value = '1';
  $export['comment_preview_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_person';
  $strongarm->value = 1;
  $export['comment_subject_field_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_show_preview_changes_node_person';
  $strongarm->value = 0;
  $export['diff_show_preview_changes_node_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__person';
  $strongarm->value = array(
    'view_modes' => array(
      'full' => array(
        'custom_settings' => TRUE,
      ),
    ),
  );
  $export['field_bundle_settings_node__person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_person';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_person';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeformscols_field_placements_person_default';
  $strongarm->value = array(
    'actions' => array(
      'region' => 'footer',
      'weight' => '20',
      'has_required' => FALSE,
      'title' => 'Save',
      'hidden' => 0,
    ),
    'revision_information' => array(
      'region' => 'right',
      'weight' => '6',
      'has_required' => FALSE,
      'title' => 'Revision information',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'author' => array(
      'region' => 'right',
      'weight' => '4',
      'has_required' => FALSE,
      'title' => 'Authoring information',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'options' => array(
      'region' => 'right',
      'weight' => '1',
      'has_required' => FALSE,
      'title' => 'Publishing options',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'body' => array(
      'region' => 'main',
      'weight' => '5',
      'has_required' => FALSE,
      'title' => 'Short bio',
      'hidden' => 0,
    ),
    'field_address' => array(
      'region' => 'main',
      'weight' => '7',
      'has_required' => FALSE,
      'title' => 'Address',
      'hidden' => 0,
    ),
    'field_email' => array(
      'region' => 'main',
      'weight' => '8',
      'has_required' => FALSE,
      'title' => 'Email',
      'hidden' => 0,
    ),
    'field_first_name' => array(
      'region' => 'main',
      'weight' => '1',
      'has_required' => TRUE,
      'title' => 'First Name',
    ),
    'field_last_name' => array(
      'region' => 'main',
      'weight' => '3',
      'has_required' => TRUE,
      'title' => 'Last Name',
    ),
    'field_middle_name_or_initial' => array(
      'region' => 'main',
      'weight' => '2',
      'has_required' => FALSE,
      'title' => 'Middle Name or Initial',
      'hidden' => 0,
    ),
    'field_person_photo' => array(
      'region' => 'main',
      'weight' => '11',
      'has_required' => FALSE,
      'title' => 'Photo',
      'hidden' => 0,
    ),
    'field_phone' => array(
      'region' => 'main',
      'weight' => '9',
      'has_required' => FALSE,
      'title' => 'Phone',
      'hidden' => 0,
    ),
    'field_professional_title' => array(
      'region' => 'main',
      'weight' => '4',
      'has_required' => FALSE,
      'title' => 'Professional Titles/Affiliations',
      'hidden' => 0,
    ),
    'field_url' => array(
      'region' => 'main',
      'weight' => '13',
      'has_required' => FALSE,
      'title' => 'Redirect',
      'hidden' => 0,
    ),
    'field_website' => array(
      'region' => 'main',
      'weight' => '6',
      'has_required' => FALSE,
      'title' => 'Websites',
      'hidden' => 0,
    ),
    'og_group_ref' => array(
      'region' => 'main',
      'weight' => '14',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'og_vocabulary' => array(
      'region' => 'main',
      'weight' => '15',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'path' => array(
      'region' => 'right',
      'weight' => '2',
      'has_required' => FALSE,
      'title' => 'URL path settings',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'os_menu' => array(
      'region' => 'right',
      'weight' => '3',
      'has_required' => FALSE,
      'title' => 'Menu settings',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'actions_top' => array(
      'region' => 'right',
      'weight' => '0',
      'has_required' => FALSE,
      'title' => 'Save',
      'hidden' => 0,
    ),
    'field_child_site' => array(
      'region' => 'main',
      'weight' => '12',
      'has_required' => FALSE,
      'title' => 'Child site',
      'hidden' => 0,
    ),
    'field_destination_url' => array(
      'region' => 'main',
      'weight' => '16',
      'has_required' => FALSE,
      'title' => 'Destination URL',
      'hidden' => 0,
    ),
    'field_office_hours' => array(
      'region' => 'main',
      'weight' => '10',
      'has_required' => FALSE,
      'title' => 'Office Hours',
      'hidden' => 0,
    ),
    'field_original_destination_url' => array(
      'region' => 'main',
      'weight' => '18',
      'has_required' => FALSE,
      'title' => 'Original destination URL',
      'hidden' => 0,
    ),
    'field_prefix' => array(
      'region' => 'main',
      'weight' => '0',
      'has_required' => FALSE,
      'title' => 'Prefix',
      'hidden' => 0,
    ),
    'field_uuid' => array(
      'region' => 'main',
      'weight' => '17',
      'has_required' => FALSE,
      'title' => 'UUID',
      'hidden' => 0,
    ),
    'metatags' => array(
      'region' => 'main',
      'weight' => '19',
      'has_required' => FALSE,
      'title' => 'Meta tags',
      'collapsed' => 1,
      'hidden' => 0,
    ),
  );
  $export['nodeformscols_field_placements_person_default'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_person';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision',
  );
  $export['node_options_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_person';
  $strongarm->value = '0';
  $export['node_preview_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_person';
  $strongarm->value = 0;
  $export['node_submitted_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_person_pattern';
  $strongarm->value = 'people/[node:title]';
  $export['pathauto_node_person_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'enable_revisions_person';
  $strongarm->value = TRUE;
  $export['enable_revisions_person'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restrict_node_revision_number_for_person';
  $strongarm->value = 10;
  $export['restrict_node_revision_number_for_person'] = $strongarm;

  return $export;
}
