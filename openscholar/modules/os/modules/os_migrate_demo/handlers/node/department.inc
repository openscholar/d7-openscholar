<?php

/**
 * Migrate "Department" node.
 */
class OsMigrateDepartmentNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'department';

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('purl', 'URL'),
    array('public', 'Public'),
    array('domain', 'Domain'),
    array(VSITE_ACCESS_FIELD, 'Access'),
    array('uid', 'UID'),
    array('members', 'Members'),
  );

  public $dependencies = array(
    'OsMigrateUsers',
  );

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('purl', 'purl');
    $this->addFieldMapping('field_site_address', 'purl');
    $this->addFieldMapping(VSITE_ACCESS_FIELD, VSITE_ACCESS_FIELD);

    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');

    $this->addFieldMapping('users_to_group', 'members')
      ->sourceMigration('OsMigrateUsers')
      ->separator('|');
  }

  /**
   * Creating the PURL address for the personal site.
   */
  public function prepare($entity, $row) {
    $entity->purl = array('value' => $row->purl);

    $value = $entity->field_site_address[LANGUAGE_NONE][0]['value'];
    $entity->field_site_address[LANGUAGE_NONE][0]['value'] = variable_get('purl_base_domain') . '/' . $value;

    // This is a private site.
    if ($row->public == 'private') {
      $entity->{OG_ACCESS_FIELD}[LANGUAGE_NONE][0]['value'] = TRUE;
    }
  }

  /**
   * Group the users with the create group.
   */
  function complete($entity, $row) {
    if ($row->domain) {
      $nid = entity_id($this->entityType, $entity);
      os_migrate_demo_spaces_overrides('og', $nid, $row->domain);
    }

    if (is_array($entity->users_to_group)) {
      foreach ($entity->users_to_group as $uid) {
        og_group('node', $entity->nid, array(
          'entity' => $uid,
        ));
      }
    }
    else {
      og_group('node', $entity->nid, array(
        'entity' => $entity->users_to_group,
      ));
    }
  }
}
