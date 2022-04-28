<?php

/**
 * Extracting the files from the zip to a folder, parsing information from the
 * info file and more.
 *
 * How to use:
 *  $fid = 1;
 *  $file = new SubTheme($fid);
 *  $file->extract(); // This isn't necessary anymore.
 *  $info = $file->parseInfo();
 *
 * or:
 *
 *  $file = new SubTheme();
 *  $file->path = GIT_CLONED_PATH;
 *  $info = $file->parseInfo();
 */

class SubTheme {

  /**
   * The file ID.
   * @var
   */
  public $fid;

  /**
   * The path of the ZIP file.
   * @var
   */
  public $filePath;

  /**
   * The path which the ZIP file is extracted to.
   */
  public $extractPath;

  /**
   * The file object.
   * @var
   */
  public $file;

  /**
   * The path of the folder.
   * @var string
   */
  public $path;

  /**
   * Path of the flavour file in the subthemes' folder (and it's ready to use).
   *
   * @var string|null
   */
  private $flavourPath;

  /**
   * Constructing the class.
   *
   * @param $fid
   *  A file id.
   */
  public function __construct($fid = NULL) {
    $this->fid = $fid;
    $this->flavourPath = NULL;
    $this->fileInfo();

    if (!empty(glob($this->extractPath . DIRECTORY_SEPARATOR . '*.flav'))) {
      // Subtheme has a falvour file and
      // is extracted already
      // OR
      // it's a git project, not a ZIP archive.
      if (empty($this->path)) {
        $this->path = $this->extractPath;
      }
      $this->flavourPath = reset(glob($this->extractPath . DIRECTORY_SEPARATOR . '*.flav'));
    }
  }

  /**
   * Return info for the intended folder path for a zip file.
   */
  public function fileInfo() {
    $this->file = file_load($this->fid);

    // Creating an array of stream wrappers that will be removed.
    $streams = array();
    foreach (stream_get_wrappers() as $stream) {
      $streams[] = $stream . '://';
    }

    $files_folder = variable_get('file_public_path', conf_path() . DIRECTORY_SEPARATOR . 'files');

    // Generate the folder name by the unique URI of the file.
    $file_name = str_replace($streams, '', $this->file->uri);
    $folder_name = str_replace(array('.', '_'), '-', $file_name);

    $this->filePath = empty($this->filePath) ? $files_folder . DIRECTORY_SEPARATOR . $file_name : $this->filePath;
    $this->extractPath = empty($this->extractPath) ? $files_folder . DIRECTORY_SEPARATOR . $folder_name : $this->extractPath;
  }

  /**
   * Extracting the zip file.
   */
  public function extract() {
    if (!empty($this->flavourPath)) {
      return;
    }

    $zip = new ZipArchive;

    if ($zip->open($this->filePath) === TRUE) {
      // Extract the file.
      $zip->extractTo($this->extractPath);
      $zip->close();

      $this->processExtractedSubtheme(TRUE);
    }
    else {
      // Couldn't extract the zip file, display an error message and watchdog.
      $params = array(
        '@file' => $this->filePath,
        '@path' => $this->extractPath,
      );

      $message = t('Unable to extract @file to @path', $params);
      watchdog('subtheme', $message);
      drupal_set_message($message, 'error');
    }
  }

  /**
   * Helper callback to process the Subtheme.
   *
   * @param bool $cleanup_needed
   *   Whether the subtheme folder has to be cleaned form unnecessary folders
   *   or not.
   */
  private function processExtractedSubtheme($cleanup_needed = FALSE) {
    $flav_root = $this->findFileByPattern($this->extractPath, '*.flav');
    // We found a flavour file and can set up path for this subtheme.
    $this->path = $this->extractPath;

    if (
      !$flav_root ||
      $flav_root === $this->extractPath
    ) {
      return;
    }

    if ($cleanup_needed) {
      //
      // Remove unnecessary subfolders.
      //
      $dirs_in_path_root = glob($this->path . DIRECTORY_SEPARATOR . '*', GLOB_ONLYDIR);
      foreach ($dirs_in_path_root as $index => $dir_in_path_root) {
        if (strpos($flav_root, $dir_in_path_root) !== 0) {
          $this->deleteTree($dir_in_path_root);
          unset($dirs_in_path_root[$index]);
        }
      }
    }

    //
    // Move theme files and folders into the root path.
    //
    $flav_root_content = array_filter(scandir($flav_root), function($value) {
      return !in_array($value, ['.', '..']);
    });

    foreach ($flav_root_content as $resource) {
      rename(
        $flav_root . DIRECTORY_SEPARATOR . $resource,
        $this->path .  DIRECTORY_SEPARATOR . $resource
      );
    }

    if ($cleanup_needed) {
      // Remove the first subpath in the unzipped folder where the flavour files
      // were extracted originally if it won't cause component loss.
      // Exception example:
      // Flavour file extracted to:
      // @code
      // public://
      // |-- my-flavour-zip
      //   |-- css
      //     |-- my-flavout.flav
      //     |-- screenshot.png
      //     |-- css
      //       |-- my-flavout.css
      // @endcode
      // In this case, we wont remove the 'public://my-flavour-zip/css' dir
      // since it would lead to loose 'my-flavout.css' as well.
      foreach ($dirs_in_path_root as $dir_in_path_root) {
        if (!in_array(basename($dir_in_path_root), $flav_root_content)) {
          $this->deleteTree($dir_in_path_root);
        }
      }
    }
  }

  /**
   * Helper method to find file in a directory by a name pattern.
   *
   * @param string $path
   *   The path to search in recursively.
   * @param string $pattern
   *   File or directory name pattern to match.
   *
   * @return string|null
   *   Path to the matching resource found first or NULL if there are no matches
   *   at all.
   */
  private function findFileByPattern($path, $pattern) {
    $path_match = NULL;
    $files_in_path = glob($path . DIRECTORY_SEPARATOR . trim($pattern, " \t\n\r\0\x0B" . DIRECTORY_SEPARATOR));

    if (!empty($files_in_path)) {
      return $path;
    }

    $dirs_in_path = glob($path . DIRECTORY_SEPARATOR . '*', GLOB_ONLYDIR);

    if (empty($dirs_in_path)) {
      return NULL;
    }

    foreach ($dirs_in_path as $subdir) {
      $path_match = $this->findFileByPattern($subdir, $pattern);

      if ($path_match) {
        break;
      }
    }

    return $path_match;
  }

  /**
   * Helper method to delete non-empty directory.
   *
   * @param string $dir
   *   A relative or absolute dir to delete.
   *
   * @return bool
   *   Returns TRUE on success or FALSE on failure (e.g. lack of permissions).
   */
  private function deleteTree($dir) {
    $files = array_filter(scandir($dir), function($value) {
      return !in_array($value, ['.', '..']);
    });

    foreach ($files as $file) {
      $match = $dir . DIRECTORY_SEPARATOR . $file;
      (is_dir($match)) ? $this->deleteTree($match) : unlink($match);
    }

    return rmdir($dir);
  }

  /**
   * Parsing the info file of the sub theme.
   */
  public function parseInfo() {
    if (empty($this->path)) {
      $this->extract();
    }

    $info_files = glob($this->path . '/*.flav');
    $info = array();

    $info_file = reset($info_files);

    // Get the information from the info file.
    $file_content = drupal_parse_info_file($info_file);

    foreach ($file_content as $key => $value) {
      $info[$key] = $value;
    }

    $info['theme name'] = str_replace(array(dirname($info_file), '.flav', '/'), '', $info_file);

    return $info;
  }
}
