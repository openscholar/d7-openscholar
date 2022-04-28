<?php

/**
 * @file
 * Contains OsImporterClassValidator
 */
class OsImporterClassValidator extends OsImporterEntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields['field_semester'] = array(
      'validators' => array(
        array($this, 'validationSemester'),
      ),
    );

    $fields['field_offered_year__start'] = array(
      'validators' => array(
        array($this, 'validateOfferedYear'),
      ),
    );

    return $fields;
  }

  /**
   * Validate the semester field.
   */
  function validationSemester($field_name_name, $value) {
    // Allow empty value for the semester.
    if (empty($value)) {
      return;
    }

    $value = trim($value);

    $info = field_info_field($field_name_name);

    // Validate the semester.
    $allowed_values = $info['settings']['allowed_values'] + array('N/A' => 'N/A');
    if (!in_array(strtolower($value), array_map('strtolower', $allowed_values))) {
      $params = array(
        '@allowed-values' => implode(', ', $allowed_values),
        '@value' => $value,
      );

      $this->setError($field_name_name, 'The given value (@value) is not a valid value for semester, and it should be one of the followings (@allowed-values)', $params);
    }
  }

  /**
   * Preprocess the offered year and preprocess the form on the way.
   */
  function validateOfferedYear($field_name, $value) {
    // Allow empty value for the year.
    if (empty($value)) {
      return;
    }

    $value = reset($value);

    if (!is_numeric($value) || (is_numeric($value) && $value > 9999)) {
      $params = array(
        '@value' => $value,
      );

      $this->setError($field_name, 'The value for the year field is not valid value(@value). The value should be a year.', $params);
    }
  }
}
