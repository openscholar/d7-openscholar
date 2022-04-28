<?php

/**
 * @file
 * Admin functions for the Vsite Register module.
 */

/**
 * Form builder; returns a standard system settings form.
 */
function vsite_register_admin_form($form, &$state) {
  $presets = _vsite_register_list_presets();
  $form = array(
    'vsite_register_get_site_text' => array(
      '#type' => 'textfield',
      '#title' => t('For users who may create more site(s)'),
      '#size' => 60,
      '#default_value' => variable_get('vsite_register_get_site_text', t('Create Your Site Here')),
    ),
    'vsite_register_max_site_text_single' => array(
      '#type' => 'textfield',
      '#title' => t('For maxed-out users with 1 site'),
      '#size' => 60,
      '#default_value' => variable_get('vsite_register_max_site_text_single', t('Go to your site')),
    ),
     'vsite_register_max_site_text_plural' => array(
      '#type' => 'textfield',
      '#title' => t('For maxed-out users with multiple sites'),
      '#size' => 60,
      '#default_value' => variable_get('vsite_register_max_site_text_plural', t('Go to your sites')),
    ),
    'os_enabled_spaces_presets' => array(
      '#title' => t('Enabled presets'),
      '#type' => 'checkboxes',
      '#description' => t('Presets provide default settings and features for different types of sites.'),
      '#default_value' => variable_get('os_enabled_spaces_presets', array_combine(array_keys($presets), array_keys($presets))),
      '#options' => $presets,
    ),
    // Prepares setting for maximum number of vsites per user.
    'os_vsites_per_user' => array(
      '#title' => t('Vsites per user'),
      '#type' => 'textfield',
      '#size' => '5',
      '#default_value' => variable_get('os_vsites_per_user', 1),
    ),
    // Optionally choose a node to display to newly registered users.
    'vsite_register_welcome_message' => array(
      '#title' => t('Welcome message node ID'),
      '#type' => 'textfield',
      '#description' => t('(Optional) Enter the nid of a post to display in an overlay to newly registered vsite owners on their first visit.'),
      '#size' => '5',
      '#default_value' => variable_get('vsite_register_welcome_message', ''),
    ),
    // Specify an email address to receive notifications whenever a new site
    // is registered.
    'vsite_register_admin_mail' => array(
      '#title' => t('Administrator notification email'),
      '#type' => 'textfield',
      '#description' => t('(Optional) Specify an email address to receive notifications whenever a new site is registered. Leave blank for no administrator notification.'),
      '#maxlength' => 255,
      '#default_value' => variable_get('vsite_register_admin_mail', ''),
    ),
  );

  return system_settings_form($form);
}

/**
 * Form validate callback; checks vsites per user and enabled presets.
 */
function vsite_register_admin_form_validate(&$form, &$state) {
  $state['values'];
  if ((int) $state['values']['os_vsites_per_user'] < 0) {
    form_set_error('os_vsites_per_user', t('Vsites per user must be a positive integer.  To restrict users from creating sites, please edit permissions instead.'));
  }

  if (!count(array_filter($state['values']['os_enabled_spaces_presets']))) {
    form_set_error('os_enabled_spaces_presets', t('At least one preset must be enabled.'));
  }
}

/**
 * Returns an array of presets keyed by machine name, value as human name.
 */
function _vsite_register_list_presets() {
  $presets = array();
  foreach (spaces_preset_load() as $name => $preset) {
    if (!(isset($preset->disabled) && $preset->disabled)) {
      $presets[$name] = $preset->title;
    }
  }
  return $presets;
}


