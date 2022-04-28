<?php
/**
 * @file
 */

$plugin = array(
  'form' => 'os_importer_pathauto_cleanstring_form',
  'callback' => 'os_importer_pathauto_cleanstring_convert',
  'name' => t('Clean string with pathauto'),
  'category' => 'Text',
);

/**
 * Tamper form.
 */
function os_importer_pathauto_cleanstring_form($importer, $element_key, $settings) {
  $form = array();
  $form['help'] = array(
    '#markup' => t('Clean the input string with pathauto_cleanstring function.'),
  );
  return $form;
}

/**
 * Tamper callback.
 */
function os_importer_pathauto_cleanstring_convert($result, $item_key, $element_key, &$field, $settings, $source) {
  if (!module_exists('pathauto')) {
    return;
  }
  module_load_include('inc', 'pathauto');
  if (is_array($field)) {
    foreach ($field as $key => $value) {
      $field[$key] = pathauto_cleanstring($field[$key]);
    }
  }
  else {
    $field = pathauto_cleanstring($field);
  }
}
