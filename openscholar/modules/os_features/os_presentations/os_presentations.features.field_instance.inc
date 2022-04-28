<?php
/**
 * @file
 * os_presentations.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function os_presentations_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-presentation-body'
  $field_instances['node-presentation-body'] = array(
    'bundle' => 'presentation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
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
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'os_text_trimmed',
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
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => 0,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'os_wysiwyg',
      'settings' => array(
        'max_rows' => 10,
        'rows' => 5,
        'summary_rows' => 5,
      ),
      'type' => 'os_wysiwyg_expanding_textarea',
      'weight' => 31,
    ),
  );

  // Exported field_instance: 'node-presentation-field_presentation_date'
  $field_instances['node-presentation-field_presentation_date'] = array(
    'bundle' => 'presentation',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 0,
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
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
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
    'field_name' => 'field_presentation_date',
    'label' => 'Presentation Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'display_all_day' => 0,
        'increment' => 15,
        'input_format' => 'M j Y - g:i:sa',
        'input_format_custom' => '',
        'label_position' => 'above',
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-10:+5',
      ),
      'type' => 'date_popup',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'node-presentation-field_presentation_file'
  $field_instances['node-presentation-field_presentation_file'] = array(
    'bundle' => 'presentation',
    'deleted' => 0,
    'description' => '.ppt, .pptx, .pps, .ppsx, and .pdf',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 2,
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
        'label' => 'hidden',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 2,
      ),
      'title' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_presentation_file',
    'label' => 'Presentation Slides',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => '',
      'file_extensions' => 'pdf doc docx odt xls pps ppsx ppt pptx key txt',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => 0,
            'transliterate' => 0,
          ),
          'value' => '[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => 0,
            'transliterate' => 0,
          ),
          'value' => '',
        ),
        'retroactive_update' => 0,
      ),
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          'embed' => 0,
          'oembed' => 0,
          'public' => 'public',
        ),
        'allowed_types' => array(
          'audio' => 0,
          'default' => 0,
          'document' => 0,
          'executable' => 0,
          'html' => 0,
          'image' => 0,
          'video' => 0,
          'ppt' => 'ppt',
          'pptx' => 'pptx',
          'pps' => 'pps',
          'ppsx' => 'ppsx',
          'pdf' => 'pdf',
        ),
        'browser_plugins' => array(
          'library' => 0,
          'media_default--media_browser_1' => 'media_default--media_browser_1',
          'media_embed' => 0,
          'media_internet' => 'media_internet',
          'upload' => 'upload',
        ),
        'progress_indicator' => 'throbber',
      ),
      'type' => 'media_generic',
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'node-presentation-field_presentation_location'
  $field_instances['node-presentation-field_presentation_location'] = array(
    'bundle' => 'presentation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
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
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'title' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_presentation_location',
    'label' => 'Location',
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
      'weight' => 33,
    ),
  );

  // Exported field_instance: 'node-presentation-field_upload'
  $field_instances['node-presentation-field_upload'] = array(
    'bundle' => 'presentation',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'os_files',
        'settings' => array(),
        'type' => 'os_files_image_list',
        'weight' => 1000,
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
    'field_name' => 'field_upload',
    'label' => 'Attach Supplemental Presentation Files (handouts, notes, curricula, etc.)',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'jpg jpeg gif png txt doc docx gz xls xlsx pdf ppt pptx pps odt ods odp tex tgz xml zip sas',
      'filefield_paths' => array(
        'active_updating' => 0,
        'file_name' => array(
          'options' => array(
            'pathauto' => 0,
            'transliterate' => 0,
          ),
          'value' => '[file:ffp-name-only-original].[file:ffp-extension-original]',
        ),
        'file_path' => array(
          'options' => array(
            'pathauto' => 0,
            'transliterate' => 0,
          ),
          'value' => '',
        ),
        'retroactive_update' => 0,
      ),
      'max_filesize' => '128 MB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media_draggable',
      'settings' => array(
        'allow_replace' => 0,
        'allowed_schemes' => array(
          'embed' => 'embed',
          'oembed' => 'oembed',
          'public' => 'public',
        ),
        'allowed_types' => array(
          'audio' => 'audio',
          'default' => 'default',
          'document' => 'document',
          'executable' => 'executable',
          'html' => 'html',
          'image' => 'image',
          'video' => 'video',
        ),
        'browser_plugins' => array(
          'media_default--media_browser_1' => 'media_default--media_browser_1',
          'media_default--media_browser_my_files' => 0,
          'media_internet' => 'media_internet',
          'upload' => 'upload',
        ),
        'droppable_area_text' => 'Drop files here to upload',
        'multiupload' => 0,
        'progress_indicator' => 'throbber',
        'standard_upload' => 1,
        'upload_button_text' => 'Upload',
        'upload_event' => 'auto',
      ),
      'type' => 'media_draggable_file',
      'weight' => 36,
    ),
  );

  // Exported field_instance: 'node-presentation-og_vocabulary'
  $field_instances['node-presentation-og_vocabulary'] = array(
    'bundle' => 'presentation',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'vsite_vocab',
        'settings' => array(
          'concatenate' => TRUE,
        ),
        'type' => 'og_vocab',
        'weight' => 2000,
      ),
      'links' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'no_image_teaser' => array(
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
      'slide_teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'vsite_vocab',
        'settings' => array(
          'concatenate' => 1,
        ),
        'type' => 'og_vocab',
        'weight' => 10,
      ),
      'title' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'og_vocabulary',
    'label' => 'OG vocabulary',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'og_vocab',
      'settings' => array(),
      'type' => 'og_vocab_complex',
      'weight' => 43,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Attach Files');
  t('Body');
  t('Location');
  t('OG vocabulary');
  t('Presentation Date');
  t('Presentation Files');
  t('Upload the file(s) for this presentation.');

  return $field_instances;
}
