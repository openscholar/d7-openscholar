<?php

/**
 * @file
 * Contains OsImporterEventValidator
 */
class OsImporterEventValidator extends OsImporterEntityValidateBase {

  private $dates = array();

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields['field_date__start'] = array(
      'validators' => array(
        array($this, 'validateOsDate'),
      ),
    );

    $fields['field_date__end'] = array(
      'validators' => array(
        array($this, 'validateOsDate'),
      ),
    );

    $fields['registration'] = array(
      'validators' => array(
        array($this, 'validateSignUp'),
      ),
    );

    unset($fields['field_event_registration']);

    return $fields;
  }

  /**
   * Verify the value of the sign up is one of the: true, false, on, off.
   */
  public function validateSignUp($field_name, $value) {
    if (empty($value)) {
      return;
    }
    $value = reset($value);

    $values = array('true', 'false', 'on', 'off');
    if (!in_array(strtolower($value), $values)) {
      $params = array(
        '@value' => $value,
        '@values' => implode(", ", $values),
      );

      $this->setError($field_name, 'The field value (@value) should be one of the following values: @values', $params);
    }
  }

  /**
   * Verify the start is occurring before the end date.
   */
  public function validateOsDate($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    if (empty($value)) {
      return;
    }

    // Store the dates to compare start and end dates.
    $this->dates[$field_name] = reset($value);

    $value = reset($value);

    // Check if start date is greater then the end date.
    if (isset($this->dates['field_date__start']) && $field_name == 'field_date__end') {
      if (strtotime($this->dates['field_date__start']) > strtotime($this->dates['field_date__end'])) {
        $this->setError($field_name, 'The start date/time should not be later than the end date/time.');
      }
    }

    // Check if only end date was supplied.
    if (empty($this->dates['field_date__start']) && $field_name == 'field_date__end') {
      $this->setError($field_name, 'The start date value should not be empty.');
    }

    $formats = array(
      'M j Y g:ia',
      'M j Y',
      'M d Y g:ia',
      'M d Y',
    );

    $this->validateDateFormats($formats, $value, $field_name);
  }
}
