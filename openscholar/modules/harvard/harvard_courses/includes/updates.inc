<?php

class HarvardCoursesUpdate7004 {

  public static function query($id = NULL) {
    $query = new EntityFieldQuery();

    $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('type', 'harvard_course');

    if ($id) {
      $query->propertyCondition('nid', $id, '>=');
    }

    return $query;

  }

  public static function iterator($entity) {
    $wrapper = entity_metadata_wrapper('node', $entity);
    $sites = array();

    // Import by catalog number.
    if ($wrapper->field_cat_number->value()) {
      if ($nids = haravard_courses_sites_nid_by_import_type('cat_num', array($wrapper->field_cat_number->value()))) {
        $sites += $nids;
      }
    }
    // Import by department ID and school name.
    if ($wrapper->field_department_id->value() && $wrapper->field_faculty->value()) {
      $nids = haravard_courses_sites_nid_by_import_type('department_school', array(
        'field_department_id' => $wrapper->field_department_id->value(),
        'field_faculty' => $wrapper->field_faculty->value(),
      ));

      if ($nids) {
        $sites += $nids;
      }
    }

    if ($sites) {
      $wrapper->{OG_AUDIENCE_FIELD}->set($sites);
      $wrapper->save();

      if (drupal_is_cli()) {
        $params = array(
          '@nids' => implode(", ", $sites),
          '@title' => $entity->title,
          '@nid' => $entity->nid,
        );
        drush_log(dt("The course @title(@nid) grouped to the sites id's: @nids", $params), 'success');
      }
    }
  }
}
