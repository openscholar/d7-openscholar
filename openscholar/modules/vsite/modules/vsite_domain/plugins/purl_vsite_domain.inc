<?php
// $Id$

/**
 * Create the processor that will override the purl_domain
 */
if (!class_exists('purl_vsite_domain')) {
  /**
   *  Full domain handling for scholar, will also remove purl prefix's from alias's
   */
  class purl_vsite_domain extends purl_domain implements purl_processor {
    use VsitePathHandling;

    public $modifier_regex = '!^[a-z0-9_\.-]+$!';

    /**
     * Simply matches our 'q' (aka domain) against an allowed value.
     *
     * If none is found and $q starts with www. then try without
     */
    public function parse($valid_values, $q) {
      $path_elements = parent::parse($valid_values, $q);

      if (!count($path_elements) && strpos($q, 'www.') === 0) {
        // Removes "www"
        $q = substr($q, 4);
        $path_elements = parent::parse($valid_values, $q);
      }

      return $path_elements;
    }

    public function method() {
      return PURL_VSITE_DOMAIN;
    }

    public function description() {
      return t('Enter a domain registered for this site, such as "www.example.edu".');
    }

    protected function rewriteBaseUrl($url) {
      return "http://$url";
    }
  }
}

