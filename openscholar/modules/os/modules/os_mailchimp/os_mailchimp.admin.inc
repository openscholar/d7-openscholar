<?php

/**
 * @function os_mailchimp_admin
 * 
 * returns Mailchimp's admin form
 */
function os_mailchimp_admin() {
	$form = array();
  $form['os_mailchimp'] = array(
	  '#type' => 'fieldset',
	  '#title' => t('Configure MailChimp'),
	  '#collapsible' => FALSE,
	  '#collapsed' => FALSE,
    '#description' => t('Settings for sitewide MailChimp box.  Does not override individual boxes.'),
	);

	//module mode 
  $form['os_mailchimp']['os_mailchimp_api'] = array(
	  '#type' => 'textfield',
	  '#title' => t('API Key'),
	  '#required' => TRUE,
	  '#default_value' => variable_get('os_mailchimp_api', ''), 
	  '#description' => t('Your MailChimp API Key.  Leaving this blank will disable automatic subscription'),
   );
   $options = _os_mailchimp_get_lists(variable_get('os_mailchimp_api', ''));
   if ($options) {
     $form['os_mailchimp']['os_mailchimp_list'] = array(
      '#type' => 'select',
      '#title' => t('Mailing List'),
      '#required' => TRUE,
      '#default_value' => variable_get('os_mailchimp_list', ''), 
      '#description' => t('MailChimp list to subscribe new users to.'),
      '#options' => $options,
     );
     
     $use_field_state = array('visible' => array('input[name="os_mailchimp_use_field"]' => array('checked' => TRUE)));
     $form['os_mailchimp']['os_mailchimp_use_field'] = array(
       '#type' => 'checkbox',
       '#title' => t('Use custom field'),
       '#default_value' => variable_get('os_mailchimp_use_field', FALSE),
     );
     $form['os_mailchimp']['os_mailchimp_use_field_name'] = array(
       '#type' => 'textfield',
       '#title' => 'Field name',
       '#default_value' => variable_get('os_mailchimp_use_field_name', ''),
       '#states' => $use_field_state,
     );
     $form['os_mailchimp']['os_mailchimp_use_field_value'] = array(
       '#type' => 'textfield',
       '#title' => 'Field value',
       '#default_value' => variable_get('os_mailchimp_use_field_value', ''),
       '#states' => $use_field_state,
       '#description' => t('Mailchimp can be customized with extra fields for capturing and storing data.  
           Openscholar can be configured to use these fields when automatically subscribing users.  Enter
            the name of a field (from the merge tag column, not the field label column in Mailchimp) and 
           a value here.  (An example of how to use this would be several OpenScholar installations adding 
           users to the same mailing list.  By creating a field for each installation, you can flag where 
           each subscription originated.)'),
     );
   } else {
     $form['os_mailchimp']['api_key_error'] = array(
       '#type' => 'markup',
       '#value' => 'No mailing lists available.  Please check your API key and try again.',
     );
   }
      
   return system_settings_form($form);
}