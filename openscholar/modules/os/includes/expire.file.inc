<?php

/**
 * @file
 * Provides class that expires files.
 */

class OSExpireFile extends ExpireFile {

  /**
   * Executes expiration actions for file.
   *
   * @param $file
   *   File object.
   *
   * @param $action
   *   Action that has been executed.
   *
   * @param $skip_action_check
   *   Shows whether should we check executed action or just expire node.
   */
   function expire($file, $action, $skip_action_check = FALSE) {

    if (empty($file->fid)) {
      return;
    }

    //Expire the standard File URI's
    parent::expire($file, $action, $skip_action_check);

    $styles = image_styles();
    $expire_urls = array();

    $file_url = file_create_url($file->uri);
    $parsed_url = parse_url($file_url);

    // Remove the non-purl prefixed version of the file.
    $expire_urls[$parsed_url['path']] = $file_url;

    foreach ($styles as $style) {
      $image_path = image_style_path($style['name'], $file->uri);
      if (file_exists($image_path)) {
        $path = file_create_url($image_path);
        watchdog('OSExpireFile', "Add [".print_r($path,TRUE)."] image derivitive for clearing.");
        $expire_urls[$path] = $path;
      }
    }

    //Expire the derived imagecache instances
    // Flush page cache for expired urls.
    if (!empty($expire_urls)) {
      ExpireAPI::executeExpiration($expire_urls,'',NULL,TRUE);
    }
  }
}
