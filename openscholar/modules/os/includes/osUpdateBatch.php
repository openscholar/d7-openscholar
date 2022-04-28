<?php

interface osUpdateBatch {

  /**
   * The query callback. Will return the query object.
   *
   * @param $id
   *   Optional. Represent the last ID we handled.
   *
   * @return EntityFieldQuery
   *   The Entity field query object.
   */
  public static function Query($id = NULL);

  /**
   * The callback to handle a single entity object.
   *
   * @param $entity
   *   A single entity object.
   */
  public static function Iterator($entity);

}
