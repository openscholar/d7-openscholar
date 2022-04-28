<?php
/**
 * @file Contains the OsRestfulEntityCacheableBase class.
 *
 * This class alows for clients to implement a permanent caching system, and only fetch updates for the entity in question
 */

abstract class OsRestfulEntityCacheableBase extends RestfulEntityBase {

  public static function controllersInfo() {
    return array(
      'updates\/\d*$' => array(
        RestfulInterface::GET => 'getUpdates',
        RestfulInterface::HEAD => 'getUpdates'
      )
    ) + parent::controllersInfo();
  }

  /**
   * {@inheritdoc}
   */
  public function getQueryForList() {
    $entity_type = $this->getEntityType();
    $query = $this->getEntityFieldQuery();
    if ($path = $this->getPath()) {
      list($ids, $subrequest) = explode('/', $path);

      // only filter by individual ids if no request for a dependent resource was made
      if (!$subrequest) {
        $ids = explode(',', $path);
        if (!empty($ids)) {
          $query->entityCondition('entity_id', $ids, 'IN');
        }
      }
    }

    $this->queryForListSort($query);
    $this->queryForListFilter($query);
    $this->queryForListPagination($query);
    $this->addExtraInfoToQuery($query);

    return $query;
  }

  /**
   * Returns all entities that have been updated since the timestamp given
   */
  public function getUpdates($path) {
    $timestamp = str_replace('updates/', '', $path);

    if ($timestamp < strtotime('-30 days')) {
      return $this->getList();
    }

    $request = $this->getRequest();

    $entity_type = $this->entityType;
    $result = $this
      ->getQueryForUpdates($timestamp)
      ->execute();

    $return = array();
    if (!empty($result[$entity_type])) {
      $ids = array_keys($result[$entity_type]);

      // Pre-load all entities if there is no render cache.
      $cache_info = $this->getPluginKey('render_cache');
      if (!$cache_info['render']) {
        entity_load($entity_type, $ids);
      }

      $return = array();

      // If no IDs were requested, we should not throw an exception in case an
      // entity is un-accessible by the user.
      foreach ($ids as $id) {
        try {
          if ($row = $this->viewEntity($id)) {
            $return[] = $row;
          }
        }
        catch (RestfulForbiddenException $rfe) {
          // do nothing. We just want to prevent the code from bailing out without giving us any updates.
        }
      }
    }

    $q = db_select('entities_deleted', 'ed')
      ->fields('ed')
      ->condition('entity_type', $entity_type)
      ->condition('deleted', (int)$timestamp, '>')
      ->execute();

    $deleted = array();

    foreach ($q as $r) {
      $deleted[] = array(
        'id' => $r->entity_id,
        'status' => 'deleted',
        'extra' => unserialize($r->extra)
      );
    }

    drupal_alter('os_restful_deleted_entities', $deleted, $this);
    $return = array_merge($return, $deleted);

    return $return;
  }

  public function getQueryForUpdates($timestamp) {
    $info = $this->getEntityInfo();
    $entity_type = $this->getEntityType();
    $query = $this->getEntityFieldQuery();

    $this->queryForListSort($query);
    $this->queryForListFilter($query);
    $this->queryForListPagination($query);
    $this->addExtraInfoToQuery($query);

    if (in_array('changed', $info['schema_fields_sql']['base table'])) {
      $query->propertyCondition('changed', (int)$timestamp, '>');
    }

    return $query;
  }

  public function fetchingUpdates() {
    return (strpos($this->path, 'updates/') !== FALSE);
  }

  /**
   * @param $id - the entity of the id to retrieve the last modified timestamp for
   * @return mixed - either a timestamp with the last time this entity was changed, or FALSE if the entity no longer exists
   */
  abstract protected function getLastModified($id);

  // Override these functions to allow us to act before any actions are performed
  protected function updateEntity($id, $null_missing_fields = FALSE) {
    $unmodified = \RestfulManager::getRequestHttpHeader('If-Unmodified-Since');
    if ($unmodified) {
      $modified = $this->getLastModified($id);
      if ($modified === FALSE) {
        throw new RestfulGoneException(t("Entity @id has been deleted.", array('@id' => $id)), 410);
      }
      if (strtotime($unmodified) < $modified) {
        throw new RestfulException(t("Entity @id has been modified since updates were last retrieved.", array('@id' => $id)), 409);
      }
    }
    return parent::updateEntity($id, $null_missing_fields);
  }

  public function deleteEntity($entity_id) {
    $unmodified = \RestfulManager::getRequestHttpHeader('If-Unmodified-Since');
    if ($unmodified) {
      $modified = $this->getLastModified($entity_id);
      if ($modified === FALSE) {
        throw new RestfulGoneException(t("Entity @id has been deleted.", array('@id' => $entity_id)), 410);
      }
      if (strtotime($unmodified) < $modified) {
        throw new RestfulException(t("Entity @id has been modified since updates were last retrieved.", array('@id' => $entity_id)), 409);
      }
    }
    return parent::deleteEntity($entity_id);
  }

  /**
   * {@inheritdoc}
   */
  public function getQueryCount() {

    list ($arg1, $arg2) = explode('/', $this->getPath());
    if ($arg1 == 'updates' && $arg2 > strtotime("-30 days")) {
      $info = $this->getEntityInfo();
      $query = $this->getEntityFieldQuery();

      $this->queryForListSort($query);
      $this->queryForListFilter($query);
      $this->addExtraInfoToQuery($query);

      if (in_array('changed', $info['schema_fields_sql']['base table'])) {
        $query->propertyCondition('changed', (int)$timestamp, '>');
      }
    }
    else {
      $query = $this->getEntityFieldQuery();
      $this->queryForListFilter($query);
      $this->addExtraInfoToQuery($query);
    }

    $query->addTag('restful_count');

    return $query->count();
  }

  /**
   * Return a count of all entities, ignoring filters
   */
  public function getAllCount() {
    $query = $this->getEntityFieldQuery();
    $this->queryForListFilter($query);
    $this->addExtraInfoToQuery($query);
    $query->addTag('restful_count');

    return $query->count();
  }

  /**
   * {@inheritdoc}
   */
  public function isListRequest() {
    $isList = false;

    $path = $this->getPath();
    list ($arg1, $arg2) = explode('/', $path);
    if ($arg1 == 'updates') {
      $isList = true;
    }

    return $isList || parent::isListRequest();
  }

  
  public function additionalHateoas() {
    $addtl = array();
    $path = $this->getPath();

    if ($this->method == \RestfulInterface::GET) {
      list($call, $timestamp) = explode('/', $path);
      if ($call == "") {
        $addtl['allEntitiesAsOf'] = REQUEST_TIME;
      }
      else if ($call == 'updates') {
        if ($timestamp < strtotime('-30 days')) {
          $addtl['allEntitiesAsOf'] = REQUEST_TIME;
        } else {
          $addtl['updatesAsOf'] = REQUEST_TIME;
        }

        $addtl['totalEntities'] = intval($this->getAllCount()->execute());
      }
    }

    return $addtl;
  }

  /**
   * {@inheritdoc}
   */

  public function getUrl($request = NULL, $options = array(), $keep_query = TRUE) {
    // By default set URL to be absolute.
    $options += array(
      'absolute' => TRUE,
      'query' => array(),
    );

    if ($keep_query) {
      // Remove special params.
      unset($request['q']);
      static::cleanRequest($request);

      // Add the request as query strings.
      $options['query'] += $request;
    }

    return $this->versionedUrl($this->getPath(), $options);
  }
}

?>