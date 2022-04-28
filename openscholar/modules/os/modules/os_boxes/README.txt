For Developers:

By inheriting from os_boxes_default, box types gain the Advanced Options fieldset, 
which is locked behind the 'use boxes advanced settings' permission. Users without
this permission may not see the fieldset. It's a place to put settings that may be 
too technical, complicated or confusing for normal users.

Ex.

public function options_form(&$form_state) {
  $form = parent::options_form($form_state);
  
  $form['advanced']['enable_fanciness'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fanciness'),
    '#etc' => 'etc',
  );
}