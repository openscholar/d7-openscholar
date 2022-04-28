<?php

/**
 * Implements hook_cp_settings().
 */
function os_mailchimp_cp_settings() {
  $api = variable_get('os_mailchimp_vsite_api_key', '');

  $settings['os_mailchimp_vsite_api_key'] = array(
    'group' => array(
      '#id' => 'mailchimp',
      '#title' => t('Mailing List'),
      '#weight' => 0,
      '#help_link' => l(t('Learn more about the mailing list'), 'https://help.theopenscholar.com/embedding-mailchimp-newsletter-sign', array('attributes' => array('target' => '_blank')))
    ),
    'form' => array(
      '#type' => 'textfield',
      '#title' => t('MailChimp API Key'),
      '#default_value' => $api,
      '#description' => 'Your <a href="https://us1.admin.mailchimp.com/account/api-key-popup" target="_new">MailChimp API key</a>.  Example: 1234567890abcefghijklmnopqrstuvwx-us2',
    ),
  );

  return $settings;
}
