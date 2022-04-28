<?php

/**
 * Contains static methods for os_profile update 7051.
 */


class update7051 extends AbstractUpdate {

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

    // Now trigger a re-sync of the node data by notifying the listeners
    $node = node_load($entity->nid);
    $uuid = $node->field_uuid[LANGUAGE_NONE][0]['value'];

    $destination = "http://" . $_SERVER['SERVER_NAME'];
    $arguments = array(
      'action' => 'update',
      'UUID' => $uuid,
    );

    os_profiles_manage_synced_profiles($destination, $arguments);
  }
}
