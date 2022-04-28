<?php

/**
 * @file
 * Form validate and submit callbacks that only happen for User 1.
 */

/**
 * Form validate callback; adds purl validation if domain value is set.
 */
function vsite_domain_cp_settings_domain_validate($form, $form_state) {

  // Extracts the important values from the form.
  $domain = $form_state['values']['vsite_domain_name']['value'];
  $shared = $form_state['values']['vsite_domain_name']['vsite_domain_shared'];
  $sid = $form['sid']['#value'];

  // Validates owned domains:
  if ($shared == 0) {
    // First, exits if purl_form_validate() fails.
    if (strlen($form['value']['#value'])) {
      if (!purl_form_validate($form)) {
        // purl_form_validate() already handles form_set_error() for duplicate
        // domain names and a few other things.
        return FALSE;
      }
    }
    // Checks that this domain isn't already shared by other sites.
    if ($domain) {
      // Finds any vsite_domain_name variables that match this domain.

      // TODO: Fix this query.
      // It doesn't work at all. It always returns nothing because the value field is serialized, and we're not
      // serializing $domain. There are some edge cases involving shared domains and what not, or I would fix it right now.
      $result = db_select('spaces_overrides', 'so')
        ->fields('so', array('id'))
        ->condition('object_type', 'variable')
        ->condition('object_id', 'vsite_domain_name')
        ->condition('type', 'og')
        ->condition('id', $sid, '!=')
        ->condition('value', $domain)
        ->execute()
        ->fetchAssoc();
      // Throws form_set_error() if any are found.
      if ($result) {
        $count = format_plural(count($result), '1 other site', '@count other sites');
        form_set_error($form['#parents'][0], t('You may not own this domain. It is currently shared by @count.', array('@count' => $count)));
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
      form_set_error('vsite_domain_shared', t('You may not share this domain. Another site currently owns it.'));
    }
  }

  return TRUE;
}

/**
 * Form submit callback; updates purl table when vsite_domain_name changes.
 */
function vsite_domain_cp_settings_domain_submit(&$form, &$form_state) {
  // Note: this submit callback is only added to the CP > Settings form for
  // users with the "change vsite domain" permission -- currently only User 1.
  // Additionally, costly functions first check that values have been modified
  // in order to avoid needless expensive cache flushing & database operations.

  // Checks to see if there is a new group path field value for the vsite node.
  $vsite = vsite_get_vsite();
  $new_values = _vsite_domain_process_new_values($form, $form_state, $vsite);

  // Does nothing else if there is no change to the existing values.
  if ($new_values) {
    _vsite_domain_set_domain($new_values, $vsite);
  }
}

function _vsite_domain_set_domain($new_values, $vsite) {

  // Otherwise, sets the new field value on the vsite node.
  vsite_domain_set_group_path($new_values['group_path'], $vsite);

  // Then, finds what actions are necessary to perform.
  $actions = _vsite_domain_submit_actions($new_values);

  // Invokes purl_save() or purl_delete() if necessary.
  if ($actions['callback']) {
    $callback = $actions['callback'];
    $modifier = array(
      'value' => $actions['value'],
      'provider' => PURL_VSITE_DOMAIN,
      'id' => $vsite->group->nid,
    );
    call_user_func_array($callback, array($modifier));
  }

  if ($new_values['domain']['modified']) {
    $vsite->controllers->variable->set('vsite_domain_name', $new_values['domain']['new']);
  }
  if ($new_values['shared']['modified']) {
    $vsite->controllers->variable->set('vsite_domain_shared', $new_values['shared']['new']);
  }

  _vsite_domain_clear_field_caches($vsite->id);

  // Clears the specified cache if necessary (i.e. "purl_modifiers_en" cid).
  if ($actions['cache']) {
    try {
      cache_clear_all($actions['cache']);
    }
    catch (Exception $e) {
      drupal_set_message('Couldn\'t clear cache.');
    }
  }

  watchdog('vsite_domain', "actions, new values: \n@a\n@v\n", array('@a' => json_encode($actions), '@v' => json_encode($new_values)));
}


/**
 * Returns an array of necessary actions for these new values, or FALSE.
 */
function _vsite_domain_submit_actions($new_values) {
  $domain = $new_values['domain'];
  $shared = $new_values['shared'];
  $group_path = $new_values['group_path'];

  // Determines what kind of domain change scenario is taking place.
  // Each domain scenario has at least one action required: a purl callback,
  // a cache clear, and/or a form redirect.
  $domain_change = _vsite_domain_get_domain_change($domain, $shared);
  // Returns an array of the required actions for this domain change scenario.
  $actions = _vsite_domain_change_actions($domain_change, $domain, $group_path);

  return $actions;
}

/**
 * Returns the type of domain change occuring, or FALSE if none.
 *
 * @param array $domain
 *   An indexed array representing the vsite_domain_name var, containing:
 *   - 'modified': (bool) TRUE only if the original and new values differ.
 *   - 'original': (string) the original value from the form, i.e. "domain.com"
 *   - 'new': (string) the new value from the form, i.e. "domain.com"
 * @param array $shared
 *   An indexed array representing the vsite_domain_shared var, containing:
 *   - 'modified': (bool) TRUE only if the original and new values differ.
 *   - 'original': (string) the original value from the form, accepts '1' or '0'
 *   - 'new': (string) the new value from the form, accepts '1' or '0'
 *
 * @return mixed
 *   FALSE if no domain name change is occuring, or one of these 10 strings:
 *     'standard to owned'
 *     'standard to group'
 *     'owned to standard'
 *     'owned to group'
 *     'group to standard'
 *     'group to owned'
 *     'one owned to another owned'
 *     'one owned to another group'
 *     'one group to another owned'
 *     'one group to another group'
 */
function _vsite_domain_get_domain_change($domain, $shared) {
  // If the site is changing from "standard" domain setting...
  if (!$domain['original']) {
    // Note: Only the 2 distinct cases below match the above condition.

    // Case A: Going from standard to owned custom domain.
    if (!$shared['new']) {
      // Example: [site:url]/foo -> http://foo.com
      return 'standard to owned';
    }
    // Case B: Going from standard to group (shared) custom domain.
    else {
      // Example: [site:url]/foo -> http://foo.com/foo
      return 'standard to group';
    }
  }

  // If the site is making any other change with at most 1 custom domain...
  elseif ($domain['new'] xor $domain['modified']) {
    // Note: Only the 4 distinct cases below match the above condition.

    // Case C: Going from owned custom domain to standard.
    if (!$shared['original'] && !$domain['new']) {
      // Example: http://foo.com -> [site:url]/foo
      return 'owned to standard';
    }
    // Case D: Going from owned domain to group (shared) domain.
    elseif (!$shared['original'] && $domain['new']) {
      // Example: http://foo.com -> http://foo.com/foo
      return 'owned to group';
    }
    // Case E: Going from to group (shared) domain to standard.
    elseif ($shared['original'] && !$domain['new']) {
      // Example: http://foo.com/foo -> [site:url]/foo
      return 'group to standard';
    }
    // Case F: Going from to group (shared) domain to owned.
    elseif ($shared['original'] && $domain['new']) {
      // Example: http://foo.com/foo -> http://foo.com
      return 'group to owned';
    }
  }

  // If the change involves more than 1 custom domain...
  elseif ($domain['original'] && $domain['new'] && $domain['modified']) {
    // Note: Only the 4 distinct cases below match the above condition.

    // Case G: Going from one owned custom domain to another owned custom domain.
    if (!$shared['original'] && !$shared['new']) {
      // Example: http://foo.com -> http://bar.com
      return 'one owned to another owned';
    }
    // Case H: Going from standard to owned custom domain.
    elseif (!$shared['original'] && $shared['new']) {
      // Example: http://foo.com -> http://bar.com/foo
      return 'one owned to another group';
    }
    // Case I: Going from standard to owned custom domain.
    elseif ($shared['original'] && !$shared['new']) {
      // Example: http://foo.com/foo -> http://bar.com
      return 'one group to another owned';
    }
    // Case J: Going from standard to owned custom domain.
    elseif ($shared['original'] && $shared['new']) {
      // Example: http://foo.com/foo -> http://bar.com/foo
      return 'one group to another group';
    }
  }

  return FALSE;
}

/**
 * Returns an array of actions that should run for this form submit.
 *
 * @param string $domain_change
 *   The type of domain change occuring on this form submit.
 * @param string $group_path
 *   The new Group Path URL field value for the vsite being saved.
 * @param string $destination
 *   The original form destination.
 *
 * @return array $actions
 *   An indexed array containing:
 *    - 'callback': 'purl_save', 'purl_delete', or NULL if no update required.
 *    - 'value': the domain name whose purl modifier should be updated, or NULL.
 *    - 'cache': an array of cache IDs to flush, or NULL.
 *    - 'redirect': a string to redirect the form to, or NULL.
 */
function _vsite_domain_change_actions($domain_change, $domain, $group_path = '') {
  // Initializes redirect destination as "cp/settings".
  $destination = request_path();

  $actions = array(
    'callback' => NULL,
    'value' => NULL,
    'cache' => NULL,
    'redirect' => NULL,
  );

  // Prepares the actions needed for this case. Note: no 2 are identical.
  switch ($domain_change) {
    case 'standard to owned':
      $actions['callback'] = 'purl_save';
      $actions['value'] = $domain['new'];
      break;

    case 'standard to group':
      $destination = implode("/", array_slice(explode("/", $destination), 1));
      $redirect = "$group_path/$destination";
      $actions['redirect'] = $redirect;
      break;

    case 'owned to standard':
      $actions['callback'] = 'purl_delete';
      $actions['value'] = $domain['new'];
      $redirect = "$group_path/$destination";
      $actions['redirect'] = $redirect;
      break;

    case 'owned to group':
      $actions['callback'] = 'purl_delete';
      $actions['value'] = $domain['new'];
      $actions['cache'] = 'purl_modifiers_en';
      $purl = vsite_get_purl();
      $redirect = "$purl/$destination";
      $actions['redirect'] = $redirect;
      break;

    case 'group to standard':
      $destination = implode("/", array_slice(explode("/", $destination), 1));
      $redirect = "$group_path/$destination";
      $actions['redirect'] = $redirect;
      break;

    case 'group to owned':
      $actions['callback'] = 'purl_save';
      $actions['value'] = $domain['new'];
      break;

    case 'one owned to another owned':
      $actions['callback'] = 'purl_save';
      $actions['value'] = $domain['new'];
      $actions['cache'] = 'purl_modifiers_en';
      $redirect = "$group_path/$destination";
      $actions['redirect'] = $redirect;
      break;

    case 'one owned to another group':
      $actions['callback'] = 'purl_delete';
      $actions['value'] = $domain['original'];
      $actions['cache'] = 'purl_modifiers_en';
      $redirect = "$group_path/$destination";
      $actions['redirect'] = $redirect;
      break;

    case 'one group to another owned':
      $actions['callback'] = 'purl_save';
      $actions['value'] = $domain['new'];
      break;

    case 'one group to another group':
      $actions['cache'] = 'purl_modifiers_en';
      break;
  }

  return $actions;
}

/**
 * Processes the CP > Settings domain values to see what updates are needed.
 *
 * @param array $form
 *   The CP > Settings drupal form array.
 * @param array $form_state
 *   The CP > Settings drupal form_state array.
 *
 * @return mixed
 *   FALSE if no changes have been made to the domain settings.
 *   Otherwise, an indexed array containing:
 *     - 'domain': The original, new and modified status of the domain value.
 *     - 'shared': The original, new and modified status of the shared value.
 *     - 'group_path': The updated group path field value, as absolute URL.
 */
function _vsite_domain_process_new_values($form, $form_state, $vsite = NULL) {
  // Handles optional $vsite param
  if (is_numeric($vsite)) {
    $vsite = vsite_get_vsite($vsite);
  }
  elseif (!$vsite) {
    $vsite = vsite_get_vsite();
  }

  // Returns an array containing the original and new values, and bool modified.
  $domain = _vsite_domain_check_domain($form, $form_state);
  // Consider sites without custom domains to be "shared".
  // The form element will always appear checked if there is no domain,
  // but we still need to set the value to "1".
  if (!strlen($domain['new'])) {
    $form_state['values']['vsite_domain_name']['vsite_domain_shared'] = 1;
  }
  // Returns an array containing the original and new values, and bool modified.
  $shared = _vsite_domain_check_shared($form, $form_state);

  // Returns FALSE if the domain name variables are not changed.
  if (!$shared['modified'] && !$domain['modified']) {
    return FALSE;
  }

  // Checks whether to use global $base_url or custom for Group path.
  if (strlen($domain['new'])) {
    // Prepends scheme to front of domain name string.
    // Note: Currently does not handle HTTPS scheme.
    $group_path = "http://{$domain['new']}";
  }
  else {
    // Prepares the return value without '[site:url]' token.
    $group_path = variable_get('purl_base_domain', '');
  }

  // Optionally appends the purl modifier to the end of the Group path string.
  if ($shared['new'] == 1) {
    // Only loads the purl modifier if we will add it to the URL string.
    $purl = vsite_get_purl($vsite);
    $group_path = "$group_path/$purl";
  }

  return array(
    'group_path' => $group_path,
    'domain' => $domain,
    'shared' => $shared,
  );
}

/**
 * Clears field caches when any value in the domain setting is changed
 */
function _vsite_domain_clear_field_caches($gid) {
  $entities = db_select('og_membership', 'ogm')
    ->fields('ogm', array('etid', 'entity_type'))
    ->condition('gid', $gid)
    ->execute();

  $cids = array();
  // trick DB caches into using transactions for performance
  $transaction = db_transaction();
  foreach ($entities as $en) {
    cache_clear_all('field:'.$en->entity_type.':'.$en->etid, 'cache_field');
  }
  // transactions commit when the variable is destroyed. Nothing more is required.
}

/**
 * Sets a given string as the group path URL field value on the given vsite.
 */
function vsite_domain_set_group_path($group_path, $vsite = NULL) {
  // Handles optional $vsite param
  if (is_numeric($vsite)) {
    $vsite = vsite_get_vsite($vsite);
  }
  elseif (!$vsite) {
    $vsite = vsite_get_vsite();
  }

  // Stores purl_base_domain as a token '[site:url]' in the field.
  $purl_base_domain = variable_get('purl_base_domain', '');
  $group_path = str_replace($purl_base_domain, '[site:url]', $group_path);

  // Saves the Group path field value on the vsite node.
  $wrapper = entity_metadata_wrapper('node', $vsite->group);
  $field_value = $wrapper->field_group_path->value();
  $field_value['url'] = $group_path;
  $wrapper->field_group_path->set($field_value);
  $wrapper->save();
}

/**
 * Submit callback; Checks whether vsite_domain_shared value is changed.
 *
 * @return array
 *
 * @see vsite_domain_cp_settings_domain_submit()
 */
function _vsite_domain_check_shared($form, $form_state) {
  $original_path = array('domain', 'vsite_domain_shared', '#default_value');
  $new_path = array('values', 'vsite_domain_shared');
  return _vsite_domain_form_value_modified($form, $form_state, $original_path, $new_path);
}

/**
 * Submit callback; Checks whether the vsite_domain_name value is changed.
 *
 * @return array
 *
 * @see vsite_domain_cp_settings_domain_submit()
 */
function _vsite_domain_check_domain(&$form, &$form_state) {
  $original_path = array('domain', 'vsite_domain_name', '#default_value');
  $new_path = array('values', 'vsite_domain_name');
  return _vsite_domain_form_value_modified($form, $form_state, $original_path, $new_path);
}

/**
 * Returns TRUE if the specified element has two set and different values.
 *
 * Returns FALSE if either element path is undefined, or if the two values are
 * equal using the "==" operator.
 *
 * @param array $original_array
 *   A multi-leveled indexed Drupal $form array
 * @param array $new_array
 *   A multi-leveled indexed Drupal $form_state array
 * @param array $original_path
 *   An ordered array of keys to access the original value in $original_array.
 * @param array $new_path
 *   An ordered array of keys to access the new value in $new_array.
 *
 * @return bool
 *   Returns TRUE only if both paths are defined until at least the penultimate
 *   key value, and both accessed values are equal using the `==` operator.
 *
 * @see _vsite_domain_check_form_value()
 */
function _vsite_domain_form_value_modified($original_array, $new_array, $original_path, $new_path) {
  // Attempts to access both values in their respective arrays...
  $original_val = _vsite_domain_check_form_value($original_array, $original_path);
  $new_val = _vsite_domain_check_form_value($new_array, $new_path);

  // Returns FALSE if either of the values were inaccessible.
  if ($original_val === FALSE || $new_val === FALSE) {
    return FALSE;
  }

  // Returns TRUE if the values are different, otherwise they are unchanged.
  return array(
    'modified' => (bool)($new_val != $original_val),
    'original' => $original_val,
    'new' => $new_val,
  );
}

/**
 * Returns the value of an index at a given path, or FALSE if undefined.
 *
 * Note: Returns '' if only the last value of the path is undefined.
 *
 * @param array $array
 *   A multi-leveled indexed array, i.e. a Drupal $form or $form_state array
 * @param array $path
 *   An ordered array of string/int keys to access a value in the array.
 *
 * @return mixed
 *   Any possible value stored in a the array, or FALSE if any but the last
 *   key in the $path array was undefined.
 *
 * @see _vsite_domain_form_value_modified()
 */
function _vsite_domain_check_form_value($array, $path) {
  // Attempts to traverse the indexed array using the specified path...
  while (!empty($path)) {
    $next = array_shift($path);
    if (!isset($array[$next])) {
      // If this element is the last element, treats undefined key as value '';
      if (empty($path)) {
        return '';
      }
      // Returns FALSE if the element does not exist on the $form array.
      return FALSE;
    }
    // Traverses to the next level of the indexed array...
    $array = $array[$next];
  }

  // Returns the successfully accessed value in the array.
  return $array;
}

