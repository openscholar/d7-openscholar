<?php

class OsNodeRestfulBase extends RestfulEntityBaseNode {

  /**
   * Overrides \RestfulDataProviderEFQ::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        \RestfulInterface::GET => 'getList',
        \RestfulInterface::HEAD => 'getList',
        \RestfulInterface::POST => 'createEntity',
        \RestfulInterface::DELETE => 'deleteEntity',
      ),
      '^(\d+,)*\d+$' => array(
        \RestfulInterface::GET => 'viewEntities',
        \RestfulInterface::HEAD => 'viewEntities',
        \RestfulInterface::PUT => 'putEntity',
        \RestfulInterface::PATCH => 'patchEntity',
        \RestfulInterface::DELETE => 'deleteEntity',
      ),
    );
  }

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    if ($this->getBundle()) {
      $public_fields['vsite'] = array(
        'property' => OG_AUDIENCE_FIELD,
        'process_callbacks' => array(
          array($this, 'vsiteFieldDisplay'),
        ),
      );
    }

    $public_fields['body'] = array(
      'property' => 'body',
      'sub_property' => 'value',
    );

    if (field_info_instance($this->getEntityType(), 'field_upload', $this->getBundle())) {
      $public_fields['files'] = array(
        'property' => 'field_upload',
        'process_callbacks' => array(
          array($this, 'fileFieldDisplay'),
        ),
      );
    }

    $public_fields['uid'] = array(
      'callback' => array($this, 'getUid'),
    );
    $public_fields['created'] = [
      'callback' => array($this, 'getCreated'),
    ];

    $public_fields['terms'] = array(
      'property' => 'og_vocabulary',
    );

    $public_fields['path'] = array(
      'property' => 'path',
    );

    $public_fields['sticky'] = array(
      'property' => 'sticky',
    );

    return $public_fields;
  }

  protected function checkEntityAccess($op, $entity_type, $entity) {
    $request = $this->getRequest();

    if ($request['vsite']) {
      spaces_set_space(spaces_load('og', $request['vsite']));
    }

    if (empty($entity->nid)) {
      // This is still a new node. Skip.
      return;
    }

    if ($is_group = og_is_group($entity_type, $entity)) {
      $group = $entity;
    }
    else {
      $wrapper = entity_metadata_wrapper('node', $entity);
      $group = $wrapper->{OG_AUDIENCE_FIELD}->get(0)->value();
    }

    if (empty($request['vsite'])) {
      spaces_set_space(spaces_load('og', $group->nid));
    }

    $manager = og_user_access('node', $group->nid, 'administer users', $this->getAccount());

    if ($is_group) {
      // In addition to the node access check, we need to see if the user can
      // manage groups.
      return $manager && !vsite_access_node_access($group, 'view', $this->getAccount()) == NODE_ACCESS_DENY;
    }
    else {
      $app = os_get_app_by_bundle($entity->type);
      $space = spaces_get_space();
      $application_settings = ($space->controllers->variable) ? $space->controllers->variable->get('spaces_features') : OS_PUBLIC_APP;

      switch ($application_settings[$app]) {
        case OS_DISABLED_APP:
          return FALSE;

        case OS_PRIVATE_APP:
          return og_is_member('node', $group->nid, 'user', $this->getAccount()) && parent::checkEntityAccess($op, $entity_type, $entity);

        default:
        case OS_PUBLIC_APP:
          return parent::checkEntityAccess($op, $entity_type, $entity);
      }
    }
  }

  /**
   * Display the id and the title of the group.
   */
  public function vsiteFieldDisplay($value) {
    return array('title' => $value[0]->title, 'id' => $value[0]->nid);
  }

  /**
   * Process the time stamp to a text.
   */
  public function dateProcess($value) {
    return format_date($value[0]);
  }

  /**
   * Process the file field.
   */
  public function fileFieldDisplay($files) {
    $return = array();

    foreach ($files as $file) {
      $return[] = array(
        'fid' => $file['fid'],
        'filemime' => $file['filemime'],
        'name' => $file['filename'],
        'uri' => $file['uri'],
        'url' => file_create_url($files['uri']),
      );
    }

    return $return;
  }

  public function propertyValuesPreprocess($property_name, $value, $public_field_name) {

    $field_info = field_info_field($property_name);
    switch ($field_info['type']) {
      case 'datetime':
      case 'datestamp':
        return $this->handleDatePopulation($public_field_name, $value);
      case 'link_field':
        return array('url' => $value);
      default:
        return parent::propertyValuesPreprocess($property_name, $value, $public_field_name);
    }
  }

  private function handleDatePopulation($public_field_name, $value) {
    if (in_array($this->getBundle(), array('presentation', 'news'))) {
      return strtotime($value);
    }
    else {
      return array(array($this->publicFields[$public_field_name]['sub_property'] => ''));
    }
  }

  public function singleFileFieldDisplay($file) {
    return $this->fileFieldDisplay(array($file));
  }

  /**
   * Callback for node Owner uid.
   */
  public function getUid($wrapper) {
    $node = $wrapper->value();

    return $node->uid;
  }

  /**
   * Callback for node created timestamp.
   */
  public function getCreated($wrapper) {
    $node = $wrapper->value();

    return $node->created;
  }

  /**
   * Get the pager range.
   *
   * @return int
   *  The range.
   */
  public function getRange() {
    return 100;
  }

}
