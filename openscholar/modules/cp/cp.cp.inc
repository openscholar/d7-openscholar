<?php


/**
 * Implements hook_cp_settings().
 *
 * Allows user to switch between full and mini cp menus on the front view
 */
function cp_cp_settings() {
  $settings = array();

  if (module_exists('redirect')) {
    $rids = cp_redirect_get_redirects();
    $redirects = redirect_load_multiple(array_keys($rids));

    $redirect_values = array();
    foreach ($redirects as $r) {
      $redirect_values[] = array(
        'id' => $r->rid,
        'path' => $r->source,
        'target' => $r->redirect
      );
    }

    $settings['redirects'] = array(
      'group' => array(
        '#title' => t('URL Redirects'),
        '#id' => 'redirects',
        '#help_link' => l(t('Learn more about redirects'), 'https://help.theopenscholar.com/global-site-settings#url-redirects', array('attributes' => array('target' => '_blank')))
      ),
      'form' => array(
        '#title' => t('Manage redirects'),
        '#access' => module_exists('redirect') && redirect_access('create', 'redirect'),
        '#custom_directive' => 'redirects',
        '#value' => $redirect_values,
        '#maximum_value_count' => variable_get('cp_redirect_max', CP_REDIRECT_MAX_DEFAULT)
      ),
      'after_build' => array('_cp_settings_form_redirects_build'),
    );
    $settings['cp_redirect_max'] = array(
      'group' => array(
        '#title' => t('Max Redirects'),
        '#id' => 'max-redirects',
        '#help_link' => l(t('Learn more about redirects'), 'https://help.theopenscholar.com/global-site-settings#url-redirects', array('attributes' => array('target' => '_blank')))
      ),
      'form' => array(
        '#type' => 'textfield',
        '#title' => 'Number of maximum redirects',
        '#size' => 15,
        '#weight' => 2,
        '#default_value' => variable_get('cp_redirect_max', CP_REDIRECT_MAX_DEFAULT),
        '#element_validate' => array('element_validate_integer_positive'),
        '#access' => user_access('set maximum redirects'),
      ),
    );
  }

  if (module_exists('vsite') && module_exists('expire')) {
    $settings['cp_clear_cache'] = array(
      'group' => array(
        '#title' => t('Cache / Reindex'),
        '#id' => 'os-maintenance',
      ),
      'form' => array(
        '#title' => t('Clear site cache'),
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#description' => t('Clear the cache so website visitors see the most recent updates. This removes the normal ~15 minute delay for new/updated posts to show up, but can slow the site down for your visitors.'),
        '#submit' => array('cp_clear_site_cache'),
        '#access' => user_access('clear vsite cache'),
      ),
      'rest_trigger' => 'cp_clear_site_cache'
    );

    // Grant permission to those without it.
    if ($vsite = vsite_get_vsite()) {
      $settings['cp_clear_cache']['form']['#access'] = ($settings['cp_clear_cache']['form']['#access'] || og_user_access('node', $vsite->group->nid, 'clear site cache', NULL, FALSE, TRUE));
    }
  }

  return $settings;
}