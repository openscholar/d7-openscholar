<?php

/**
 * @contains
 */

class CsvExport extends ExportBase {

  /**
   * {@inheritdoc}
   */
  public function exportToFile($name) {
    $content = $this->generateFileContent();

    header("Content-Type: text/plain");
    header("Content-Disposition: attachment; filename=\"$name\";" );

    echo $content;
    drupal_exit();
  }
}
