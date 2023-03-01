<?php

/**
 * @file
 * Contains \OsRestfulSpaces
 */
abstract class OsRestfulSpaces extends \OsRestfulDataProvider {

  /**
   * @var vsite
   * The space object.
   */
  protected $space;

  /**
   * @var string
   * Object type: context, boxes etc. etc.
   */
  protected $objectType = '';

  /**
   * @var EntityMetadataWrapper
   *
   * The group wrapper object.
   */
  protected $group;

  /**
   * Overrides \RestfulDataProviderEFQ::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        \RestfulInterface::GET => 'getSpace',
        \RestfulInterface::POST => 'createSpace',
        \RestfulInterface::PUT => 'updateSpace',
        \RestFulInterface::DELETE => 'deleteSpace',
      ),
      '^.*$' => array(
        \RestfulInterface::GET => 'getSpace',
      ),
    );
  }

  abstract public function createSpace();
  abstract public function updateSpace();
  abstract public function deleteSpace();

  /**
   * {@inheritdoc}
   */
  public function access() {
    return $this->checkGroupAccess();
  }

  /**
   * {@inheritdoc}
   */
  public function publicFieldsInfo() {
    return $this->simpleFieldsInfo(array('type', 'id', 'object_id', 'object_type', 'value', 'changed'));
  }

  /**
   * Simple fields mapping.
   *
   * @param array $fields
   *   The list of the properties.
   * @return array
   *   List of the schema fields.
   */
  protected function simpleFieldsInfo($fields = array()) {
    $return = array();
    foreach ($fields as $field) {
      $return[$field] = array('property' => $field);
    }

    return $return;
  }

  /**
   * Overriding the query list filter method: Exposing only boxes.
   */
  protected function queryForListFilter(\SelectQuery $query) {
    parent::queryForListFilter($query);

    if ($this->objectType) {
      $query->condition('object_type', $this->objectType);
    }

    // To make filter using multiple vsite ids.
    if (!empty($this->request['vsiteid'])) {
      $query->condition('id', $this->request['vsiteid'], 'IN');
    }

    if (!empty($this->request['vsite'])) {
      $query->condition('id', $_GET['vsite']);
      if (!empty($this->request['widget_id'])) {
        $delta = 'boxes-' . $this->request['widget_id'];
        $length = strlen($delta);
        $string = "s:{$length}:\"{$delta}\"";
        $query->condition('value', '%' . $string . '%', 'LIKE');
      }
    }

    if (!empty($this->request['delta'])) {
      $query->condition('object_id', $this->request['delta']);
    }

    if (!empty($this->request['plugin_key'])) {
      $plugin_type = $this->request['plugin_key'];
      $length = strlen($plugin_type);
      $string = "s:{$length}:\"{$plugin_type}\"";
      $query->condition('value', '%' . $string . '%', 'LIKE');
    }
    if (!empty($this->request['changed'])) {
      $datetime = date('Y-m-d H:i:s', $this->request['changed']);
      $query->condition('changed', $datetime, '>=');
    }
  }

  /**
   * Throwing exception easily.
   * @param $message
   *   The exception message.
   * @throws RestfulBadRequestException
   */
  public function throwException($message) {
    throw new \RestfulBadRequestException($message);
  }

  /**
   * Verify the user's request has access CRUD in the current group.
   */
  public function checkGroupAccess() {
    $this->getObject();
    $vsite_id = !empty($this->object->vsite) ? $this->object->vsite : $_GET['vsite'];
    if (!$this->space = spaces_load('og', $vsite_id)) {
      // No vsite context.
      $this->throwException('The vsite ID is missing.');
    }

    // Set up the space.
    spaces_set_space($this->space);

    $this->group = entity_metadata_wrapper('node', $this->space->og);

    if (user_access('administer group', $this->getAccount())) {
      return TRUE;
    }
    return TRUE;
  }

  /**
   * un-serialize the value object.
   */
  public function mapDbRowToPublicFields($row) {
    $row->value = unserialize($row->value);
    $request = $this->getRequest();
    if (!empty($request['widget_id'])) {
      if(!empty($row->value['blocks'])) {
        foreach ($row->value['blocks'] as $key=>$boxes) {
          if($boxes['delta'] != $request['widget_id']) {
            unset($row->value['blocks'][$key]);
          }
        }
        return parent::mapDbRowToPublicFields($row);
      }
    }

    $options = isset($row->value->options) ? $row->value->options : [];
    $plugin_key = isset($request['plugin_key']) ? strip_tags($request['plugin_key']) : false;

    if ($plugin_key && $plugin_key == 'os_boxes_manual_list') {
      $nodes = $options['nodes'];
      $nids = array_keys($nodes);

      foreach ($nids as $nid) {
        $node = node_load($nid);
        $nodes[$nid]['type'] = $node->type;
      }

      $row->value->options['nodes'] = $nodes;
    }

    if (!empty($options)) {
      $fid = isset($options['fid']) ? $options['fid'] : 0;
      $fid = is_array($fid) ? $fid['fid'] : $fid;

      // Attach complete file info if valid fid (to be used by D8 migrations).
      if ($fid > 0 && $file_info = file_load($fid)) {
        $row->value->options['fid_file_info'] = (array) $file_info;
      }

    }

    return parent::mapDbRowToPublicFields($row);
  }

  /**
   * Override the list method in order to return a specific delta from the
   * space override.
   */
  public function getSpace() {
    $list = parent::index();

    $request = $this->getRequest();

    if (!empty($request['type']) && !empty($list[0]['value'][$request['type']])) {
      // We need a sub value from the space.
      $sub_value = $list[0]['value'][$request['type']];

      if (!empty($request['delta']) && !empty($sub_value[$request['delta']])) {
        // We need a specif delta from the sub value.
        return $sub_value[$request['delta']];
      }
      return $sub_value;
    }

    return $list;
  }

}
