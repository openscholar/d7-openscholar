<?php
/**
 * @file
 * Set the given date as the site timezone.
 */

$plugin = array(
  'name' => t('Adjust date to timezone'),
  'callback' => 'os_importer_adjust_date_to_timezone',
  'form' => 'os_importer_adjust_date_to_timezone_form',

  'category' => 'Date/time',
);

function os_importer_adjust_date_to_timezone_form() {
  return array();
}

/**
 * Tamper callback.
 */
function os_importer_adjust_date_to_timezone($result, $item_key, $element_key, &$field, $settings, $source) {
  $timezone = variable_get('date_default_timezone');
  $datetime = new DateTime($timezone);
  $datetime->setTimestamp(strtotime($field));
  $field = $datetime->getTimestamp();
}
