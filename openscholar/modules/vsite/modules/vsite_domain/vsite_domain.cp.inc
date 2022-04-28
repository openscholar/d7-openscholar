<?php

/**
 * Implements hook_cp_settings().
 */
function vsite_domain_cp_settings() {

  if (!$vsite = vsite_get_vsite()) {
    return;
  }

  // Ensure we're in a vsite and it can take domains.
  $purl_types = array_keys(variable_get('purl_types', array()));
  if (!in_array('vsite_domain', $purl_types)) {
    return;
  }

  // Ensure we have access to change domains or we're in a subsite whose parent
  // provides domains
  $parent = (!empty($vsite->group->field_group_parent)) ? vsite_get_vsite($vsite->group->field_group_parent[LANGUAGE_NONE][0]['target_id']) : FALSE;
  if (!(($parent && $parent->group->field_subsite_domains) || user_access('change vsite domain'))) {
    return;
  }

  $template = array(
    'group' => array(
      '#id' => 'domain',
      '#title' => t('Custom Domain'),
      '#weight' => 0,
      '#group_validate' => 'vsite_domain_cp_settings_validate',
      '#help_link' => l(t('Learn more about custom domains'), 'https://help.theopenscholar.com/apply-a-custom-domain', array('attributes' => array('target' => '_blank')))
    ),
  );

  $settings = array('vsite_domain_name' => array(
    'group' => array(
      '#id' => 'domain',
      '#title' => t('Domain'),
      '#weight' => 0,
    ),
  ));
  $domain = variable_get('vsite_domain_name', '');
  $shared = variable_get('vsite_domain_shared', '');
  $purl = preg_replace('~https?://~', '', variable_get('purl_base_domain', ''));
  ctools_include('form', 'vsite_domain');

  // Get purl form and modify it
  $form = purl_form('vsite_domain', $vsite->group->nid, $domain);
  $form['value']['#title'] = t('Custom Domain Name (Do not include http://)');
  $form['value']['#field_prefix'] = 'http://';
  $form['value']['#required'] = FALSE;   // Leaving the vsite domain name field blank means use purl base domain.
  $form['#element_validate'] = array('vsite_domain_cp_settings_domain_validate');  // Use our validate function (which still invokes purl's validate).
  $form['sid'] = array(
    '#type' => 'hidden',
    '#value' => isset($vsite->group->nid) ? $vsite->group->nid : 0,
  );

  // Adds the option to share multiple vsites on one domain
  // If this option is selected, we actually do not make a purl record.
  $form['vsite_domain_shared'] = array(
    // Disable/check the box when no domain is set. Attempts to align with the actual allowed states.
    '#states' => array(
      'enabled' =>  array(':input[name="vsite_domain_name"]' => array('filled' => TRUE)),
      'disabled' => array(':input[name="vsite_domain_name"]' => array('filled' => FALSE)),
      'checked' =>  array(':input[name="vsite_domain_name"]' => array('filled' => FALSE)),
    ),
    '#type' => 'checkbox',
    '#title' => t('Share domain name'),
    '#default_value' => $shared,
    '#weight' => 10,
    '#description' => t("This will add \"<em><strong>/@site</strong></em>\" to the URL and share this domain name with other sites (default).<br/>If you own this custom domain, leave unchecked to use for just this site.", array('@site' => vsite_get_purl($vsite))),
  );

  //subsite domains - non-admins can only add domains provided by parent site
  if (spaces_access_admin() && !user_access('change vsite domain')) {
    $parent_domains = array_map(function($a) {return $a['value'];}, $parent->group->field_subsite_domains[LANGUAGE_NONE]);
    $form['value'] = array_merge($form['value'], array(
      '#type' => 'select',
      '#options' => array_merge(array('' => $purl), drupal_map_assoc($parent_domains)), // '' should clear the domain
      '#value' => ($domain) ? $domain : '',
      '#size' => null,
      '#description' => t('Change your site\'s domain name.'),
    ));

    $form['vsite_domain_shared'] = array_merge($form['vsite_domain_shared'], array(
      '#type' => 'hidden',
      '#value' => TRUE,
    ));

  } else {
    $form['value']['#access'] = user_access('change vsite domain');
  }

  $s['vsite_domain_name'] = $template + array(
      'form' => $form['value'],
      'submit' => array('vsite_domain_cp_settings_domain_submit'),
      'rest_submit' => function ($value) {
        vsite_domain_cp_settings_submit('vsite_domain_name', $value);
      },
    );

  $s['vsite_domain_shared'] = $template + array(
      'form' => $form['vsite_domain_shared'],
      'rest_submit' => function ($value) {
        vsite_domain_cp_settings_submit('vsite_domain_shared', $value);
      },
    );

  // If shared, makes sure to display the shared option as "checked".
  // Needed because checkbox #default_value = 1 does NOT show as checked.
  // @see http://drupal.org/node/104852
  if ($shared) {
    $s['vsite_domain_shared']['form']['#states']['checked'][':input[name="vsite_domain_name"]'] = array('enabled' => TRUE);
    $form['vsite_domain_shared']['#states']['checked'][':input[name="vsite_domain_name[value]"]'] = array('enabled' => TRUE);
  }

  $settings['vsite_domain_name']['form'] = $form;
  return $s;
}



/**
 * Validation function for restful domain submissions.
 *
 * This function runs from the restful handler, so should always run twice on any given request.
 * Once for the name and once for the shared checkbox.
 */
function vsite_domain_cp_settings_validate($values) {
  $domain = $values['vsite_domain_name'];
  $shared = $values['vsite_domain_shared'];

  if ($shared == 0) {
    if (empty($domain)) {
      return TRUE;
    }
    $modifier = array(
      'provider' => 'vsite_domain',
      'value' => $domain,
      'id' => $_GET['vsite'],
    );
    if (!purl_validate($modifier)) {
      t('There was an error registering the value "@value". It is either invalid or is already taken. Please choose another.', array('@value' => $domain));
      return FALSE;
    } else {

      // Finds any vsite_domain_name variables that match this domain.
      $result = db_select('spaces_overrides', 'so')
        ->fields('so', array('id'))
        ->condition('object_type', 'variable')
        ->condition('object_id', 'vsite_domain_name')
        ->condition('id', $sid, '!=')
        ->condition('value', $domain)
        ->execute()
        ->fetchAssoc();
      // Throws form_set_error() if any are found.
      if ($result) {
        $count = format_plural(count($result), '1 other site', '@count other sites');
        t('You may not own this domain. It is currently shared by @count.', array('@count' => $count));
        return FALSE;
      }
    }
  }
  // Validates shared domains:
  elseif ($domain) {
    // Checks this this domain isn't already owned by another site.
    $result = db_select('purl', 'p')
      ->fields('p', array('id'))
      ->condition('value', $domain)
      ->condition('id', $sid, '!=')
      ->execute()
      ->fetchAssoc();
    // Throws form_set_error() if another site owns this domain.
    if ($result) {
      t('You may not share this domain. Another site currently owns it.');
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Submit handler for this stuff
 */
function vsite_domain_cp_settings_submit($var, $value) {
  $vsite = vsite_get_vsite();
  $new_values = array(
    'group_path' => '',
    'domain' => array(
      'modified' => false,
      'original' => variable_get('vsite_domain_name', ''),
    ),
    'shared' => array(
      'modified' => false,
      'original' => variable_get('vsite_domain_shared', 0)
    ),
  );

  static $processed = array(
    'vsite_domain_name' => FALSE,
    'vsite_domain_shared' => FALSE,
  );

  switch ($var) {
    case 'vsite_domain_name':
      $processed['vsite_domain_name'] = $value;
      break;
    case 'vsite_domain_shared':
      $processed['vsite_domain_shared'] = $value;
      break;
  }

  // we've processed both values and can continue
  if ($processed['vsite_domain_name'] !== FALSE && $processed['vsite_domain_shared'] !== FALSE) {
    module_load_include('inc', 'vsite_domain', 'includes/form');

    if ($processed['vsite_domain_name'] == "") {
      $new_values['group_path'] = '[site:url]' . '/' . $vsite->og->purl;
      $new_values['shared']['new'] = 0;
    }
    else {
      $new_values['group_path'] = 'http://'.$processed['vsite_domain_name'];
      if ($processed['vsite_domain_shared']) {
        $new_values['group_path'] .= '/' . $vsite->og->purl;
      }
      $new_values['domain']['new'] = $processed['vsite_domain_name'];
      $new_values['shared']['new'] = $processed['vsite_domain_shared'];
    }

    $new_values['domain']['modified'] = $new_values['domain']['new'] != $new_values['domain']['original'];
    $new_values['shared']['modified'] = $new_values['shared']['new'] != $new_values['shared']['original'];

    _vsite_domain_set_domain($new_values, $vsite);
  }
}
