<?php

/**
 * @file
 * Contains OsImporterMediaGalleryValidator
 */
class OsImporterMediaGalleryValidator extends OsImporterEntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields['media_gallery_rows'] = $fields['media_gallery_columns'] = array(
      'validators' => array(
        array($this, 'validateRowsColumns'),
      ),
    );

    return $fields;
  }

  /**
   * The rows and columns should be positive.
   */
  public function validateRowsColumns($field_name, $value) {
    if ($value < 0) {
      $params['@value'] = $value;
      $this->setError($field_name, 'The field @field should be positive. The given value is: @value.', $params);
    }
  }
}
