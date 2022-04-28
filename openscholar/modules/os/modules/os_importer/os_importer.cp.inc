<?php
/**
 * @file
 * os_importer.cp.inc
 */

/**
 * Implements hook_cp_settings().
 */
function os_importer_cp_settings() {
  $settings = array();
  $app_info = os_app_info();
  $vsite = vsite_get_vsite();
  if (!$vsite) {
    return array();  
  }
  
  $importer_menu_title = array(
    'biblio' => 'Publication',
    'media_gallery' => 'Media Gallery',
    'faq' => 'FAQ',
    'software_project' => 'Software Project'
  );

  $importer_formats = array(
    'ical' => 'iCal',
    'rss' => 'RSS',
    'csv' => 'CSV'
  );

  $params = array(
    '@wikipedia-url' => 'http://en.wikipedia.org/wiki/Character_encoding',
  );

  $encode_options = array(
    'utf-8' => t('UTF-8'),
    'utf-16' => t('UTF-16'),
    'utf-32' => t('UTF-32'),
    t('MS-Windows character sets') => array(
      'Windows-1250' => t('Central European languages that use Latin script'),
      'Windows-1251' => t('Cyrillic alphabets'),
      'Windows-1252' => t('Western languages'),
      'Windows-1253' => t('Greek'),
      'Windows-1254' => t('Turkish'),
      'Windows-1255' => t('Hebrew'),
      'Windows-1256' => t('Arabic'),
      'Windows-1257' => t('Baltic languages'),
      'Windows-1258' => t('Vietnamese'),
    ),
    t('ISO 8859') => array(
      'ISO 8859-1' => t('Western Europe'),
      'ISO 8859-2' => t('Western and Central Europe'),
      'ISO 8859-9' => t('Western Europe with amended Turkish character set'),
      'ISO 8859-14' => t('Celtic languages (Irish Gaelic, Scottish, Welsh)'),
    ),
  );
  $helpLinkMapping = _os_importer_get_help_link_mapping();

  foreach ($app_info as $type => $info) {
    // Bypassing apps which have RSS feeds as import source.
    if (in_array($type, array('os_blog', 'os_news'))) {
      continue;
    }

    if (empty($info['importers'])) {
      continue;
    }

    if (!is_array($info['nodetypes'])) {
      continue;
    }

    foreach ($info['nodetypes'] as $node_type) {
      $import_format_option = array();

      if (!os_importer_importable_content($node_type)) {
        continue;
      }

      if (isset($info['importers'][$node_type])) {
        foreach ($info['importers'][$node_type] as $import_type => $feature) {
          $import_format_option[$import_type] = $importer_formats[$import_type];
        }
      }

      if (count($import_format_option) > 1) {
        $form_settings = array(
          '#title' => t('Format'),
          '#type' => 'radios',
          '#options' => $import_format_option,
          '#weight' => -6,
          '#default_value' => 'csv',
          '#access' => os_importer_import_access($node_type),
          //'#onclick_callback' => 'template_download_information',
        );
      } else {
        $single_option = array_values($import_format_option);
        $form_settings = array(
          '#type' => 'markup',
          '#markup' => '<label>Format</label>' . (isset($single_option[0]) ? $single_option[0] : '') . ' only',
          '#weight' => -6,
          '#access' => os_importer_import_access($node_type),
        );
      }

      // Import format option should be displayed for all content types except biblio/publication.
      if ($node_type != 'biblio') {
        $settings['os_importer_content_format-' . $node_type] = array(
          'group' => array(
            '#id' => 'os_importer_content_' . $node_type,
            '#title' => isset($importer_menu_title[$node_type]) ? $importer_menu_title[$node_type] : ucfirst($node_type),
            '#weight' => -6,
            '#menu_parent' => 'import',
          ),
          'form' => $form_settings,
        );

        $url_options = array(
          'attributes' => array(
            'class' => 'overlay-exclude'
          ),
          'external' => true,
        );
        $url = $vsite->get_absolute_url ('cp/os-importer/' . $node_type . '/template');
        $template_markup = '<div class="import-help csv-import-fields">' . l(t('Download a template'), $url, $url_options) . '</div>';

        $settings['os_importer_download_template-' . $node_type] = array(
          'group' => array(
            '#id' => 'os_importer_content_' . $node_type,
            '#title' => isset($importer_menu_title[$node_type]) ? $importer_menu_title[$node_type] : ucfirst($node_type),
            '#weight' => -5,
            '#menu_parent' => 'import',
          ),
          'form' => array(
            '#type' => 'container',
            '#markup' => $template_markup,
            '#cid' => 'edit-os-importer-download-template-' . $node_type,
            '#weight' => -5,
            '#access' => os_importer_import_access($node_type),
            '#states' => array(
              'visible' => array(':input[name="os_importer_content_format-' . $node_type . '"]' => array('value' => 'csv')),
            ),
          ),
        );
      }
      if ($node_type == 'biblio') {
        $extension_types = array('csv', 'ris', 'bib', 'tex', 'xml', 'txt', 'mrc', 'enw');
      } else if ($node_type == 'event') {
        $extension_types = array('csv', 'ics');
      } else{
        $extension_types = array('csv');
      }
      $settings['os_importer_file_upload-' . $node_type] = array(
        'form' => array(
          '#type' => 'managed_file',
          '#access' => os_importer_import_access($node_type),
          '#custom_directive' => 'media-browser-field',
          '#custom_directive_parameters' => array(
            'cardinality' => 1,
            'panes' => array('upload', 'library'),
            'hide_helpicon' => true,
            'droppable_text' => t('Drag and drop file here.'),
            'upload_text' => t('Select file to import'),
            'max-filesize' => '500kb',
            'types' => $extension_types,
          ),
          '#class' => array('csv-import-fields', 'os-importer-file-upload-wrapper'),
          '#upload_location' => $purl ? "public://{$purl}/files" : "public://",
          '#upload_validators' => array(
            'file_validate_extensions' => array('csv'),
            'file_validate_size' => array(2*1024*1024),
          ),
        ),
        'group' => array(
          '#id' => 'os_importer_content_' . $node_type,
          '#title' => isset($importer_menu_title[$node_type]) ? $importer_menu_title[$node_type] : ucfirst($node_type),
          '#weight' => 3,
          '#menu_parent' => 'import',
        ),
      );

      $feeds_process_limit = variable_get('feeds_process_limit', FEEDS_PROCESS_LIMIT);

      // Markup field to add help note for media-browser field
      $settings['os_importer_file_upload_note-' . $node_type] = array(
          'group' => array(
            '#id' => 'os_importer_content_' . $node_type,
            '#title' => isset($importer_menu_title[$node_type]) ? $importer_menu_title[$node_type] : ucfirst($node_type),
            '#weight' => 4,
            '#menu_parent' => 'import',
          ),
          'form' => array(
            '#type' => 'container',
            '#markup' => t('Note: Import files with more than %count rows are not permitted. Try creating multiple import files in %count row increments.', array('%count' => $feeds_process_limit)),
            '#weight' => 4,
            '#class' => array('description'),
            '#access' => os_importer_import_access($node_type),
          ),
        );
      // Biblio specific select dropdown.
      if ($node_type == 'biblio') {
        $biblio_import_options = module_invoke_all('biblio_import_options');
        asort($biblio_import_options);
        $biblio_import_options = array_merge(array('0' => 'Select type'), $biblio_import_options);

        $settings['os_importer_filetype-' . $node_type] = array(
            'group' => array(
              '#id' => 'os_importer_content_' . $node_type,
              '#title' => isset($importer_menu_title[$node_type]) ? $importer_menu_title[$node_type] : ucfirst($node_type),
              '#weight' => 5,
              '#menu_parent' => 'import',
            ),
            'form' => array(
              '#title' => t('File Type'),
              '#type' => 'optgroup',
              '#options' => $biblio_import_options,
              '#default_value' => '0',
              '#weight' => 5,
              '#access' => os_importer_import_access($node_type),
            ),
          );
      }

      // Encoding option should be displayed for all content types except biblio/publication.
      if ($node_type != 'biblio') {
          $settings['os_importer_encode-' . $node_type] = array(
            'group' => array(
              '#id' => 'os_importer_content_' . $node_type,
              '#title' => isset($importer_menu_title[$node_type]) ? $importer_menu_title[$node_type] : ucfirst($node_type),
              '#weight' => 5,
              '#menu_parent' => 'import',
            ),
            'form' => array(
              '#title' => t('Encoding'),
              '#type' => 'optgroup',
              '#options' => $encode_options,
              '#default_value' => 'utf-8',
              '#description' => t('Select the encoding of your file. For a full list of encodings you can visit <a href="@wikipedia-url">this</a> Wikipedia page.', $params),
              '#weight' => 5,
              '#class' => array('csv-import-fields'),
              '#access' => os_importer_import_access($node_type),
            ),
          );
      }

      $settings['os_importer_submit-' . $node_type] = array(
        'group' => array(
          '#id' => 'os_importer_content_' . $node_type,
          '#title' => isset($importer_menu_title[$node_type]) ? $importer_menu_title[$node_type] : ucfirst($node_type),
          '#weight' => 6,
          '#menu_parent' => 'import',
          '#group_validate' => 'os_importer_import_group_validate',
        ),
        'form' => array(
          '#value' => t('Submit'),
          '#type' => 'submit',
          '#weight' => 6,
          '#access' => os_importer_import_access($node_type),
        ),
        'rest_submit' => 'os_importer_file_submit',
        'submit_full_request' => TRUE,
      );
      if (isset($helpLinkMapping[$node_type]['path'])) {
        $settings['os_importer_submit-' . $node_type]['group']['#help_link'] = l(t('Learn more about importing'), $helpLinkMapping[$node_type]['path'], array('attributes' => array('target' => '_blank')));
      }
    }
  }

  return $settings;
}

function os_importer_import_group_validate($values) {
  $valid = TRUE;
  // Determining node type from array key.
  foreach($values as $var => $value) {
    if (strpos($var, 'os_importer_submit-') === 0) {
      $part = explode('-', $var);
      $node_type = end($part);
      break;
    }
  }

  // Remove previous warnings and errors.
  drupal_get_messages();

  // For biblio content types validation
  if ($node_type == 'biblio') {
    // If no files have been selected.
    if (empty($values['os_importer_file_upload-' . $node_type])) {
      drupal_set_message(t('The file field is required.'), 'error');
      $valid = FALSE;
    } else if ($values['os_importer_filetype-' . $node_type] == '0') {
      // If no file type have been selected.
      drupal_set_message(t('You did not select the file type.'), 'error');
      $valid = FALSE;
    }
    return $valid;
  }

  // For CSV import files.
  if (empty($values['os_importer_content_format-' . $node_type]) || $values['os_importer_content_format-' . $node_type] == 'csv') {
    // If no files have been selected.
    if (empty($values['os_importer_file_upload-' . $node_type])) {
      drupal_set_message(t('The file field is required.'), 'error');
      $valid = FALSE;
    } else if (!empty($values['os_importer_file_upload-' . $node_type])) {
      $file = file_load($values['os_importer_file_upload-' . $node_type]);
      $part = !empty($file->filename) ? explode('.', $file->filename) : array();
      $extension = end($part);
      if ($extension != 'csv') {
        drupal_set_message(t('Please select a CSV file.'), 'error');
        $valid = FALSE;
      }
      else {

        // check the CSV file has more than 50 rows
        feeds_include_library('ParserCSV.inc', 'ParserCSV');
        ini_set("auto_detect_line_endings", TRUE);
        $parser = new ParserCSV();
        $parser->setDelimiter(',');
        $iterator = new ParserCSVIterator($file->uri);
        $rows = $parser->parse($iterator);
        $row_count = count($rows);
        $feeds_process_limit = variable_get('feeds_process_limit', FEEDS_PROCESS_LIMIT);
        if ($row_count > $feeds_process_limit + 1) {
          drupal_set_message(t('Only %count rows are allowed per import file. Try creating multiple import files in %count row increments', array('%count' => $feeds_process_limit)), 'error');
          $valid = FALSE;
        }

        if ($node_type == 'person') {
          feeds_include_library('ParserCSV.inc', 'ParserCSV');
          ini_set("auto_detect_line_endings", TRUE);
          $parser = new ParserCSV();
          $parser->setDelimiter(',');
          $iterator = new ParserCSVIterator($file->uri);
          $rows = $parser->parse($iterator);
          // Iterate over the rows for field_website validation.
          $valid_url = TRUE;
          foreach ($rows as $key => $row) {
            if ($key == 0) {
              continue;
            }

            // If title is not empty but url is empty.
            if (!empty($row[11]) && empty($row[12])) {
              $valid_url = FALSE;
              break;
            }
            else if (!empty($row[13]) && empty($row[14])) {
              $valid_url = FALSE;
              break;
            }
            else if (!empty($row[15]) && empty($row[16])) {
              $valid_url = FALSE;
              break;
            }
          }

          if (!$valid_url) {
            drupal_set_message(t('The field Websites URL cannot be empty.'), 'error');
            $valid = FALSE;
          }
        }

        if ($node_type == 'event') {
          // Parse uploaded file content with ParserCSV class.
          $file_content = _os_importer_fix_file_content($file);
          file_put_contents($file->uri, $file_content);
          feeds_include_library('ParserCSV.inc', 'ParserCSV');
          $parser = new ParserCSV();
          $parser->setDelimiter(',');
          $iterator = new ParserCSVIterator($file->uri);
          $rows = $parser->parse($iterator);
          if (count($rows) < 2) {
            drupal_set_message(t('The import file does not contain any events.'), 'error');
            $valid = FALSE;
          }
        }
        // Validating the mapping of CSV headers.
        $bundle = os_importer_importable_content($node_type);
        $source = feeds_source($bundle['csv']);
        $config = $source->importer->getConfig();
        // Get the mappings of the current importer.
        $mappings_diff = array();
        $mappings = array();
        foreach ($config['processor']['config']['mappings'] as $mapping) {
          $mappings_diff[] = strtolower($mapping['source']);
          $mappings[] = $mapping['target'];
        }
        $file_content = _os_importer_fix_file_content($file);

        $file_rows = explode("\n", $file_content);
        file_put_contents($file->uri, $file_content);

        feeds_include_library('ParserCSV.inc', 'ParserCSV');
        $parser = new ParserCSV();
        $parser->setDelimiter(',');
        $iterator = new ParserCSVIterator($file->uri);

        $rows = $parser->parse($iterator);

        // Parse file's headers.
        $header = _os_importer_parse_header($parser, $iterator, $mappings);

        // Check what columns are missing from the uploaded CSV file and display
        // message if needed.
        if ($missing = array_diff($mappings_diff, $header)) {
          $params = array('!missing' => theme('item_list', array('items' => $missing)));
          drupal_set_message(t('The following columns are not present in the CSV file: !missing The structure of the your CSV file probably needs to be updated. Please download the template again.', $params), 'error');
          $valid = FALSE;
        }

        // Iterate over the rows for field validation and removing empty rows.
        foreach ($rows as $key => $row) {
          if (!$key) {
            // Check the header for bad vocabularies.
            foreach (array_diff(array_map('strtolower', $row), $mappings_diff) as $vocab) {
              if (!trim($vocab)) {
                drupal_set_message(t('You cannot have a vocabulary without a name.'), 'error');
                $valid = FALSE;
                continue;
              }
              if (preg_match('@[^a-z0-9_]+@', $vocab) && !str_replace(array(' ', '.', '&', '-'), '', strtolower($vocab))) {
                drupal_set_message(t('The vocabularies you want to import doesn\'t contain any text.'), 'error');
                $valid = FALSE;
                continue;
              }
            }
            continue;
          }

          // Check if row is empty.
          if (!implode("", $row)) {
            unset($rows[$key]);
            continue;
          }

          /** @var OsImporterEntityValidateBase $handler */
          if (!$handler = entity_validator_get_validator_handler('node', $node_type)) {
            continue;
          }
          $handler->rest = FALSE;
          $node = array(
            'type' => $node_type,
          );
          $node = entity_create('node', $node);
          $wrapper = entity_metadata_wrapper('node', $node);
          // Build the form values - break down the row.

          foreach ($row as $delta => $value) {
            // Check only the fields found in the mappings array. Columns after the
            // last mapping column are used to create taxonomy terms.
            // @see os_importer_feeds_presave().
            if ($delta >= count($mappings)) {
              break;
            }

            // Fail in case mapping does not exist.
            if (!isset($mappings[$delta])) {
              $params = array(
                '@delta' => $delta,
                '@value' => $value,
              );
              drupal_set_message(t('Mapping of CSV file is incorrect for delta @delta with value @value', $params), 'error');
              $valid = FALSE;
            }
            $field = $mappings[$delta];

            $field = str_replace(':', '__', $field);

            // Do not validate file fields.
            if ($field == 'file') {
              continue;
            }

            if ($field == 'body') {
              $value = array('value' => $value);
            }

            $info = $wrapper->{$field}->info();

            if (!is_array($value) && $info['type'] == 'list<text>') {
              $value = array($value);
            }
            elseif (!is_array($value) && $info['type'] == 'text_formatted') {
              $value = array('value' => $value);
            }
            elseif (empty($value) && in_array($info['type'], array('integer', 'decimal'))) {
              $value = 0;
            }
            elseif ($value && $info['type'] == 'integer') {
              $value = intval($value);
            }
            elseif ($field == 'created' && is_string($value)) {
              $orig_value_created = $value;
              $value = strtotime($value);
              // if strtotime() output is blank, then date format is not correct, displaying error message.
              if(empty($value) || ($value < 0)) {
                drupal_set_message(t('Invalid date value entered for the field "!field" with value "!value".', array('!field' => $mappings_diff[$delta], '!value' => $orig_value_created)), 'error');
                $valid = FALSE;
              }
            }

            try {
              $wrapper->{$field}->set($value);
            }
            catch (EntityMetadataWrapperException $e) {
              drupal_set_message(t('Settings the value of field "!field" with value "!value" failed.', array('!field' => $field, '!value' => $orig_value)));
              $valid = FALSE;
            }
          }

          if ($handler->validate($node, TRUE) === FALSE) {
            $errors = $handler->getErrors(FALSE);

            $return = array();

            // Replace the messages' placeholders with the CSV column name for better
            // validation error messages.
            foreach ($errors as $field => $field_errors) {
              foreach ($field_errors as $error) {
                if (!isset($error['params']['@field'])) {
                  continue;
                }

                // We take the label of the field (either the original label, or if
                // it's a property we added, then the one defined in
                // os_importer_entity_property_info_alter().
                $field_info = $wrapper->{$field}->info();
                $field_label = $field_info['label'];
                $error['params']['@field'] = $field_label;

                $return[] = format_string($error['message'], $error['params']) . '</br>';
              }
            }
            $return = array(
              '!errors' => implode("\n\r", $return),
            );
            // Remove previous warnings and errors.
            drupal_set_message(t('Field validation errors:</br>!errors', $return), 'error');
            $valid = FALSE;
          }
        }
      }
    }
  } else if ($values['os_importer_content_format-' . $node_type] == 'ical') {
    // If no files have been selected.
    if (empty($values['os_importer_file_upload-' . $node_type])) {
      drupal_set_message('The file field is required.', 'error');
      $valid = FALSE;
    } else if (!empty($values['os_importer_file_upload-' . $node_type])) {
      $file = file_load($values['os_importer_file_upload-' . $node_type]);
      $part = !empty($file->filename) ? explode('.', $file->filename) : array();
      $extension = end($part);
      if ($extension != 'ics') {
        drupal_set_message('Please select an iCal file.', 'error');
        $valid = FALSE;
      } else {
        // iCal file validation.
        // Parsing ics file content with iCalcreator class.
        $file_content = file_get_contents($file->uri);
        $config = array(
          'unique_id' => '',
          'filename' => drupal_realpath($file->uri),
        );

        $creator_path = libraries_get_path('iCalcreator');
        require_once($creator_path . '/iCalcreator.class.php');
        $calendar = new vcalendar($config);
        if (!$calendar->parse($file_content)) {
          drupal_set_message(t('The import file does not contain any events.'), 'error');
          $valid = FALSE;
        }
      }
    }
  }
  return $valid;
}

/**
 * Submit handler for os_importer_submit button for CSV/iCal import.
 */
function os_importer_file_submit($request) {
  global $user, $language;
  foreach($request as $var => $value) {
    if (strpos($var, 'os_importer_submit-') === 0) {
      $part = explode('-', $var);
      $node_type = end($part);
    }
  }
  $import_file_fid = $request['os_importer_file_upload-' . $node_type];
  $import_file = file_load($import_file_fid);
  $part = !empty($import_file->uri) ? explode('.', $import_file->uri) : array();
  $extension = end($part);
  if ($extension == 'ics') {
    $extension = 'ical';
  }

  if ($node_type != 'biblio' && in_array($extension, array('csv', 'ical'))) {
    $bundle = os_importer_importable_content($node_type);
    $source = feeds_source($bundle[$extension]);
    $fetcher_config = $source->getConfigFor($source->importer->fetcher);
    $fetcher_config['source'] = $import_file->uri;
    $fetcher_config['file'] = $import_file;
    $fetcher_config['encode'] = isset($request['os_importer_encode-' . $node_type])? $request['os_importer_encode-' . $node_type] : 'csv';
    $source->setConfigFor($source->importer->fetcher, $fetcher_config);
    $source->save();
    $source->startImport();
  } else if ($node_type == 'biblio') {
    $userid = $user->uid;
    $filetype = $request['os_importer_filetype-biblio'];
    $session_id = microtime();
    $context = array();
    $terms = array();
    module_load_include('inc', 'biblio', 'includes/biblio.import.export');
    biblio_import($import_file, $filetype, $userid, $terms, FALSE, $session_id, $context);
    biblio_import_finalize(TRUE, $context['results']);
  }
  file_save($import_file);
}
