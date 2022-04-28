<?php

/**
 * Contains static methods for vsite update 7029.
 */
class update7029 extends AbstractUpdate {

  /**
   * @inheritdoc
   */
  public static function Iterator($entity) {
    self::assignRoles($entity, self::$nobelRoles, array('bypass redirection'));
  }
}
