<?php

/**
 * Extension of Drupal public (public://) stream wrapper class.
 *
 * Provides support for storing publicly accessible files with vsite paths
 * withing the Drupal file interface.
 */
class OSPublicStreamWrapper extends DrupalPublicStreamWrapper {

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a private file.
   */
  function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    $base_url = $GLOBALS['base_url'];
    if ($this->useStatic($path) && $static = variable_get('os_files_static_domain')) {
      $base_url = $static;
      $link = array(
        '#type' => 'html_tag',
        '#tag' => 'link',
        '#attributes' => array(
          'rel' => 'dns-prefetch',
          'href' => $static
        )
      );
      drupal_add_html_head($link, 'dns-prefetch');
    }

    $url = $base_url . '/' . self::getExternalDirectoryPath() . '/' . drupal_encode_path($path);

    $files = file_load_multiple(array(), array('uri' => $this->getUri()));
    $file = reset($files);

    // file will be false if the uri in question is an image_style url, in which
    // case it's an image anyway.
    $types = variable_get('os_files_mtime_query_file_types', array('image', 'icon'));
    if ((!$file || (!empty($file->type) && in_array($file->type, $types))) && variable_get('os_files_mtime_query', TRUE)) {
      $mtime = @filemtime($this->getLocalPath());
      if (!$mtime) {
        $mtime = REQUEST_TIME;
      }
      return $url . '?m=' . $mtime;
    }
    else {
      return $url;
    }
  }

  /**
   * Determines if the path we're given should use the static domain
   */
  private function useStatic($path) {
    return _os_files_use_static_domain($path);
  }

  /**
   * Similar to getDirectoryPath(), this allows you to have seperate internal/external paths.
   * This will allow you to save to 'file_public_path' and have a different 'external facing' path that is rewritten.
   *
   * If the 'file_external_public_path' variable is not set, the drupal default is used.
   */
  public function getExternalDirectoryPath() {
    return variable_get('file_external_public_path', variable_get('file_public_path', conf_path() . '/files'));
  }
}
