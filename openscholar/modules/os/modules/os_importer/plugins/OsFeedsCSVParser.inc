<?php

/**
 * @file
 *
 * Extending FeedsCSVParser to allow custom parsing of CSV files.
 */
class OsFeedsCSVParser extends FeedsCSVParser {

  /**
   * Overrides FeedsCSVParser::parseHeader().
   *
   * Only lowercase the mappings and not the entries of other values after the
   * header as these are vocabularies that should stay in their original letter
   * casing.
   */
  protected function parseHeader(ParserCSV $parser, ParserCSVIterator $iterator) {
    $source = feeds_source($this->id);
    $config = $source->importer->getConfig();

    // Get the mappings of the current importer.
    $mappings = array();
    foreach ($config['processor']['config']['mappings'] as $mapping) {
      $mappings_diff[] = strtolower($mapping['source']);
      $mappings[] = $mapping['target'];
    }

    $parser->setLineLimit(1);
    $rows = $parser->parse($iterator);
    if (!count($rows)) {
      return FALSE;
    }
    $header = array_shift($rows);
    foreach ($header as $i => $title) {
      $header[$i] = $i < count($mappings) ? trim(drupal_strtolower($title)) : trim($title);
    }
    return $header;
  }
}
