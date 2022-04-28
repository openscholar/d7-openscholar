<?php

class OsOgVocab extends \RestfulEntityBase {

  /**
   * {@inheritdoc}
   */
  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();

    $fields['vid'] = array(
      'property' => 'vid',
    );

    $fields['entity_type'] = array(
      'property' => 'entity_type',
    );

    $fields['bundle'] = array(
      'property' => 'bundle',
    );

    $fields['field_name'] = array(
      'property' => 'field_name',
    );

    $fields['settings'] = array(
      'property' => 'settings',
    );

    unset($fields['self'], $fields['label']);

    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  protected function checkEntityAccess($op, $entity_type, $entity) {
    if ($this->getMethod() == \RestfulBase::GET) {
      return TRUE;
    }

    $vid = $this->getMethod() == \RestfulBase::POST ? $this->request['vid'] : $entity->vid;
    if (!$relation = og_vocab_relation_get($vid)) {
      throw new \RestfulBadRequestException('The vocabulary is not relate to any group.');
    }

    spaces_set_space(vsite_get_vsite($relation->gid));
    $this->getAccount();

    $permissions = array(
      \RestfulBase::POST => 'administer taxonomy',
      \RestfulBase::PATCH => 'edit terms',
      \RestfulBase::PUT => 'edit terms',
      \RestfulBase::DELETE => 'delete terms',
    );

    return vsite_og_user_access($permissions[$op]);
  }

  /**
   * {@inheritdoc}
   */
  protected function checkPropertyAccess($op, $public_field_name, EntityMetadataWrapper $property_wrapper, EntityMetadataWrapper $wrapper) {
    return TRUE;
  }

  public function entityValidate(\EntityMetadataWrapper $wrapper) {
    $query = new EntityFieldQuery();
    $results = $query
      ->entityCondition('entity_type', 'og_vocab')
      ->propertyCondition('entity_type', $this->request['entity_type'])
      ->propertyCondition('bundle', $this->request['bundle'])
      ->propertyCondition('vid', $this->request['vid'])
      ->execute();

    if (!empty($results['og_vocab'])) {
      $params = array(
        '@entity_type' => $this->request['entity_type'],
        '@bundle' => $this->request['bundle'],
      );
      throw new \RestfulBadRequestException(format_string('OG vocabulary already exists for @entity_type:@bundle', $params));
    }
  }

  public function entityPreSave(\EntityMetadataWrapper $entity) {
    $settings = $entity->settings->value();
    $settings += array(
      'required' => FALSE,
      'widget_type' => 'options_select',
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    );
    $entity->settings->set($settings);
  }

}
