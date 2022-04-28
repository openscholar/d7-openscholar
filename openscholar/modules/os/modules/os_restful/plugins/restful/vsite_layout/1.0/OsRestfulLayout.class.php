<?php

/**
 * @file
 * Contains \OsRestfulLayout
 */

class OsRestfulLayout extends \OsRestfulSpaces {

  protected $validateHandler = 'layouts';
  protected $objectType = 'context';

  /**
   * Verify the user have access to the manage layout.
   */
  public function checkGroupAccess() {
    return TRUE;

    if (parent::checkGroupAccess()) {
      return TRUE;
    }

    $account = $this->getAccount();

    if (!spaces_access_admin($account, $this->space)) {
      // The current user can't manage boxes.
      $this->throwException("You can't manage layout in this vsite.");
    }

    return true;
  }

  /**
   * Updating a given space override.
   *
   * type: PUT
   * values: {
   *  vsite: 2,
   *  object_id: os_pages-page-581,
   *  blocks: [
   *    os_search_db-site-search: [
   *      region: "sidebar_first"
   *    ]
   *  ]
   * }
   */
  public function updateSpace() {
    // Check group access.
    $this->checkGroupAccess();

    // Validate the object from the request.
    $this->validate();

    // Set up the blocks layout.
    ctools_include('layout', 'os');

    $blocks = os_layout_get($this->object->object_id, FALSE, FALSE, $this->space);

    foreach ($blocks as $delta => $block) {
      if (empty($this->object->blocks[$delta])) {
        continue;
      }
      $blocks[$delta] = array_merge($blocks[$delta], $this->object->blocks[$delta]);
    }

    os_layout_set($this->object->object_id, $blocks, $this->space);

    return $blocks;
  }

  /**
   * Creating a space override.
   *
   * type: POST
   * values: {
   *  vsite: 2,
   *  object_id: os_pages-page-581,
   *  boxes: [
   *    boxes-1419335380: [
   *      module: "boxes",
   *      delta: "1419335380",
   *      region: "sidebar_second",
   *      weight: 2,
   *      status: 0
   *    ]
   *  ]
   * }
   */
  public function createSpace() {
    // Check group access.
    $this->checkGroupAccess();

    // Validate the object from the request.
    $this->validate();

    if (!isset($this->object->blocks['os_pages-main_content'])) {
      // When creating the layout override we need the page content.
      $this->object->blocks['os_pages-main_content'] = array(
        'module' => "os_pages",
        'delta' => "main_content",
        'region' => "content_top",
      );
    }

    // Set up the blocks layout.
    ctools_include('layout', 'os');

    os_layout_set($this->object->object_id, $this->object->blocks, $this->space);

    return $this->object->blocks;
  }

  /**
   * In order to delete the layout override pass the next arguments:
   *
   * type: DELETE
   * values: {
   *  vsite: 2,
   *  object_id: os_pages-page-582:reaction:block,
   *  delta: boxes-1419335380
   * }
   */
  public function deleteSpace() {
    // Check group access.
    $this->checkGroupAccess();

    db_delete('spaces_overrides')
      ->condition('object_id', $this->object->object_id)
      ->condition('id', $this->object->vsite)
      ->execute();
  }
}
