<?php
/**
 * @file
 * cp.form.inc
 */

/**
 * Form builder; merges various site settings into a single "Settings" form.
 */
function cp_settings_form($form, &$form_state) {
  $form['#submit'] = array();
  $form['#after_build'] = array();

  // Loops over all implementations of hook_cp_settings().
  $settings = cp_get_setting_forms($form_state);
  foreach ($settings as $key => $setting) {
    if (!isset($setting['form'])) {
      continue;
    }

    // Creates any non-existing fieldgroup for this setting, if specified.
    if (isset($setting['group'])) {
      $id = $setting['group']['#id'];
      if (!isset($form[$id])) {
        $form[$id] = $setting['group'];
        $form[$id]['#type'] = 'fieldset';
      }

      $form[$id][$key] = $setting['form'];
    }
    else {
      $form[$key] = $setting['form'];
    }

    // Merges any submit handler(s) into the existing value.
    if (isset($setting['submit'])) {
      $form['#submit'] = array_unique(array_merge($form['#submit'], (array)$setting['submit']));
    }
    if (isset($setting['after_build'])) {
      $form['#after_build'] = array_merge($form['#after_build'], $setting['after_build']);
    }
  }

  return system_settings_form($form);
}

/**
 * Spaces features form.
 */
function cp_app_settings_form($form, &$form_state) {
  $spaces_features = variable_get('spaces_features', array());
  $space = spaces_get_space();
  $features = spaces_features($space ? $space->type : 'site');

  $sort_array = array();
  foreach ($features as $feature => $info) {
    $sort_array[$feature] = $info->info['name'];
  }
  asort($sort_array);
  $i = 0;
  foreach ($sort_array as $feature => $name) {
    $form['spaces_features'][$feature]['#weight'] = $i++;
  }

  foreach ($features as $feature => $info) {
    $label = "<strong>" . t($info->info['name']) . "</strong>";
    $label .= "<div class='description'>" . t($info->info['description']) . "</div>";
    $form['labels'][$feature] = array(
      '#type' => 'markup',
      '#markup' => $label,
    );
    $form['spaces_features'][$feature] = array(
      '#type' => 'select',
      '#options' => $space ? $space->feature_options() : array(
          0 => t('Disabled'),
          1 => t('Enabled'),
        ),
      '#default_value' => isset($spaces_features[$feature]) ? $spaces_features[$feature] : 0,
    );
  }

  $form = system_settings_form($form);
  $form['#theme'] = 'spaces_features_form';
  $form['actions']['#weight'] = 100;

  if ($space = spaces_get_space()) {
    // Add our submit handler to system settings forms
    if (!empty($form['#submit'])) {
      $position = array_search('system_settings_form_submit', $form['#submit'], TRUE);
      if ($position !== FALSE) {
        $form['#submit'][$position] = 'cp_app_system_settings_form_submit';
        if (isset($form['actions']) && count(element_children($form['actions'])) == 1 &&
          spaces_access_admin_perms(array('administer spaces'))
        ) {
        $form['actions']['reset'] = array(
          '#type' => 'submit',
          '#value' => t('Reset to defaults'),
          );
          $form['actions']['submit']['#value'] = t('Save for @name', array(
            '@name' => $space->title(),
          ));
        }
     }
   }
    // Add the #space value to all forms generated in a space to enable non-
    // purl space-types to handle AHAH callbacks. @see spaces_ahah_check()
    $form['#space'] = array(
      '#type' => 'value',
      '#value' => $space->type . ':' . $space->id,
    );
  }

  return $form;
}

function cp_app_system_settings_form_submit($form, &$form_state) {
  if ($space = spaces_get_space()) {
    $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
    // Exclude unnecessary elements.
    form_state_values_clean($form_state);
    $spaces_features = variable_get('spaces_features', array());
    $features = spaces_features($space ? $space->type : 'site');

    if ($op == t('Reset to defaults')) {
      $space->controllers->variable->del('spaces_features');
    }
    else
    {
      foreach ($form_state['values'] as $key => $value)
      {
        if (isset($features[$key]))
        {
          $spaces_features[$key] = $value;
        }
      }
      $space->controllers->variable->set('spaces_features', $spaces_features);
    }
    if ($op == t('Reset to defaults')) {
      drupal_set_message(t('The configuration options have been reset to their default values for %space.', array('%space' => $space->title())));
    }
    else {
      drupal_set_message(t('The configuration options have been saved for %space.', array('%space' => $space->title())));
    }
  }
}

