<?php

/**
 * @file
 * The OpenScholar Help - Documentation and Support poupup screens
 */

/**
 * Implements hook_cp_settings().
 */
function os_help_cp_settings() {

  $form = array();

  $form['os_help'] = array(
    'group' => array(
      '#id' => 'documentation',
      '#title' => t('Documentation'),
      '#weight' => 50,
      '#menu_parent' => 'help'
    ),
    'form' => array(
      '#custom_directive' => 'documentation',
      '#access' => TRUE,
      '#weight' => -1,
    ),
  );

  $form['os_support'] = array(
    'group' => array(
      '#id' => 'support',
      '#title' => t('Support'),
      '#weight' => 50,
      '#menu_parent' => 'help'
    ),
    'form' => array(
      '#custom_directive' => 'support',
      '#access' => TRUE,
      '#weight' => -1,
    ),
  );

  return $form;
}

function os_help_contact_form_validate($value) {
  if (!empty($value)) {
    return true;
  }
  return false;
}

function os_help_rest_cp_settings_submit($var, $value) {

  $values['sender'] = $user;
  $values['sender']->name = variable_get('os_support_name');
  $values['sender']->mail = variable_get('os_support_mail');
  $values['category'] = contact_load(variable_get('os_support_cid'));
  $values['copy'] = variable_get('os_support_copy');
  $values['message'] = variable_get('os_support_message');
  $values['subject'] = variable_get('os_support_subject');

  // Get the to and from e-mail addresses.
  $to = $values['category']['recipients'];
  $from = $values['sender']->mail;

  // Send the e-mail to the recipients using the site default language.
  drupal_mail('contact', 'page_mail', $to, language_default(), $values, $from);

  // If the user requests it, send a copy using the current language.
  if ($values['copy']) {
    drupal_mail('contact', 'page_copy', $from, $language, $values, $from);
  }
  variable_del('os_support_name');
  variable_del('os_support_mail');
  variable_del('os_support_cid');
  variable_del('os_support_copy');
  variable_del('os_support_message');
  variable_del('os_support_subject');
  flood_register_event('contact', variable_get('contact_threshold_window', 3600));
}
