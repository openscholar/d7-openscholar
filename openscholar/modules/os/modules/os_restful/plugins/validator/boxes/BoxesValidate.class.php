<?php

class BoxesValidate extends OsObjectValidate {

  /**
   * Overrides ObjectValidateBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    FieldsInfo::setFieldInfo($fields['widget'], $this)
      ->setProperty('widget')
      ->addCallback('validateWidget');

    FieldsInfo::setFieldInfo($fields['options'], $this)
      ->setProperty('options')
      ->addCallback('validateOptions');

    FieldsInfo::setFieldInfo($fields['delta'], $this)
      ->setProperty('delta')
      ->addCallback('validateDelta');

    return $fields;
  }

  /**
   * Validate the option attribute,
   */
  public function validateOptions($property, $value, $object) {
    if (empty($value['description'])) {
      $this->setError($property, 'The description is missing in the widgets settings.');
    }
  }

  /**
   * Verify we got delta when we updating a box.
   */
  public function validateDelta($property, $value, $object) {
    if (!property_exists($object, 'new')) {
      return;
    }

    if ($object->new) {
      return;
    }

    if (empty($object->delta)) {
      $this->setError($property, 'You need to pass a delta of existing box.');
    }
  }

  /**
   * Verifying we got a widget when creating box.
   */
  public function validateWidget($property, $value, $object) {
    if (!property_exists($object, 'new')) {
      return;
    }

    if (!$object->new) {
      return;
    }

    if (empty($object->widget)) {
      $this->setError($property, 'You need to pass a widget.');
    }
  }
}
