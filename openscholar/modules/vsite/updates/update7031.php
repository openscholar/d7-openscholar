<?php

/**
 * Contains static methods for vsite update 7031.
 */
class update7031 extends AbstractUpdate {

  /**
   * @inheritdoc
   */
  public static function Iterator($entity) {

    self::assignRoles($entity, array_merge(self::$nobelRoles, array('vsite user')), array('add content to books'));
    self::assignRoles($entity, self::$nobelRoles, array('administer book outlines'));
  }

}
