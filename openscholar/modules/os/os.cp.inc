<?php

/**
 * Implements hook_cp_settings().
 *
 * Add the option to choose between Infinite scrolling or Pagination display for long list of content.
 */
function os_cp_settings() {
  $setting = array();
  $setting['privacy_policy'] = array(
   'group' => array(
     '#title' => t('Privacy Policy'),
      '#id' => 'privacy-policy',
      '#help_link' => l(t('Learn more about creating a privacy policy for your site'), 'http://docs.openscholar.harvard.edu/global-site-settings#privacy-policy', array('attributes' => array('target' => '_blank'))),
      ),
    'form' => array(
      '#type' => 'textfield',
      '#default_value' => variable_get('privacy_policy', ''),
      '#title' => t('Privacy Policy URL'),
      '#description' => t('Enter the URL to the privacy policy you wish to use for your site. Can be a link to a post on your own site.'),
    ),
  );
  $setting['long_list_content_pagination'] = array(
    'group' => array(
      '#title' => t('Long List Display'),
      '#id' => 'long-list-content-pagination',
      '#weight' => 40,
      '#help_link' => l(t('Learn more about Long list display'), 'https://help.theopenscholar.com/global-site-settings#long-list-display', array('attributes' => array('target' => '_blank')))
    ),
    'form' => array(
      '#type' => 'radios',
      '#prefix' => '<label class="margin-bottom settings-form-heading">Choose how long lists of content will display</label>',
      '#default_value' => variable_get('long_list_content_pagination', 'infinite_scroll'),
      '#options' => array('infinite_scroll' => 'Infinite scrolling', 'pager' => 'Pagination'),
      '#description' => t('Pagination applies only to Blog, Links, News, FAQs, Publications and Profiles.')
    ),
  );

  return $setting;
}