<?php 

/**
 * @file
 * 
 * Admin forms for cp module
 */

function cp_admin_form() {
  $form = array(
    'cp_support_getsatisfaction_widget' => array(
      '#title' => t('GetSatisfaction widget'),
      '#description' => t('Replace the contact form with a GetSatisfaction widget.  Create the widget in your GetSatisfaction community.  Get its four digit id from the embed code.  (ie &lt;div id="getsat-widget-<b>1234</b>"&gt; ...).  Set to 0 to disable.'),
      '#type' => 'textfield',
      '#default_value' => variable_get('cp_support_getsatisfaction_widget', 0),
      '#element_validate' => array('element_validate_integer'),
    ),  
  );
  
  return system_settings_form($form);
}
