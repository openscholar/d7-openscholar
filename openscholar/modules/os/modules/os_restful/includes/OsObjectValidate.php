<?php

abstract class OsObjectValidate extends ObjectValidateBase {

  /**
   * Overrides ObjectValidateBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $fields = array();

    $fields['vsite'] = array(
      'property' => 'id',
      'validators' => array(
        array($this, 'validateVsite'),
      ),
    );

    return $fields;
  }

  /**
   * Validate the user passed a vsite.
   */
  public function validateVsite($property, $value, $object) {
    if (empty($object->vsite)) {
      $this->setError('vsite', 'You need to pass vsite ID.');
    }
  }
}
