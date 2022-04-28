<?php

function media_embed_drush_command() {

  return array(
    'media-embed-domain-add' => array(
      'description' => dt('Adds a domain to the whitelist for HTML files.'),
      'options' => array(),
      'arguments' => array(),
      'examples' => array(
        'drush @prod me-da example.com' => dt('Allows html fragments that include references to example.com in javascript and iframe tags.'),
      ),
      'aliases' => array('me-da'),
    ),
    'media-embed-domain-remove' => array(
      'description' => dt('Removes a domain to the whitelist for HTML files.'),
      'options' => array(),
      'arguments' => array(),
      'examples' => array(
        'drush @prod me-dr example.com' => dt('Blocks HTML fragments that with script or iframe tags that point to example.com'),
      ),
      'aliases' => array('me-dr'),
    ),
    'media-embed-domain-view' => array(
      'description' => dt('View all currently whitelisted domains.'),
      'options' => array(),
      'arguments' => array(),
      'examples' => array(
        'drush @prod me-dv' => dt('Prints a list of all domains that users can use in HTML fragments.'),
      ),
      'aliases' => array('me-dv'),
    )
  );
}

function drush_media_embed_domain_add() {
  $args = func_get_args();
  $domains = variable_get('media_embed_whitelist', array());
  foreach ($args as $d) {
    $domains[] = $d;
  }

  // Filter out duplicates.
  $domains = media_embed_remove_domains_protocol($domains);
  $domains = array_unique($domains);
  variable_set('media_embed_whitelist', $domains);

  $output = dt("The following domains are now in your whitelist:\r\n").implode("\r\n", $args);
  drush_print($output);
}

function drush_media_embed_domain_remove() {
  $args = func_get_args();
  $domains = variable_get('media_embed_whitelist', array());
  $new_domains = array_diff($domains, $args);
  $removed = array_diff($domains, $new_domains);
  $excluded = array_diff($args, $removed);
  variable_set('media_embed_whitelist', $new_domains);

  $output = '';
  if (count($removed)) {
    $output .= dt("The following domains have been removed from your whitelist: \r\n").implode("\r\n", $removed)."\r\n";
  }
  if (count($excluded)) {
    $output .= dt("The following domains where not a part of your whitelist: \r\n").implode("\r\n", $excluded);
  }
  drush_print($output);
}

function drush_media_embed_domain_view() {
  $domains = media_embed_get_whitelist();
  if (count($domains)) {
    $output = dt("The following domains are in your whitelist: \r\n").implode("\r\n", $domains);
    $output .= "\r\n".dt("Note that some domains may be added by modules, and cannot be removed from the whitelist.");
  }
  else {
    $output = dt("There are no domains currently whitelisted.");
  }

  drush_print($output);
}
