<?php

function vsite_domain_drush_command() {

  return array(
    'vsite-domain-site-clear' => array(
      'description' => dt('Clears a single site\'s clear cache'),
      'options' => array(),
      'arguments' => array(),
      'examples' => array(
        'drush @prod vd-sc 11' => dt('Clears the field caches for the site with nid 11'),
      ),
      'aliases' => array('vd-sc'),
    ),
  );
}

function drush_vsite_domain_site_clear() {
  $args = func_get_args();

  foreach ($args as $a) {
    if (is_numeric($a) && og_is_group('node', $a)) {
      module_load_include('inc', 'vsite_domain', 'includes/form');
      _vsite_domain_clear_field_caches($a);
    }
  }

  drush_print(dt('Field caches cleared.'));
}