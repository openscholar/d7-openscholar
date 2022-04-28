<?php
/**
 * @file
 * vsite.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function vsite_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_sitewide';
  $context->description = 'Context that is active on all non-vsite pages except the frontpage.';
  $context->tag = 'OpenScholar';
  $context->conditions = array(
    'context' => array(
      'values' => array(
        '~os_sitewide_front' => '~os_sitewide_front',
        '~os_users' => '~os_users',
        '~os_cp' => '~os_cp',
      ),
    ),
    'path' => array(
      'values' => array(
        '~<front>' => '~<front>',
        '~user' => '~user',
        '~user/*' => '~user/*',
        '~admin/*' => '~admin/*',
        '~cp/*' => '~cp/*',
        '~site/register' => '~site/register',
      ),
    ),
    'os_site' => array(
      'values' => array(
        0 => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context that is active on all non-vsite pages except the frontpage.');
  t('OpenScholar');
  $export['os_sitewide'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_sitewide_front';
  $context->description = 'Active on the frontpage in multi-tenant installs';
  $context->tag = 'OpenScholar';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
    'os_site' => array(
      'values' => array(
        0 => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'os_search_solr-solr_search_box' => array(
          'module' => 'os_search_solr',
          'delta' => 'solr_search_box',
          'region' => 'header_third',
          'weight' => '-10',
        ),
        'os_search_db-site-search' => array(
          'module' => 'os_search_db',
          'delta' => 'site-search',
          'region' => 'header_third',
          'weight' => '-10',
        ),
        'vsite_register-get_your_site_block' => array(
          'module' => 'vsite_register',
          'delta' => 'get_your_site_block',
          'region' => 'sidebar_second',
          'weight' => '101',
        ),
        'os-install_status' => array(
          'module' => 'os',
          'delta' => 'install_status',
          'region' => 'footer',
          'weight' => '10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Active on the frontpage in multi-tenant installs');
  t('OpenScholar');
  $export['os_sitewide_front'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'os_users';
  $context->description = 'Context that is active when in a user area of an Openscholar site, like user profile or log in screen';
  $context->tag = 'OpenScholar';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'user' => 'user',
        'users' => 'users',
        'user/*' => 'user/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Context that is active when in a user area of an Openscholar site, like user profile or log in screen');
  t('OpenScholar');
  $export['os_users'] = $context;

  return $export;
}
