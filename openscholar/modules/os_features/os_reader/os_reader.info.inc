<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class OsReaderMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['description'] = array(
      'label' => t('Description'),
      'type' => 'text',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
      'description' => t('The feed items text.'),
      'schema field' => 'description',
    );

    // Notify Drupal that this field is not just an integer but a reference to a
    // node, so we can use it as a relationship in Views.
    $properties['importer_nid']['type'] = 'node';

    return $info;
  }
}
