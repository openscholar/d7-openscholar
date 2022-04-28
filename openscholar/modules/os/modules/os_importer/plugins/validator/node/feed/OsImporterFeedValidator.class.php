<?php

/**
 * @file
 * Contains OsImporterFeedValidator
 */
class OsImporterFeedValidator extends OsImporterEntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $url = $this->rest ? 'field_url' : 'field_url__url';
    $fields[$url] = array(
      'validators' => array(
        array($this, 'validatorUrlNotEmpty'),
      ),
    );

    return $fields;
  }
}
