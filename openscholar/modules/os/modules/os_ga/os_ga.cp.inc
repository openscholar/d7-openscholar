<?php

/**
 * Implements hook_cp_settings().
 */
function os_ga_cp_settings() {
  $settings = array();

  $settings['os_ga_google_analytics_id'] = array(
    'form' => array(
      '#type' => 'textfield',
      '#title' => t('Google Analytics Tracking ID'),
      '#size' => 20,
      '#maxlength' => 255,
      '#default_value' => variable_get('os_ga_google_analytics_id', ''),
      '#description' => t('Enter your tracking ID, which enables tracking for your <a href="@ga" target="_blank">Google Analytics</a> account. <a href="@howto" target="_blank">How to sign up &raquo;</a><br/>Expected format: <strong>UA-XXXXXXX-XX</strong>', array('@ga' => 'http://www.google.com/analytics/', '@howto' => 'http://support.google.com/analytics/bin/answer.py?hl=en&answer=1009694')),
      '#element_validate' => array('os_ga_analytics_id_validate'),
    ),
    'group' => array(
      '#id' => 'ga',
      '#title' => t('Google Analytics'),
      '#weight' => -100,
      '#help_link' => l(t('Learn more about analytics'), 'https://help.theopenscholar.com/global-site-settings#google-analytics', array('attributes' => array('target' => '_blank')))
    ),
  );

  return $settings;
}