<?php
/**
 * @file
 * vsite.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function vsite_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_vsite';
  $strongarm->value = 0;
  $export['comment_anonymous_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_vsite';
  $strongarm->value = 1;
  $export['comment_default_mode_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_vsite';
  $strongarm->value = '50';
  $export['comment_default_per_page_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_vsite';
  $strongarm->value = 1;
  $export['comment_form_location_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_vsite';
  $strongarm->value = '1';
  $export['comment_preview_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_vsite';
  $strongarm->value = 1;
  $export['comment_subject_field_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_vsite';
  $strongarm->value = '2';
  $export['comment_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_long';
  $strongarm->value = 'l, F j, Y, g:ia';
  $export['date_format_long'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_short';
  $strongarm->value = 'n/j/Y g:ia';
  $export['date_format_short'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'locale_custom_strings_en';
  $strongarm->value = array(
    '' => array(
      'User %name is already subscribed to group.' => 'User %name is already a member of this website.',
      '%user has been added to group.' => '%user has been added to the website.',
      'Allow non-members to join a group without an approval from group administrators.' => 'Allow non-members to join a website without an approval from website administrators.',
      'OG audience list' => 'Website audience list',
      'There is no content in this group.' => 'There is no content in this website.',
      'Join this group' => 'Join this website',
      'This is a closed group. Only a group administrator can add you.' => 'This is a closed website. Only a website administrator can add you.',
      'Save selected overrides from this space to the current preset <strong>@preset</strong>.' => 'Save selected overrides from this website to the current preset <strong>@preset</strong>.',
      'Revert the selected overrides for this space.' => 'Revert the selected overrides for this website.',
      'This is the text a user may send to the group administrators.' => 'This is the text a user may send to the website administrators.',
      'Leave this group' => 'Leave this website',
      'Are you sure you want to join the group %title?' => 'Are you sure you want to join the web site %title?',
      'Are you sure you want to remove !name from the group %title?' => 'Are you sure you want to remove !name from the web site %title?',
      'Admin: Create' => 'Make administrator',
      'Admin: Remove' => 'Remove administrator status',
      'Group manager' => 'Website manager',
      'OG audience.' => 'Website audience.',
      'Group description.' => 'Website description.',
      'Delete group' => 'Delete web site',
      'My groups' => 'My websites',
      'Other groups' => 'Other websites',
      'New groups' => 'New Websites',
      '1 user added to the group.' => '1 user added to the website.',
      '@count users added to the group.' => '@count users added to the website.',
      'Private group' => 'Private website',
      'Controlled group' => 'Controlled website',
      'Public group' => 'Public website',
      'Groups' => 'Websites',
      'Your group membership settings were saved successfully.' => 'Your website membership settings were saved successfully.',
      'All users may view public content from this group. User may join this group at will.' => 'All users may view public content from this website. User may join this website at will.',
      'Only members will be able to access this group. Membership is strictly managed by admins.' => 'Only members will be able to access this website Membership is strictly managed by adminstrators.',
      'Biblio' => 'Publication',
      'Secondary Title' => 'Publication Type',
      'Browse by Secondary Title' => 'Browse by Publication Type',
      'Biblio: Filter by Secondary Title' => 'Biblio: Filter by Publication Type',
      'Delete node' => 'Delete',
      'book' => 'booklet',
      'Book' => 'Booklet',
      'Book Page' => 'Booklet',
    ),
  );
  $export['locale_custom_strings_en'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_vsite';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_vsite';
  $strongarm->value = '1';
  $export['node_preview_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_vsite';
  $strongarm->value = 1;
  $export['node_submitted_vsite'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_context_negotiation_group_context';
  $strongarm->value = array(
    'node' => array(),
    'spaces_og' => array(),
  );
  $export['og_context_negotiation_group_context'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_features_ignore_og_fields';
  $strongarm->value = 1;
  $export['og_features_ignore_og_fields'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_group_manager_full_access';
  $strongarm->value = FALSE;
  $export['og_group_manager_full_access'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_node_access_strict';
  $strongarm->value = TRUE;
  $export['og_node_access_strict'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'os_enabled_spaces_presets';
  $strongarm->value = array(
    'os_department' => 'os_department',
    'os_department_minimal' => 'os_department_minimal',
    'os_scholar' => 'os_scholar',
    'os_project' => 'os_project',
    'hwp_administrative' => 'hwp_administrative',
    'hwp_lab_research_group' => 'hwp_lab_research_group',
    'hwp_project' => 'hwp_project',
    'hwp_personal' => 'hwp_personal',
    'hwp_professional' => 'hwp_professional',
    'hwp_event_conference' => 'hwp_event_conference',
  );
  $export['os_enabled_spaces_presets'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_settings';
  $strongarm->value = array(
    'toggle_logo' => 1,
    'toggle_name' => 1,
    'toggle_slogan' => 1,
    'toggle_node_user_picture' => 1,
    'toggle_comment_user_picture' => 1,
    'toggle_comment_user_verification' => 1,
    'toggle_favicon' => 0,
    'toggle_main_menu' => 1,
    'toggle_secondary_menu' => 1,
    'default_logo' => 1,
    'logo_path' => NULL,
    'logo_upload' => NULL,
    'default_favicon' => 0,
    'favicon_path' => NULL,
    'favicon_upload' => NULL,
    'nice_menus_custom_css' => NULL,
  );
  $export['theme_settings'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'os_vsites_per_user';
  $strongarm->value = 1;
  $export['os_vsites_per_user'] = $strongarm;

  return $export;
}
