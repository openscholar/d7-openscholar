<?php

/**
 * Migrate gallery feeds.
 */
class OsMigrateGalleryNode extends OsMigrate {
  public $entityType = 'node';
  public $bundle = 'media_gallery';

  public $csvColumns = array(
    array('id', 'ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('images', 'Images'),
    array('destination', 'Destination'),
    array(OG_AUDIENCE_FIELD, 'Site'),
    array(OG_VOCAB_FIELD, 'Terms'),
    array('uid', 'UID'),
  );

  public $dependencies = array(
    'OsMigratePersonalNode',
    'OsMigrateProjectNode',
    'OsMigrateScienceTaxonomyTerm',
    'OsMigrateUsers',
  );

  public function __construct() {
    parent::__construct();

    // $this->addFieldMapping('body', 'body');
    $this->addFieldMapping(OG_AUDIENCE_FIELD, OG_AUDIENCE_FIELD)
      ->sourceMigration(array('OsMigratePersonalNode', 'OsMigrateProjectNode'));

    $this->addFieldMapping('media_gallery_file', 'images')
      ->separator(';');
    $this->addFieldMapping('media_gallery_file:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('media_gallery_file:source_dir')
      ->defaultValue(drupal_get_path('module', 'os_migrate_demo') . '/images');
    $this->addFieldMapping('media_gallery_file:destination_dir', 'destination');

    $this->addFieldMapping(OG_VOCAB_FIELD, OG_VOCAB_FIELD)
      ->sourceMigration(array('OsMigrateScienceTaxonomyTerm'))
      ->separator('|');

    $this->addFieldMapping('uid', 'uid')
      ->sourceMigration('OsMigrateUsers');
  }

  /**
   * Create OG memberships for the used files in the galleries.
   */
  public function complete($entity, $row) {
    $wrapper = entity_metadata_wrapper('node', $entity);
    $gid = $wrapper->{OG_AUDIENCE_FIELD}->value(array('identifier' => TRUE));
    foreach ($wrapper->media_gallery_file->value() as $file) {
      $membership = og_membership_create('node', reset($gid), 'file', $file['fid'], OG_AUDIENCE_FIELD);
      og_membership_save($membership);

      // Create a dummy file_usage entry. It will be removed only when the file is
      // deleted.
      if ($file_temp = file_load($file['fid'])) {
        file_usage_add($file_temp, 'os_files', 'node', $gid);
      }
    }
  }
}
