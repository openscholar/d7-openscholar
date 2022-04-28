<?php
/**
 * @file
 * Convert a year entry, yyyy, to a full date in the form dd/mm/yyyy.
 */

$plugin = array(
  'form' => 'os_importer_year_to_date_form',
  'callback' => 'os_importer_year_to_date_convert',
  'name' => t('Convert year to full date'),
  'category' => 'Date/time',
);

/**
 * Tamper form.
 */
function os_importer_year_to_date_form($importer, $element_key, $settings) {
  $form = array();
  $form['help'] = array(
    '#markup' => t('Convert the value of a date given in years to a full date.'),
  );
  return $form;
}

/**
 * Tamper callback.
 *
 * In order for a date given in years to be processed correctly we need to
 * convert the date in years to a full date, e.g. 2014 would be converted to
 * 1/1/2014.
 */
function os_importer_year_to_date_convert($result, $item_key, $element_key, &$field, $settings, $source) {
  if (is_array($field)) {
    foreach ($field as $key => $value) {
      $field[$key] = $field[$key] ? '1/1/' . $field[$key] : '';
    }
  }
  else {
    $field = $field ? '1/1/' . $field : '';
  }
}
