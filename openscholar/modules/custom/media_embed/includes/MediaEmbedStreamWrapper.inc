<?php

/**
 *  @file
 *  Create a Embed Stream Wrapper class for the Media/Resource module.
 *  Just a shell to trick Drupal into making files for internet resources.
 */

/**
 *  Create an instance like this:
 *  $embed = new MediaEmbedStreamWrapper('embed://[md5-hash]');
 */
class MediaEmbedStreamWrapper extends MediaReadOnlyStreamWrapper {

  function interpolateUrl() {
    return $this->get_parameters();
  }

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'html/embed';
  }
}
