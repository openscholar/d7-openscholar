<?php

/**
 * Menu callback; Set the white listed domains for embedded media.
 */
function media_embed_settings($form, &$form_state) {
  $domains = variable_get('media_embed_whitelist', array());
  $form = array();

  $form['whitelist'] = array(
    '#type' => 'textarea',
    '#title' => t('Domain Whitelist'),
    '#default_value' => implode("\n", $domains),
    '#description' => t('Enter the list of domains that users may add iframe, object, embed and script tags from. One domain per line. <b>Do not add domains with http://www or https://www</b>'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}


/**
 * Submit handler; Set the white listed domains.
 */
function media_embed_settings_submit($form, $form_state) {
  $domains = $form_state['values']['whitelist'];

  $domains = array_merge(array_filter(preg_split("[\n\r|\n|\r]", $domains)));
  variable_set('media_embed_whitelist', media_embed_remove_domains_protocol($domains));
}
