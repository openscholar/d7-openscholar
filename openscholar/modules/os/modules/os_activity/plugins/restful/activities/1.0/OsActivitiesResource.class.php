<?php

/**
 * @file
 * Contains OsActivitiesResource.
 */

class OsActivitiesResource extends RestfulEntityBaseMultipleBundles {

  /**
   * Define the bundles to exposed to the API.
   *
   * @var array
   *  Array keyed by bundle machine, and the RESTful resource as the value.
   */
  protected $bundles = array(
    'os_create_node' => 'activities',
    'os_edit_node' => 'activities',
  );

  /**
   * Overrides RestfulEntityBase::getList().
   */
  public function getList($request = NULL, stdClass $account = NULL) {
    // Allow settings to be overridden.
    $defaults = array(
      'filter_vsite' => TRUE,
      'view_mode' => 'full',
      'range' => 10,
      'langcode' => NULL,
      'page' => NULL,
    );

    $options = array();
    foreach (array_keys($defaults) as $key) {
      if (isset($_GET[$key])) {
        $options[] = $_GET[$key];
      }
    }

    $options += $defaults;

    $entity_type = $this->entityType;
    $result = $this
      ->getQueryForList()
      ->execute();

    if (empty($result[$entity_type])) {
      return;
    }

    $ids = array_keys($result[$entity_type]);

    // Pre-load all entities.
    $messages = entity_load($entity_type, $ids);

    // Start building output.
    $return['success'] = TRUE;

    $return = array(
      'success' => TRUE,
      'messages' => array(),
    );

    $build = entity_view('message', $messages, $options['view_mode'], $options['langcode'], $options['page']);
    $key = 'message__message_text__0';

    foreach ($messages as $mid => $message) {
      // Add the rendered HTML snippet to the entity object.
      $messages[$mid]->markup = $build['message'][$mid][$key]['#markup'];
      // Use timestamp + message ID, as array key for easier cross-install
      // aggregation.
      // Example: "1389220285.285411"
      $return['messages']["{$messages[$mid]->timestamp}.{$mid}"] = $messages[$mid];
    }

    return $return;
  }

  /**
   * Overrides RestfulEntityBase::getQueryForList().
   */
  public function getQueryForList() {
    $query = parent::getQueryForList();
    $query->entityCondition('bundle', array_keys($this->getBundles()), 'IN');
    if (module_exists('vsite_access')) {
      $query->fieldCondition('field_private_message', 'value', VSITE_ACCESS_PUBLIC);
    }

    list($offset, $range) = $this->parseRequestForListPagination();
    $query->range($offset, $range);
    $query->propertyOrderBy('timestamp', 'DESC');
    $query->propertyOrderBy('mid', 'DESC');

    // Only continues to filter by current space if we can load the vsite.
    // @FIXME This condition currently returns empty results when not site-wide...
    $filter_vsite = isset($request['filter_vsite']) ? $request['filter_vsite'] : TRUE;
    if (module_exists('vsite') && $filter_vsite) {
      $space = spaces_get_space();
      if (is_object($space) && is_numeric($space->id)) {
        $query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $space->id);
      }
    }

    return $query;
  }
}
