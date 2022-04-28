<?php

/**
 * @file
 * Contains OsImporterPresentationValidator
 */
/**
 * required title
 * validate date
 */
class OsImporterPresentationValidator extends OsImporterEntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields['field_presentation_date__start'] = array(
      'validators' => array(
        array($this, 'validateOsDate'),
      ),
    );

    return $fields;
  }

  /**
   * Overrides OsImporterEntityValidateBase::validateOsDate() to allow empty
   * date value.
   */
  public function validateOsDate($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    $value = reset($value);

    // We allow an empty date value for this content type.
    if (empty($value)) {
      return;
    }

    // Validate the date format for the start and end date.
    $this->validateDateFormats(array('M j Y', 'M d Y'), $value, $field_name);
  }
}
