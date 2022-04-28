<?php

function os_files_settings_form() {
  $form = array();

  $value = '';
  $mappings = variable_get('os_files_extension_map', array());
  foreach ($mappings as $ext => $type) {
    $value .= "$ext: $type\r\n";
  }
  $form['os_files_extension_mappings'] = array(
    '#type' => 'textarea',
    '#title' => t('Map Extensions to File Types'),
    '#default_value' => $value,
    '#description' => t('Set file types for extensions with the following pattern: ext: file_type. <br>
    Put each mapping on a new line. Ex.<br>
      txt: document<br>
      pdf: document<br>
      Most common file extensions are already handled. This form is for esoteric extensions that most sites may never see.')
  );

  $form = system_settings_form($form);
  $form['#submit'][] ='os_files_settings_form_submit';

  return $form;
}

function os_files_settings_form_submit($form, $form_state) {
  $mappings = array();
  $types = file_entity_type_get_names();
  $lines = explode("\n", $form_state['values']['os_files_extension_mappings']);
  foreach ($lines as $l) {
    $l = trim($l);
    $data = explode(': ', $l);
    if (isset($types[$data[1]])) {
      $mappings[$data[0]] = $data[1];
    }
  }

  variable_set('os_files_extension_map', $mappings);
}