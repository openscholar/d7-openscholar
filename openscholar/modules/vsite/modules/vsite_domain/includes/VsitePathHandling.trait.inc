<?php

trait VsitePathHandling {

  /**
   * Prevents the vsite from activating at all if the path is excluded.
   */
  function detect($q) {
    $plugin = ctools_get_plugins('spaces', 'plugins', 'vsite');
    if ($plugin) {
      $class = ctools_plugin_get_class($plugin, 'handler');
    }

    if (isset($class) && call_user_func(array($class, 'is_excluded_path'), $q)) {
      global $base_url;
      // Matches based on global base URL
      return $base_url;
    }
    else {
      $domain = parent::detect($q);

      // Removes the port if it is 80 (some browsers don't do this by default)
      if (substr($domain, -3) == ":80") {
        $domain = substr($domain, 0, (strlen($domain) - 3));
      }

      return $domain;
    }
  }

  /**
   * Triggered on hook_url_inbound_alter().
   *
   * Rewrites the given url to include the purl, so we can match against the url alias.
   *
   * We get urls in the form of <<domain>>/path/to/node, but the url alias needs to be
   * sitename/path/to/node in order to maintain uniquity. We need to add the sitename to the path given
   * in order to perform the alias lookup.
   *
   * Also, I just learned that uniquity was a real word.
   *
   * If a purl_path/domain has been set we should rewrite the current url
   * so that aliased paths will be picked up
   */
  public function adjust(&$value, $item, &$q) {
    if ((!($vsite = spaces_get_space()) && !($vsite = spaces_load('og', $item->id))) || !strlen($vsite->group->purl)) {
      return;
    }

    // Aliases query if it has not been aliased yet
    if (!in_array($vsite->group->purl, _vsite_domain_processed_purls())) {
      _vsite_domain_processed_purls($vsite->group->purl);

      $args = explode('/', trim($_GET['q'], '/'));

      // Prepends the value to the front of the query string
      if (current($args) !== (string) $vsite->group->purl) {
        array_unshift($args, $vsite->group->purl);
      }

      // String with purl added
      $qstring = implode('/', $args);
      // Alias of that url
      $qstring_alias = drupal_lookup_path('source', $qstring);

      // Modifies $q since purl/path/path has an alias
      if ($qstring_alias && $qstring != $qstring_alias) {
        $q = $qstring;
      }
    }
  }

  /**
   * Triggered on hook_url_outbound_alter().
   *
   * Strips the purl from the alias.
   *
   * Either force the url, or set it back to the base.
   */
  public function rewrite(&$path, &$options, $element) {
    $plugin = ctools_get_plugins('spaces', 'plugins', 'vsite');
    if ($plugin) {
      $class = ctools_plugin_get_class($plugin, 'handler');
    }

    // Doesn't alter if link destination is an excluded path
    if (isset($class) && call_user_func(array($class, 'is_excluded_path'), $path)) {
      return;
    }

    $options['absolute'] = TRUE;
    if (!_purl_skip($element, $options)) {
      $path = $this->remove_scholar_purl($path, $options);
      $options['base_url'] = $this->rewriteBaseUrl($element->value);
    }
    elseif (!isset($options['base_url'])) {
      //If purl_skip was triggered and no base_url has been set manually then set as purl_base_domain
      global $base_url;
      $options['base_url'] = variable_get('purl_base_domain', $base_url);
    }
  }

  abstract protected function rewriteBaseUrl($url);

  /**
   * Strips the purl from the path, if applicable
   *
   * We do this for urls being printed to the page, so users don't see customdomain.com/sitename/path/to/node
   */
  private function remove_scholar_purl($path, &$options) {
    if (!($vsite = spaces_get_space()) || !strlen($vsite->group->purl)) {
      return $path;
    }

    // Removes URL prefix if prefix is part of the path alias
    if (!empty($path) && $path != '<front>' && !$options['alias']) {
      $language = isset($options['language']) && isset($options['language']->language) ? $options['language']->language : '';

      //Get the stored alias.
      $alias = drupal_get_path_alias($path, $language);
      $args = explode('/', $alias);

      //If an alias exists and is prefixed with the purl value then remove it.
      if ($alias != $path && current($args) === (string) $vsite->group->purl) {
        // Removes the value from the front of the query string.
        array_shift($args);
        $options['alias'] = true;
        return implode('/', $args);
      }
    }

    return $path;
  }
}