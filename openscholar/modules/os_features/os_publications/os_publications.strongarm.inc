<?php
/**
 * @file
 * os_publications.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function os_publications_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_author_links';
  $strongarm->value = 0;
  $export['biblio_author_links'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_base';
  $strongarm->value = 'publications';
  $export['biblio_base'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_base_title';
  $strongarm->value = 'Publications';
  $export['biblio_base_title'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_contrib_fields_delta';
  $strongarm->value = '1';
  $export['biblio_contrib_fields_delta'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_export_links';
  $strongarm->value = array(
    'bibtex' => 'bibtex',
    'rtf' => 'rtf',
    'tagged' => 'tagged',
    'xml' => 'xml',
    'ris' => 'ris',
  );
  $export['biblio_export_links'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_init_auth_count';
  $strongarm->value = '1';
  $export['biblio_init_auth_count'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'biblio_rowsperpage';
  $strongarm->value = '20';
  $export['biblio_rowsperpage'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_biblio';
  $strongarm->value = 0;
  $export['comment_anonymous_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_biblio';
  $strongarm->value = 0;
  $export['comment_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_biblio';
  $strongarm->value = 1;
  $export['comment_default_mode_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_biblio';
  $strongarm->value = '50';
  $export['comment_default_per_page_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_biblio';
  $strongarm->value = 1;
  $export['comment_form_location_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_biblio';
  $strongarm->value = '1';
  $export['comment_preview_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_biblio';
  $strongarm->value = 1;
  $export['comment_subject_field_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'diff_show_preview_changes_node_biblio';
  $strongarm->value = 0;
  $export['diff_show_preview_changes_node_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_biblio';
  $strongarm->value = array(
    0 => 'primary-menu',
    1 => 'secondary-menu',
  );
  $export['menu_options_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_biblio';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodeformscols_field_placements_biblio_default';
  $strongarm->value = array(
    'actions' => array(
      'region' => 'footer',
      'weight' => '13',
      'has_required' => FALSE,
      'title' => 'Save',
      'hidden' => 0,
    ),
    'biblio_tabs' => array(
      'region' => 'main',
      'weight' => '6',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'biblio_type' => array(
      'region' => 'main',
      'weight' => '0',
      'has_required' => TRUE,
      'title' => 'Publication Type',
    ),
    'biblio_next' => array(
      'region' => 'main',
      'weight' => '5',
      'has_required' => FALSE,
      'title' => 'Next',
      'hidden' => 0,
    ),
    'revision_information' => array(
      'region' => 'right',
      'weight' => '7',
      'has_required' => FALSE,
      'title' => 'Revision information',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'author' => array(
      'region' => 'right',
      'weight' => '5',
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
    'field_biblio_extra' => array(
      'region' => 'main',
      'weight' => '9',
      'has_required' => FALSE,
      'title' => 'Extra Fields',
      'hidden' => 0,
    ),
    'field_biblio_image' => array(
      'region' => 'main',
      'weight' => '7',
      'has_required' => FALSE,
      'title' => 'Publication Image',
      'hidden' => 0,
    ),
    'field_upload' => array(
      'region' => 'main',
      'weight' => '8',
      'has_required' => FALSE,
      'title' => 'Attach Files',
      'hidden' => 0,
    ),
    'og_group_ref' => array(
      'region' => 'main',
      'weight' => '10',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'og_vocabulary' => array(
      'region' => 'main',
      'weight' => '11',
      'has_required' => FALSE,
      'title' => NULL,
      'hidden' => 0,
    ),
    'comment_settings' => array(
      'region' => 'right',
      'weight' => '6',
      'has_required' => FALSE,
      'title' => 'Comment settings',
      'collapsed' => 1,
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
    'biblio_pubmed_lookup' => array(
      'region' => 'main',
      'weight' => '2',
      'has_required' => FALSE,
      'title' => 'PubMed Lookup',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'biblio_doi_lookup' => array(
      'region' => 'main',
      'weight' => '3',
      'has_required' => FALSE,
      'title' => 'DOI Lookup',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'biblio_cut_paste' => array(
      'region' => 'main',
      'weight' => '1',
      'has_required' => FALSE,
      'title' => 'Paste BibTex Record',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'biblio_ris_paste' => array(
      'region' => 'main',
      'weight' => '4',
      'has_required' => FALSE,
      'title' => 'Paste RIS Record',
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
    'title_field' => array(
      'region' => 'main',
      'weight' => '-5',
      'has_required' => TRUE,
      'title' => 'Title',
    ),
    'metatags' => array(
      'region' => 'main',
      'weight' => '40',
      'has_required' => FALSE,
      'title' => 'Meta tags',
      'collapsed' => 1,
      'hidden' => 0,
    ),
    'citation_distribute' => array(
      'region' => 'right',
      'weight' => '4',
      'has_required' => FALSE,
      'title' => 'Citation Distribute',
      'hidden' => 0,
    ),
    'gkplus' => array(
      'region' => 'right',
      'weight' => 80,
      'has_required' => FALSE,
      'title' => 'Custom theme settings',
      'hidden' => 0,
    ),
  );
  $export['nodeformscols_field_placements_biblio_default'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_biblio';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision',
  );
  $export['node_options_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_biblio';
  $strongarm->value = '0';
  $export['node_preview_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_biblio';
  $strongarm->value = 1;
  $export['node_submitted_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_biblio_pattern';
  $strongarm->value = 'publications/[node:title]';
  $export['pathauto_node_biblio_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'enable_revisions_biblio';
  $strongarm->value = TRUE;
  $export['enable_revisions_biblio'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'restrict_node_revision_number_for_biblio';
  $strongarm->value = 10;
  $export['restrict_node_revision_number_for_biblio'] = $strongarm;

  return $export;
}
