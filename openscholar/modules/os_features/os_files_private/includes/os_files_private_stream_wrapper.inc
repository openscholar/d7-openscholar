<?php

/**
 * Extension of Drupal private (private://) stream wrapper class.
 *
 * Provides support for loading private files.
 */
class OSFilesPrivateStreamWrapper extends DrupalPrivateStreamWrapper {

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a private file.
   */
  function getExternalUrl() {
    $options = array('absolute' => TRUE, 'purl' => array('disabled' => TRUE));
    $path = str_replace('\\', '/', $this->getTarget());
    return url('system/files/' . $path, $options);
  }
}
