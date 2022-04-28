<?php

/**
 * Contains static methods for vsite update 7032.
 */
class update7032 extends AbstractUpdate {

  static $nobelRoles = array('content editor', 'vsite admin', 'member');

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
    self::assignRoles($entity, self::$nobelRoles, array('insert link into wysiwyg'));
  }

}
