<?php

/**
 * @file
 * Contains AbstractUpdate.php
 */

abstract class AbstractUpdate implements osUpdateBatch {

  static $nobelRoles = array('content editor', 'vsite admin');

  /**
   * Helper function to assign roles. For a much more clear code.
   *
   * @param $entity
   *   The vsite object.
   * @param $granted_roles
   *   The roles which will be granted with the roles.
   * @param $permission
   *   The permission to grant.
   */
  static protected function assignRoles($entity, $granted_roles, $permission) {
    $roles = og_roles('node', $entity->type, $entity->nid);

    foreach ($roles as $rid => $role) {
      if (!in_array($role, $granted_roles)) {
        continue;
      }

      og_role_grant_permissions($rid, $permission);
    }
  }

  /**
   * Return a base query object for iterating over all the available groups.
   *
   * @return EntityFieldQuery
   */
  static protected function getBaseQuery() {
    $query = new EntityFieldQuery();

    $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('type', array_keys(vsite_vsite_og_node_type_info()), 'IN');

    return $query;
  }

  /**
   * @inheritdoc
   */
  public static function Query($id = NULL) {
    $query = self::getBaseQuery();

    if ($id) {
      $query->propertyCondition('nid', $id, '>=');
    }

    return $query;
  }

}
