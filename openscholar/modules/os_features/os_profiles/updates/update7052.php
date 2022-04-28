<?php

/**
 * Contains static methods for os_profile update 7052.
 */


class update7052 extends AbstractUpdate {

  /**
   * @inheritdoc
   */
  public static function Query($id = NULL) {
    $query = db_select('field_data_field_original_destination_url', 'u')
            ->fields('u')
            ->condition('field_original_destination_url_value', '%node%.json', 'LIKE');

    return $query;
  }

  /**
   * @inheritdoc
   */
  public static function Iterator($entity) {

    $old_url = $entity->field_original_destination_url[LANGUAGE_NONE][0]['value'];
    $url_pattern = '/node\/+(\d+)/';
    if (preg_match($url_pattern, $old_url, $matches)) {
      $host = parse_url($old_url)['host'];
      $entity_id = $matches[1];
      $new_url = "http://" . $host . "/" . OS_PROFILE_JSON_PREFIX . $entity_id;

      db_update('field_data_field_original_destination_url')
        ->fields(array('field_original_destination_url_value' => $new_url))
        ->condition('entity_id', $entity->entity_id)
        ->execute();
    }
  }
}
