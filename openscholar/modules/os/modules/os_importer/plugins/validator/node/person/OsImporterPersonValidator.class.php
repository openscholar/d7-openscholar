<?php

/**
 * @file
 * Contains OsImporterPersonValidator
 */
class OsImporterPersonValidator extends OsImporterEntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields['person_photo'] = array(
      'validators' => array(
        array($this, 'validatorPersonPhoto'),
      ),
    );

    $fields['title'] = array();

    return $fields;
  }

  /**
   * Validating the image is in 220X220.
   */
  public function validatorPersonPhoto($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    // Allow empty photo.
    if (empty($value)) {
      return;
    }

    $this->validatorPhoto($field_name, reset($value), 220, 220);
  }

  /**
   * Overriding parent:isNotEmpty().
   *
   * The person node don't need title by default since the title is generated
   * from the first\last\middle name and we already verifying the titles.
   */
  public function isNotEmpty($field_name, $value, EntityMetadataWrapper $wrapper, EntityMetadataWrapper $property_wrapper) {
    if ($field_name == 'title') {
      return;
    }

    parent::isNotEmpty($field_name, $value,  $wrapper, $property_wrapper);
  }

}
