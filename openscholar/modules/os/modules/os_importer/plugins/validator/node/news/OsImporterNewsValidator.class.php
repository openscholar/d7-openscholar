<?php

/**
 * @file
 * Contains OsImporterNewsValidator
 */
class OsImporterNewsValidator extends OsImporterEntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields[$this->rest ? 'field_news_date' : 'field_news_date__start'] = array(
      'validators' => array(
        array($this, 'isNotEmpty'),
        array($this, 'validateOsDate'),
      ),
    );

    $fields['field_photo'] = array(
      'validators' => array(
        array($this, 'validatorNewsPhoto'),
      ),
    );

    return $fields;
  }

  /**
   * Validating the image is in 220X220.
   */
  public function validatorNewsPhoto($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    // Allow empty photo.
    if (empty($value)) {
      return;
    }

    $this->validatorPhoto($field_name, $value, 250, 250);
  }

  public function isValidValue($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    $info = $property_wrapper->Info();
    if ($info['type'] == 'field_item_image') {
      $value = array($value);
    }
    parent::isValidValue($field_name, $value, $wrapper, $property_wrapper);
  }
}
