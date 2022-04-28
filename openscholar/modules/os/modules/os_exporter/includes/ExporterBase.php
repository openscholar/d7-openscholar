<?php

abstract class ExportBase implements Export {

  /**
   * @var array
   *
   * Holds the file content.
   */
  protected $content = array();

  /**
   * @var \EntityFieldQuery.
   *
   * The object of the entity field query.
   */
  protected $query;

  /**
   * @var array
   *
   * The information of the fields to export.
   */
  protected $fields = array();

  /**
   * @var string
   *
   * The entity type.
   */
  protected $entityType;

  /**
   * @var string
   *
   * The bundle of the entity.
   */
  protected $bundle;

  /**
   * {@inheritdoc}
   */
  public function addContent($content) {
    $this->content[] = $content;
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function getContent() {
    return $this->content;
  }


  /**
   * {@inheritdoc}
   */
  public function setContent($content) {
    $this->content = $content;
    return $this;
  }

  /**
   * {@inheritdoc}
   */
  public function generateFileContent() {
    $file_content = '';

    foreach ($this->content as $content) {
      $file_content .= implode(',', $content) . "\n";
    }

    return $file_content;
  }

  /**
   * {@inheritdoc}
   */
  public function setFileHeader($header) {
    $content = $this->content;

    $this->content = array($header);
    $this->content[] = $content;
    return $this;
  }
}
