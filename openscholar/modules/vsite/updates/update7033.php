<?php

/**
 * Contains static methods for vsite update 7032.
 */
class update7033 extends AbstractUpdate {

  /**
   * @inheritdoc
   */
  public static function Query($id = NULL) {
    return parent::Query()->fieldCondition('og_roles_permissions', 'value', TRUE);
  }

  /**
   * @inheritdoc
   */
  public static function Iterator($entity) {
    $roles = og_roles('node', $entity->type, $entity->nid, false, false);
    self::assignRoles($entity, $roles, array('insert link into wysiwyg'));
  }

}
