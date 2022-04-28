<?php

interface Export {

  /**
   * Concatenating content to the content variable.
   *
   * @param $content
   *   The value to add.
   */
  public function addContent($content);

  /**
   * Returning the content variable.
   *
   * @return array
   *   Get the content variable.
   */
  public function getContent();

  /**
   * Set the content variable to a specific value.
   *
   * @param $content
   *   The content to set.
   *
   * @return $this
   *   The current object.
   */
  public function setContent($content);

  /**
   * @return string
   *   Generate the file.
   */
  public function generateFileContent();

  /**
   * Put array in the beginning of the content variable.
   *
   * @return $this
   *   The current object.
   */
  public function setFileHeader($header);

  /**
   * Export the content to a file.
   *
   * @param $name
   *   The name of the file.
   */
  public function exportToFile($name);
}
