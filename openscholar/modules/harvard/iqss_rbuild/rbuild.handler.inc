<?php
// $Id$


/**
 * @file
 * Implements os_software_handler for rBuild
 */

/**
 * Includes os_software_handler PHP Interface
 */
require_once (drupal_get_path('module', 'os_software') . "/os_software.handler.inc");

/**
 * Implements OS Software handler for Rbuild.
 * @author oren [or] <oren_robinson@harvard.edu>
 *
 */
class rbuild_handler implements os_software_handler {

  private static $cached_packages = array();

  // An array of associative arrays of releases
  private $packages = array();
  // Unique machine name for this Rbuild product
  private $short_name = FALSE;
  // The remote repository URL
  private $url = FALSE;

  /**
   * Constructor
   *
   * @param string $url
   * @param string $app_key
   */
  function __construct($url, $short_name = FALSE) {
    // Caches all projects
    if ($url) {
      $new_url = $this->create_url($url);
      if (!array_key_exists($new_url, self::$cached_packages)) {
        self::$cached_packages[$new_url] = $this->parse($new_url);
      }
    }

    // Uses cached packages or tries this project directly
    if (count(self::$cached_packages[$this->create_url($url)])) {
      $this->packages = self::$cached_packages[$this->create_url($url)];
    }
    elseif ($url) {
      $this->packages = $this->parse($this->create_url($url, $short_name));
    }

    $this->short_name = $short_name;
    $this->url = rtrim($url, '/ ');

    return count($this->packages) ? TRUE : FALSE;
  }

  /**
   * Gets all packages
   *
   * @return array $packages
   */
  public function get_packages() {
    return $this->packages;
  }

  /**
   * Gets the package if rBuild short name is defined.
   *
   * @return mixed $package
   *   Returns array if exists, otherwise returns FALSE.
   */
  public function get_package() {
    if (!$this->short_name) {
      return FALSE;
    }
    if (!array_key_exists($this->short_name, $this->packages)) {
      return FALSE;
    }

    return $this->packages[$this->short_name];
  }

  /**
   * Alters a project node before it is saved
   *
   * @param object $project_node
   *
   * @return bool $changed
   *   TRUE only if changes were made to the Software Project node
   */
  public function alter_project(&$project_node) {
    $changed = FALSE;
    if (!($package = $this->get_package())) {
      return $changed;
    }

    // Updates the title with what is in rbuild
    if (!strlen($project_node->title)) {
      $short_name_stored = substr($package['title'], 0, strlen($this->short_name));
      $is_new_key = (strtolower($short_name_stored) !== strtolower($this->short_name));
      if (strlen($this->short_name) && $is_new_key) {
        $title_prefix = $this->short_name . ': ';
      }
      else {
        $title_prefix = '';
      }

      $project_node->title = $title_prefix . $package['title'];
      $changed = TRUE;
    }

    return $changed;
  }

  /**
   * Creates the initial batch of release files
   *
   * @param object $project_node
   */
  public function fetch_releases(&$project_node) {
    if (!is_numeric($project_node->nid)) {
      return FALSE;
    }

    $releases = $this->find_all_release_files();

    if (sizeof($releases) === 0) {
      return FALSE;
    }

    // array
    $package = $this->get_package();

    $operations = array();
    foreach ($releases as $version => $info) {
      // Creates new Software Release node
      $node_exists = _rbuild_query_release_node_exists($project_node->nid, $version);
      if ($node_exists) {
        continue;
      }

      // Adds this node to the operations queue only if it doesn't exist yet.
      $operations[] = array(
        '_rbuild_add_release_node',
        array($project_node, $version, $info['filename'], $package, $this->url, $info['created']),
      );
      // Orders operations to import oldest releases first
      $operations = array_reverse($operations);
    }

    if (count($operations) === 0) {
      return;
    }

    $batch = array(
      'title' => t('Importing all Software Releases for <em>' . $project_node->title . '</em>'),
      'init_message' => t('Finding packages for <em>' . $this->short_name . '</em> at <em>' . $this->url . '</em>...'),
      'operations' => $operations,
      'finished' => '_rbuild_batch_finished',
    );

    // Redirects user to node view page after batch process.
    // like "node/13"
    $project_path = 'node/' . $project_node->nid;
    // like "software/title"
    $project_path = drupal_lookup_path('alias', $project_path);
    batch_set($batch);

    // Processes batch without redirect if this is being run by cron.
    global $user;
    if ((int) $user->uid === 0 || strpos(request_uri(), '/admin/config/system/cron') === 0) {
      $batch =& batch_get();
      $batch['progressive'] = FALSE;
      batch_process();
      return TRUE;
    }

    batch_process($project_path);
    return TRUE;
  }

  /**
   * Checks that rBuild short name is found at URL
   */
  public function validate() {
    if (!($package = $this->get_package())) {
      form_set_error(
        'rbuild short name',
        t('Invalid rBuild settings: Product <em>!shortname</em> not found at <em>!url</em>.',
          array(
            '!shortname' => $this->short_name,
            '!url' => $this->url,
          )
        )
      );
    }
  }

  /**
   * Creates URL path.
   *
   * @param string $url
   * @param string $short_name
   *
   * @return string $path
   */
  private function create_url($url, $short_name = FALSE) {
    $path = rtrim($url, '/ ') . '/PACKAGES';

    // Appends rBuild short name if given
    if (strlen($short_name) > 0) {
      $path .= $short_name;
    }

    return $path;
  }

  /**
   * Parses a package file and retrieves avalible packages
   *
   * @param $url
   *
   * @return array $packages
   */
  private function parse($url) {
    $package_file = _rbuild_get_file_contents($url);
    $packages_raw = preg_split("#\n\n#", $package_file);

    // Prepares package name and information.
    $packages = array();
    foreach ($packages_raw as $package_raw) {
      $package_info = array();
      $package_name = FALSE;
      $package_raw  = trim($package_raw);
      $lines        = preg_split("#\n#", $package_raw);

      // Searches lines like "Key: Value" for package name and metadata.
      foreach ($lines as $line) {
        $line_elements = (explode(':', $line, 2));
        if (count($line_elements) === 2) {
          $current_key = strtolower(trim($line_elements[0]));
          $current_value = trim($line_elements[1]);
          if ($current_key === 'package') {
            $package_name = $current_value;
          }
          $package_info[$current_key] = $current_value;
        }
        elseif (count($line_elements) === 1) {
          $line_full = $line_elements[0];
          $package_info[$line_full] = ' ' . $line_full;
        }
      }

      if ($package_name && count($package_info)) {
        $packages[$package_name] = $package_info;
      }
    }

    return $packages;
  }

  /**
   * Parses a package's files.
   *
   * @param $url
   *
   * @return array $releases
   *   An array of strings containing release package filepaths like:
   *
   *   array(
   *     '1.0' => 'http://harvard.edu/src/short_name-1.0.tar.gz',
   *     '2.0' => 'http://harvard.edu/src/short_name-2.0.tar.gz',
   *   )
   */
  private function find_all_release_files() {

    $releases = array();

    // Reads in the full list of download files
    $index_file_data = _rbuild_get_file_contents($this->url . '/');
    // Empty array()
    if (strlen($index_file_data) === 0) {
      return $releases;
    }

    // Finds all links to gzip files matching this project's short name
    $filename_matches = array();
    // Matches all links with this project's shortname in the href value
    $filename_pattern = '/(a href\=")(' . $this->short_name . '([\._-\d]*)\.tar\.gz)(")/i';
    preg_match_all($filename_pattern, $index_file_data, $filename_matches);

    // Extracts the parts we want: unique versions and package filenames.
    $filenames = $filename_matches[2];
    $versions = $filename_matches[3];

    $datestr_matches = array();
    // Matches a datetime string formatted like "14-Jul-2006 14:49"
    $datestr_pattern = '/a href\="(' . $this->short_name . '([\._-\d]*)\.tar\.gz)(").*>0?(\d+?-\w+-\d{4}\s+\d+:\d{2})/i';
    preg_match_all($datestr_pattern, $index_file_data, $datestr_matches);

    // Extracts the parts we want: unique versions and package filenames.
    $datestrs = $datestr_matches[4];

    $i = 0;
    foreach ($filenames as $index => $filename) {
      $datestr = $datestrs[$i];
      $created = $this->format_created_date($datestr);
      $releases[trim($versions[$index], '_')] = array(
        'filename' => $filename,
        'created' => $created,
      );
      $i++;
    }
    // Empty array()
    if (count($releases) === 0) {
      return $releases;
    }

    // Ensures the current release is also included in the $releases array
    $package = $this->get_package();
    $source_path = $this->url . '/' . $this->short_name . '_' . $package['version'] . '.tar.gz';
    if (file_exists($source_path)) {
      $releases[$package['version']] = $source_path;
    }

    return $releases;
  }

  /**
   *
   * @param string $datestr
   *   A datetime string formatted like "14-Jul-2006 14:49"
   * @return int|bool $time
   *   A created timestamp as expected by a drupal node object, or FALSE.
   */
  private function format_created_date($datestr) {
    // An array like array("14", "Jul", "2006 14:49").
    $date_parts_1 = explode('-', $datestr);
    // A string like "2006 14:49".
    $remainder = array_pop($date_parts_1);
    // An array like array("2006", "14:49").
    $date_parts_2 = explode(' ', $remainder);
    // An array like array("14", "Jul", "2006" "14:49").
    $date_parts = array_merge($date_parts_1, $date_parts_2);

    // Uses Common Log Format
    // A string like "14/Jul/2006:14:49:00 -0400"
    // @see http://www.php.net/manual/en/datetime.formats.compound.php
    $log_format_parts = array(
      array_shift($date_parts),
      '/',
      array_shift($date_parts),
      '/',
      array_shift($date_parts),
      ':',
      array_shift($date_parts),
      ':00 -0400',
    );

    $log_format = implode('', $log_format_parts);
    $time = strtotime($log_format);
    return $time;
  }
}

