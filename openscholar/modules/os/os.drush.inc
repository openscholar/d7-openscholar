<?php

/**
 * @file os.drush.inc
 *
 * Drush commands provided by OpenScholar
 */

function os_drush_command() {
  $path_options = array(
    'htaccess-file' => dt('Override path to .htaccess file.'),
    'domains-file' => dt('Override path to domains.list file.'),
    'sites-inc-file' => dt('Override path to sites.php.inc file.'),
    'domains-file-root' => dt('If your domains.list files are outside of your Drupal root path, use this option to specify their parent folder.')
  );

  return array(
    'os-domain-check' => array(
      'description' => dt('Checks that files exist and permissions are correct for running drush os-domain-* scripts.'),
      'options' => $path_options,
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-dc' => dt('Enable production site to use example.com domain.  The owner of example.com will still have to add that domain to her or her vsite.'),
      ),
      'aliases' => array('os-dc'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    ),

    'os-domain-build' => array(
      'description' => dt('Build\'s os-domains sites.php, .htaccess, and (optionally) acquia cloud based on current lists of domains.'),
      'options' => array_merge($path_options, array('domain-name-prefix' => dt('A value to prefix all domain names with.  Ex. "dev".'))),
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-db' => dt('Builds new copise of sites.php and .htaccess.  If acquia cloud API is available, also update their registry of domains.'),
        'drush @prod os-db --domains-file=/var/www/alt/path/mysite/domains.list --domains-file-root=/var/www/alt/path/' => dt('As os-db, but uses the file specified instead of the domains.list found in the sites directory.  File must be "domains.list" but path can be anywhere.  The domains-file-path instructs drush that other sites in a multisite setup may live in /var/www/alt/path and it should be searched for domains.list files.'),
      ),
      'aliases' => array('os-db'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    ),

    'os-domain-add' => array(
      'description' => dt('Add a domain to a site.'),
      'options' => array( 'www' => dt('Add domain and www.domain')) + $path_options,
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-da -www example.com' => dt('Sets up @prod site to serve example.com and www.example.com.'),
      ),
      'aliases' => array('os-da'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    ),

    'os-domain-remove' => array(
      'description' => dt('Remove a domain from a site.'),
      'options' => array( 'www' => dt('Add domain and www.domain')) + $path_options,
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-dr -www example.com' => dt('Sets up @prod site to stop serving example.com and www.example.com.'),
      ),
      'aliases' => array('os-dr'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    ),

    'os-domain-help' => array(
      'description' => dt('Lists domains managed by drush os-domain.'),
      'options' => array(),
      'arguments' => array(),
      'aliases' => array('os-dh'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    ),

    'os-domain-list' => array(
      'description' => dt('Lists domains managed by drush os-domain.'),
      'options' => $path_options,
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-dl' => dt('Lists @prod\s domains.'),
      ),
      'aliases' => array('os-dl'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    ),

    'os-domain-sync' => array(
      'description' => dt('Syncs acquia managed domains into domains.list file.'),
      'options' => $path_options,
      'arguments' => array(),
      'examples' => array(
        'drush @prod os-ds' => dt('Recreats domains list file from acquia\'s settings.'),
      ),
      'aliases' => array('os-ds'),
      'bootstrap' => DRUSH_BOOTSTRAP_NONE,
    ),
  );
}

/**
 * @function _drush_os_domain_get_server_conf()
 *
 * Gets common vars used in os-domain drush commands
 */
function _drush_os_domain_get_server_conf() {
  $status = drush_sitealias_get_record('@self');
  $root = $status['root'];
  $context = drush_get_context();

  $server_conf = array(
    'root' => $root,
    'uri' => $status['uri'],
    'base_path' => $root . '/' . $context[DRUSH_DRUPAL_SITE_ROOT],
    'files_path' => $root . '/files',
    'acquia-cloud-api' => (isset($context[DRUSH_COMMANDS]['ac-domain-add'])),
    'sitesphp' => $root . '/sites/sites.php',

    'htaccess_file' => drush_get_option('htaccess-file', "$root/files/.htaccess"),
    'domains_list' => drush_get_option('domains-file', "$root/{$context[DRUSH_DRUPAL_SITE_ROOT]}/domains.list"),
    'sites_inc' => drush_get_option('sites-inc-file', "$root/sites/sites.php.inc"),
    'domains_root' => drush_get_option('domains-file-root', "$root/sites/"),
  );

  return $server_conf;
}

/**
 * @function drush_os_domain_build
 *
 * Service hook for os-domain-build
 *
 * Parses domains.list files into .htaccess, sites.php.inc, and (optionally) acquia cloud domain config
 */
function drush_os_domain_build() {
  $srv_conf = _drush_os_domain_get_server_conf();
  drush_invoke('os-domain-check');
  $prefix = drush_get_option('domain-name-prefix', FALSE);


  /* build list of multisites and their domains */
  $sites = $srv_conf['domains_root'];
  $multisites = array();
  foreach (scandir($sites) as $site) {
    if (is_dir($sites.$site) && file_exists($sites.$site.'/domains.list')) {
      $domains = file($sites.$site.'/domains.list');
      $domains = array_unique($domains);
      $domains = array_map('trim', $domains);
      $domains = preg_grep('/^[^#].*$/', $domains); //strip out comments
      if ($prefix) {
        foreach ($domains as $key => $domain) {
          $domains[$key] = $prefix . "." . $domain;
        }
      }
      $multisites[$site] = $domains;
    }
  }


  /* flatten the multisites array for sites.php */
  $a_sites = array();
  foreach ($multisites as $site => $domains) {
    $domains = array_map(function ($v) {
      return trim($v, '*.');
    }, $domains);
    $a_sites += array_fill_keys($domains, $site);
  }
  $inc_file= $srv_conf['sites_inc'];
  $export = "<?php\n\n\$sites = " . var_export($a_sites, TRUE) . ";\n";
  _os_drush_confirm_file_overwrite($export, $inc_file);


  /* build .htaccess files */
  $htaccess = _os_drush_domain_htaccess($multisites, $prefix);
  _os_drush_confirm_file_overwrite($htaccess, $srv_conf['htaccess_file']);


  /* try acquia cloud api. */
  if ($srv_conf['acquia-cloud-api']) {
    //acquia doesn't differentiate multisites.  this list is all or nothing.
    $all_domains = array_reduce($multisites, function ($ret, $arr) {
      return array_merge($ret, $arr);
    }, array());

    ob_start();
    $response = drush_invoke('ac-domain-list');
    $raw = ob_get_clean();

    if ($response['error_status']) {
      drush_die('Could not connect to acquia cloud api');
    }

    $raw = preg_replace("[\n\r]", "\n", $raw);
    $raw = explode("\n", $raw);
    $ac_domain_list = array();
    foreach ($raw as $d) {
      if (preg_match('|name[\s]+:|', $d)) {
        $ac_domain_list[] = trim(preg_replace('|name[\s]+:[\s]+|', "", $d));
      }
    }

    //$ac_domain_list = array_map(create_function('$a', 'return $a["name"];'), $ac_domain_list['object'][1]);
    $add = array_diff($all_domains, $ac_domain_list);
    array_pop($ac_domain_list); //last item is original domain and cannot be removed.
    $del = array_diff($ac_domain_list, $all_domains);

    if ($add && (drush_confirm('Add domains to acquia config: ' . implode(', ', $add) . '?'))) {
      foreach ($add as $domain) {
        drush_invoke_process(drush_sitealias_get_record('@self'), 'ac-domain-add', array($domain));
      }
    }

    if ($del && (drush_confirm('Remove domains from acquia config: ' . implode(', ', $del) . '? '))) {
      foreach ($del as $domain) {
        drush_invoke_process(drush_sitealias_get_record('@self'), 'ac-domain-delete', array($domain));
      }
    }
  }
}

/**
 * @function drush_os_domain_add
 *
 * Service hook for os-domain-add
 *
 * Adds domain(s) to domain listings, then calls rebuild.
 */
function drush_os_domain_add() {
  $args = func_get_args();
  $srv_conf = _drush_os_domain_get_server_conf();
  $www = drush_get_option('www', FALSE);
  $self = drush_sitealias_get_record('@self');

  $check = drush_invoke_process($self, 'os-domain-check');
  if ($check['error_status']) {
    drush_die();
  }

  foreach ($args as $domain) {
    $domain = (count($args)) ? str_replace('http://', '', $args[0]) : drush_die('drush os-domain-config requires a domain.  See drush odc --help');

    //make sure domain is valid and correct
    if (drush_prompt("Are you sure you want to add '$domain'? (y/n)", 'n') != 'y') {
      drush_die('User bailed out');
    }

    if ($www) {
      $domain = $domain . "\nwww.".$domain;
    }

    //append domain to list
    if (!$fd = fopen($srv_conf['domains_list'], 'a+')) {
      drush_die(dt("ERROR: fopen(@file, 'ab') failed", array('@file' => $srv_conf['domains_list'])));
    }
    if (!fwrite($fd, "\n".$domain)) {
      drush_die(dt("ERROR: fwrite(@file) failed", array('@file' => $srv_conf['domains_list'])) . '<pre>' . $domain);
    }
  }

  drush_invoke('os-domain-build'); //@todo - prevent --www from causing superfluous unknown optoin error.
}

/**
 * @function drush_os_domain_remove
 *
 * Command callback for os-domain-remove
 *
 * Removes domains from domain list, then rebuilds
 */

function drush_os_domain_remove() {
  $args = func_get_args();
  $srv_conf = _drush_os_domain_get_server_conf();
  $www = drush_get_option('www', FALSE);
  $self = drush_sitealias_get_record('@self');

  $check = drush_invoke_process($self, 'os-domain-check');
  if ($check['error_status']) {
    drush_die();
  }

  // read current domains
  $domains = file($srv_conf['domains_list']);
  $domains = array_map(function ($s) {
    return trim($s);
  }, $domains);
  
  //prepend www. 
  if ($www) {
    $args = array_merge($args, array_map(function ($s) {
      return "www." . $s;
    } ,$args));
  }

  //diff and write
  $keep_domains = array_diff($domains, $args);
  file_put_contents($srv_conf['domains_list'], implode("\n", $keep_domains));

  drush_invoke('os-domain-build');
}

/**
 * @function drush_os_domain_list
 *
 * Command callback for os-domain-list
 **/
function drush_os_domain_list() {
  drush_invoke('os-domain-check');
  $args = func_get_args();
  $srv_conf = _drush_os_domain_get_server_conf();
  $file = file($srv_conf['domains_list']);
  drush_print(implode('', preg_grep('/^[^#]/', $file)));
}

/**
 * @function drush_os_domain_sync
 *
 * Command callback for os-domain-sync
 **/
function drush_os_domain_sync() {
  drush_invoke('os-domain-check');
  $args = func_get_args();
  $srv_conf = _drush_os_domain_get_server_conf();
  $domains_list = $srv_conf['domains_list'];

  ob_start();
  $ac_domain_list = drush_invoke_process(drush_sitealias_get_record('@self'), 'ac-domain-list', array(), array() );
  ob_get_clean();

  if ($ac_domain_list['error_status']) {
    drush_die('Could not connect to acquia cloud api');
  }

  $ac_domain_list = array_map(create_function('$a', 'return $a["name"];'), $ac_domain_list['object'][1]);
  _os_drush_confirm_file_overwrite(implode("\n", $ac_domain_list), $domains_list);
}

/**
 * @function drush_os_domain_check
 *
 * Command callback for os-domain-check
 *
 * Checks configuration and bails out if drush os-domain commands aren't going to work
 */
function drush_os_domain_check() {
  $srv_conf = _drush_os_domain_get_server_conf();

  //check that domains.list has filename=domains.list
  if (end(explode('/', $srv_conf['domains_list'])) != 'domains.list') {
    drush_die('--domains-file option must point to a domains.list file');
  }

  //check for domains.list
  if (!file_exists($srv_conf['domains_list'])) {
    $url = str_replace('http://', '', $srv_conf['uri']);
    $url = trim($url, '/');
    file_put_contents($srv_conf['domains_list'], "# This file created automatically by drush os-domain-config.  Do not delete.\n") || drush_die('Error creating domains.list.  Check your permissions.');
    file_put_contents($srv_conf['domains_list'], $url, FILE_APPEND);
  }

  //do we have sites.php?
  if (!file_exists($srv_conf['sitesphp'])) {
    drush_die('sites.php is missing.  Please create a sites.php file to use os-domain-config.');
  }

  //htaccess.
  if (!is_writable($srv_conf['htaccess_file'])) {
    drush_die('Specified htaccess file does not exist or cannot be written to: ' . $srv_conf['htaccess_file']);
  }

  //check if sites.php has our include line
  $contents = file_get_contents($srv_conf['sitesphp']);
  $inc_file = array_pop(explode('/', $srv_conf['sites_inc']));

  if (!(strpos($contents, 'require_once') && strpos($contents, $inc_file))) {
    if ($srv_conf['sites_inc'] == "{$srv_conf['root']}/sites/sites.php.inc") {
      //lets default to not hardcoding the path...
      $required = <<<EOF
    
// Read in include files from our domains.
\$dir = dirname(__FILE__);
if (file_exists(\$dir . '/sites.php.inc')) {
  require_once(\$dir . '/sites.php.inc');
}
    
EOF;
    } else {
      //hardcode in the user's custom path
      $path = $srv_conf['sites_inc'];
      $required = <<<EOF
      
// Read in include files from our domains.
if (file_exists("$path")) {
  require_once("$path");
}
      
EOF;

    }
    drush_die("sites.php exists, but is not configured to include our domain overrides.  Please add the following code (or something vaguely resembling it) before \$sites is initalized.\n" . $required);
  }

  //can we use our files dirs' .htaccess?
  if (!is_dir($srv_conf['files_path'])) {
    drush_die(dt("Path to .htaccess does not exist.  Create it or provide another path.\n @path", array('@path' => $srv_conf['files_path'])));
  }

}

/**
 * @function os_drush_domain_help()
 *
 * Some text describing drush os-domains
 */
function drush_os_domain_help() {
  $output = <<<EOF
  
drush os-domains-*
------------------

These drush scripts manage OpenScholar's domain configuration.  Users that add their
own domains to cp/settings need drupal to recognize those domains.  This script modifies:
  * `domains.list` stores lists of domains per installation
  * `sites.php` tell each drupal installation which domains it uses
  * `files/.htaccess` rewrite file urls
  * (when available) `Acquia's Cloud API`.  Configures acquia hosted virtual sites to use a given domain.  
    Sites hosted elsewhere will need to manage their own apache configurations.

Using os-domains* consists of:
  * `os-domains-list` Get the list of domains managed by these scripts
  * `os-domains-add` Appends a domain to the list, rebuilds config files 
  * `os-domains-remove` Removes a domain from the list, rebuilds config files 
  * `os-domains-build` Builds config files from list without modifying it 
  * `os-domains-check` Confirms that config files are accessible and set up for these scripts
  * `os-domains-sync` Sync domains.list from acquia's current state.  domains.list will be thrown out.
  * `os-domains-help` This documentation
      
To override the location of the files mentioned above, use the --{domains,htaccess,sites}-file= option
to point to another location.  These options can be added to a drush alias file so you don't have to
type them each time.  Be careful not to use the same domains.list for different sites!  Also note that if 
--domains-file is pointing to a location outside of your Drupal root's sites directory, --domains-file-root
must be used so your multisites can be found.
        
EOF;
  drush_print($output);
}

/**
 * @function _os_drush_confirm_file_overwrite($contents, $destination) {
 *
 * Copies contents to a temp file.  Prompts user to preview, copy, or quit.  Only overwrites after user confirmation.
 */
function _os_drush_confirm_file_overwrite($contents, $destination) {
  list($filename) = array_reverse(explode('/', $destination));
  $temp = tempnam('/tmp', 'drush-odc-'.$filename.'-');
  file_put_contents($temp, $contents);


  $action = 'u';
  $prompt_loop = !drush_get_context('DRUSH_AFFIRMATIVE'); //skip the prompt and use the new files when -y is enabled
  while ($prompt_loop) {
    $action = drush_prompt(dt('Created @filename file.  [v]iew, [d]iff, [u]se this file, or [q]uit?', array('@filename' => $filename)));
    switch ($action) {
      case 'v':
        drush_shell_exec_interactive('less ' . $temp);
        break;

      case 'd':
        drush_shell_exec_interactive("diff $destination $temp");
        break;

      case 'u':
        $prompt_loop = FALSE;
        break;

      case 'q':
        drush_die('User bailed out.');
        break;
    }
  }
  // copy the file.
  rename($destination, $destination . date('-\o\l\d-U'));
  drush_print("copying $temp to $destination");
  copy($temp, $destination);
}

/**
 * @function _os_drush_domain_htaccess($multisites)
 *
 * Builds .htaccess file for all multisites in array.
 */
function _os_drush_domain_htaccess($multisites, $prefix) {
  $head =<<<EOF
# This .htaccess file automatically built by drush os-domain-config  
      
<IfModule mod_rewrite.c>
  RewriteEngine on

  # Note: this will redirect to the /sites subfolder which is identical to
  # the domain accessed by the browser.  If you use partial domain names as
  # the /sites subfolder (e.g. /sites/domain.com vs /sites/www.domain.com)
  # you will need to modify the Drupal root .htaccess to remove the leading www
  # or modify the following to use the protion of %{HTTP_HOST} that mirrors
  # your /sites subfolders

  
EOF;

  $foot =<<<EOF
  
  # If the domain does not have a dedicated /sites/<domain>/files folder allow the
  # default folder to catch it.
  #  RewriteCond %{REQUEST_FILENAME} !-f
  #  RewriteCond %{REQUEST_FILENAME} !-d
  # To redirect the URI to the actual location of the file /sites/default/files/<filename>
  # change the [L] to [L,R]
  #  RewriteRule ^(.*)$ /sites/default/files/$1 [L]

</IfModule>      

SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006
Options None                                                                                                                                                                                                                                   
Options +FollowSymLinks

EOF;

  $domain_rules = array();
  foreach ($multisites as $site => $domains) {
    $domain_rules[] = _os_drush_domain_htaccess_domain_rules($site, $domains, $prefix);
  }

  return $head . implode("\n", $domain_rules) . $foot;
}

/**
 * @function _os_drush_domain_htaccess_domain_rules($site, $domains)
 *
 * Generates htaccess rules for $site containing $domains[]
 */
function _os_drush_domain_htaccess_domain_rules($site, $domains, $prefix) {

  $rules = '';

  if ($prefix) {
    $prod_domain = trim(substr(reset($domains), strlen($prefix)+1), '*');
    $rules .= <<<EOF
  # REWRITE FOR IMAGE STYLES THAT HAVE LOCAL FILE IN PLACE
  RewriteCond %{REQUEST_FILENAME} !-f                                                                                  
  RewriteCond %{REQUEST_FILENAME} !-d 
  # Force image styles that have local files that exist to be generated.
  RewriteCond %{REQUEST_URI} ^\/files\/styles\/[^\/]*\/public\/((.*))$
  RewriteCond %{DOCUMENT_ROOT}/sites/{$site}/files/%2 -f 
EOF;

    $rules .= _os_drush_domain_htaccess_rewrite_condition($domains);
    $rules .= '  RewriteRule ^(.*)$ /sites/' . $site . '/files/$1 [L]' . "\n";

    $rules .= <<<EOF
  # REWRITE FOR FILES THAT ARE NOT IN LOWER ENV
  RewriteCond %{REQUEST_FILENAME} !-f                                                                                  
  RewriteCond %{REQUEST_FILENAME} !-d 
  RewriteCond %{REQUEST_URI} !^(.*?)\/files\/css\/.*$
  RewriteCond %{REQUEST_URI} !^(.*?)\/files\/js\/.*$
  RewriteCond %{REQUEST_FILENAME} ^(.*?)\/files\/
  RewriteCond %{DOCUMENT_ROOT}/sites/{$site}/%{REQUEST_URI} !-f
EOF;

    $rules .= _os_drush_domain_htaccess_rewrite_condition($domains);
    $rules .= '  RewriteRule ^(.*)$ https://' . $prod_domain . '/files/$1 [QSA,L]' . "\n";

  }

  $rules .= <<<EOF
  
  RewriteCond %{REQUEST_FILENAME} !-f                                                                                  
  RewriteCond %{REQUEST_FILENAME} !-d 
  RewriteCond %{REQUEST_FILENAME} ^(.*?)\/files\/ 
EOF;

  $rules .= _os_drush_domain_htaccess_rewrite_condition($domains);
  $rules .= '  RewriteRule ^(.*)$ /sites/' . $site . '/files/$1 [L]' . "\n";
  return $rules;
}

/**
 * @function _os_drush_domain_htaccess_rewrite_condition($domains)
 *
 * Given a list of domains, returns htaccess rewrite conditions matching all of them.
 */
function _os_drush_domain_htaccess_rewrite_condition($domains) {
  $domains_raw = $domains;
  foreach ($domains as $n => $domain) {
    $subdomain = '^';
    if (strpos($domain, '*') === 0) {
      $subdomain = '';
    }
    $condition = '  RewriteCond %{HTTP_HOST} ' . $subdomain . ltrim($domain, '*.');
    $domains[$n] = str_replace('.', '\.', $condition);
  }
  $rule = "\n" . implode(" [OR]\n", $domains) . "\n";

  return $rule;
}
