<?php

/**
 * @file
 * Provides the purl_vsite_path PHP class.
 */

/**
 * Create the processor that will override the purl_path enables path + domain
 */
if (!class_exists('purl_vsite_path')) {

  /**
   *  Full path + domain handling for scholar
   */
  class purl_vsite_path extends purl_path implements purl_processor {

    public static $disabled = array();
    // Allows domain suffix to have alphanumeric chars, hyphens, underscores, minimum of 3 chararcters long
    public $modifier_regex = '!^[a-z0-9_-]{3,}+$!';

    public function method() {
      return PURL_VSITE_PATH;
    }

    /**
     * Checks path is valid
     */
    public function parse($valid_values, $q) {
      $valid_values = array_diff_key($valid_values, _vsite_domain_processed_purls());
      // Match case insensitive to lowercase.
      $parsed = parent::parse($valid_values, strtolower($q));

      $space_found = FALSE;
      // Does not return spaces without the correct domain
      foreach ($parsed as $site => $settings) {
        if ($settings->provider == 'spaces_og' && variable_get('purl_base_domain', FALSE)) {
          $vsite_domain = db_query("SELECT value FROM {spaces_overrides} WHERE type = :type AND id = :id AND object_id = 'vsite_domain_name' AND object_type = 'variable' ", array(':type' => 'og', ':id' => $settings->id,
          ))->fetchField();
          if ($vsite_domain) {
            $vsite_domain = unserialize($vsite_domain);
          }

          if (!is_string($vsite_domain)) {
            $vsite_domain = FALSE;
          }

          global $base_url;
          // HTTP_HOST includes port, but we want to remove it to make our comparisons with purl_base_domain.
          $http_hostname = explode(':', $_SERVER['HTTP_HOST'])[0];

          $on_base_domain = preg_replace('~https?://~', '', $http_hostname) == parse_url(variable_get('purl_base_domain', $base_url),PHP_URL_HOST);
          $clean_url = preg_replace('~https?://~', '', variable_get('purl_base_domain', $base_url));
          $temp = explode('/', $clean_url);
          $disabled_url = $space_found || in_array($settings->value, self::$disabled);
          $custom_domain = strlen($vsite_domain) && preg_replace('~https?://~', '', $http_hostname) != $vsite_domain;
          $host_url_match = !strlen($vsite_domain) && preg_replace('~https?://~', '', $http_hostname) != $temp[0];
          if ($disabled_url || $custom_domain || $host_url_match) {
            unset($parsed[$site]);
            if (!$space_found && $on_base_domain && strlen($vsite_domain) && count(explode('/', $q)) < 2) {
              purl_goto("<front>", array(
                'purl' => array(
                  'provider' => 'vsite_domain',
                  'id' => $settings->id,
                ),
              ));
            }
          }else{
            //We have a valid OG Space
            $space_found = TRUE;
          }
        }
      }

      return $parsed;
    }

    public function rewrite(&$path, &$options, $element) {

      $plugin = ctools_get_plugins('spaces', 'plugins', 'vsite');
      if ($plugin) {
        $class = ctools_plugin_get_class($plugin, 'handler');
      }
      // Does not alter excluded paths
      if (isset($class) && call_user_func(array($class, 'is_excluded_path'), $path)) {
        return;
      }

      // If we are asking for the absolute url then return the correct base url
      // for a path provider
      if (array_key_exists('absolute', $options) && $options['absolute']) {
        if ($element->id && $vsite = spaces_load('og', $element->id)) {
          $vsite_domain = $vsite->controllers->variable->get('vsite_domain_name','space');
          if(!$vsite_domain){
            $vsite_domain = $vsite->controllers->variable->get('vsite_domain_name','preset');
          }
        }

        if ($vsite_domain) {
          $options['base_url'] = "http://" . $vsite_domain;
        }
        // Checks for a group domain
        else {
          global $base_url;
          $options['base_url'] = variable_get('purl_base_domain', $base_url);
        }
      }

      // Disables addition of URL prefix if prefix is part of the path alias.
      if (!empty($path) && $path != '<front>') {
        $language = isset($options['language']) && isset($options['language']->language) ? $options['language']->language : '';
        $alias = drupal_get_path_alias($path, $language);
        if ($alias != $path && $alias != $this->remove($alias, $element)) {
          // The alias now contains the prefix of the viste, no need to rewrite.
          return;
        }
      }

      return parent::rewrite($path, $options, $element);
    }
  }
}
