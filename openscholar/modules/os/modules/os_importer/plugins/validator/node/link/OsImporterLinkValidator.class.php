<?php

/**
 * @file
 * Contains OsImporterLinkValidator
 */
class OsImporterLinkValidator extends OsImporterEntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields[$this->rest ? 'field_links_link' : 'field_links_link__url'] = array(
      'validators' => array(
        array($this, 'validatorUrlNotEmpty'),
      ),
    );

    return $fields;
  }
}
