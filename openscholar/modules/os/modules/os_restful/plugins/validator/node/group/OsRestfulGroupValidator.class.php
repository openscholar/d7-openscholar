<?php

class OsRestfulGroupValidator extends EntityValidateBase {

  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    // Remove fields we don't to handle for now.
    unset($fields['group_access'], $fields['og_roles_permissions']);

    FieldsInfo::setFieldInfo($fields['purl'], $this)
      ->setRequired()
      ->setProperty('purl')
      ->addCallback('validateSinglePurl');

    return $fields;
  }

  /**
   * Validating the purl isn't duplicated.
   */
  public function validateSinglePurl($field_name, $value, $wrapper, $property_wrapper) {
    $purl = $wrapper->domain->value();

    $modifier = array(
      'provider' => 'spaces_og',
      'value' => $purl,
      'id' => $wrapper->getIdentifier(),
    );

    if (strlen($modifier['value']) < 3) {
      $this->setError('domain', 'The site address should be at least 3 characters.');
    }

    if ($this->countPurlInstances($purl)) {
      $this->setError('domain', 'A site with this address already exists.');
    }

    module_load_include('form.inc', 'vsite_register');
    if (!valid_url($modifier['value']) || !_vsite_register_valid_url($modifier['value'])) {
      $this->setError('domain', 'The site address has invalid characters.');
    }
  }

  /**
   * Count the instances of a given purl.
   */
  private function countPurlInstances($value) {
    $query = db_select('purl', 'p');
    return $query
      ->fields('p')
      ->condition('value', $value)
      ->execute()
      ->rowCount();
  }

}
