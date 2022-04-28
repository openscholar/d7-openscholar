<?php

/**
 * @file
 * Contains \OsRestfulSpacesOverrides
 */

class OsRestfulBoxes extends \OsRestfulSpaces {

  protected $validateHandler = 'boxes';
  protected $objectType = 'boxes';

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    return $public_fields;
  }

  /**
   * Verify the user have access to the manage boxes.
   */
  public function checkGroupAccess() {
    $account = $this->getAccount();

    if ($account->uid == 1 || in_array('administrator', $account->roles)) {
      return TRUE;
    }

    if (parent::checkGroupAccess()) {
      return TRUE;
    }


    $access = !og_user_access('node', $this->space->id, 'administer boxes', $account) &&
              !og_user_access('node', $this->space->id, 'edit boxes', $account);

    if ($access) {
      // The current user can't manage boxes.
      $this->throwException("You can't manage boxes in this vsite.");
    }

    return TRUE;
  }

  /**
   * Updating a given space override.
   *
   * type: PUT
   * values: {
   *  vsite: 2,
   *  widget: os_taxonomy_fbt,
   *  options: [
   *    description: "Filter by terms"
   *  ]
   * }
   */
  public function updateSpace() {
    // Check group access.
    $this->checkGroupAccess();

    $this->object->new = FALSE;

    // Validate the object from the request.
    $this->validate();

    $controller = $this->space->controllers->{$this->objectType};
    $settings = $controller->get($this->object->delta);
    if (!count(get_object_vars($settings))) {
      $this->throwException("The delta which you provided doesn't exists");
    }
    $new_settings = array_merge((array) $settings, $this->object->options);
    $controller->set($this->object->delta, (object) $new_settings);

    return $new_settings;
  }

  /**
   * Creating a space override.
   *
   * type: POST
   * values: {
   *  vsite: 2,
   *  delta: 1419342352,
   *  widget: os_taxonomy_fbt,
   *  options: [
   *    description: "Terms"
   *  ]
   * }
   */
  public function createSpace() {
    // Check group access.
    $this->checkGroupAccess();

    // Validate the object from the request.
    $this->validate();

    // Creating a new widget.
    $options = array(
      'delta' => time(),
    ) + $this->object->options;

    // Create the box the current vsite.
    $box = boxes_box::factory($this->object->widget, $options);
    $this->space->controllers->boxes->set($box->delta, $box);

    return (array) $box;
  }

  /**
   * Delete a specific box.
   *
   * type: DELETE
   * values: {
   *  vsite: 2,
   *  delta: 1419335380,
   *  context: blogs_blogs
   * }
   */
  public function deleteSpace() {
    // Check group access.
    $this->checkGroupAccess();

    ctools_include('layout', 'os');

    $delta = $this->object->delta;
    $blocks = os_layout_get($this->object->context, FALSE, FALSE, $this->space);

    $this->space->controllers->boxes->del($delta);
    unset($blocks['boxes-' . $delta]);

    os_layout_set($this->object->context, $blocks, $this->space);
  }

  /**
   * Get the pager range.
   *
   * @return int
   *  The range.
   */
  public function getRange() {
    return 100;
  }
}
