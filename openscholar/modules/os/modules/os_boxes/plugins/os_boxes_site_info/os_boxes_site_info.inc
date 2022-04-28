<?php

class os_boxes_site_info extends os_boxes_default {

  function options_defaults() {
    $options = parent::options_defaults();

    $options += array(
      'site_title' => 'Your Name',
      'descrip' => 'Hover over here, and then click the gear to change me.',
      'address1' => '13 Somewhere Dr.',
      'address2' => 'That Town, USA',
    );

    return $options;
  }

  function box_form(&$form_state) {
    $form = parent::options_form($form_state);

    if (module_exists('vsite') && $vsite = spaces_get_space()) {
      $group = $vsite->group;
      $addr = field_get_items('node', $group, 'field_site_address');
      if (isset($addr[0])) {
        $addr = explode('<br>', $addr[0]['value']);
      }
      else {
        $addr = array('', '');
      }
      $descrip = field_get_items('node', $group, 'field_site_description');
      $this->options = array_merge($this->options, array(
        'site_title' => $group->title,
        'descrip' => isset($descrip[0]) ? $descrip[0]['value'] : '',
        'address1' => $addr[0],
        'address2' => isset($addr[1]) ? $addr[1] : '',
      ));
    }
    $form['site_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $this->options['site_title'],
      '#size' => 60,
      '#required' => true,
    );

    $form['descrip'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#default_value' => $this->options['descrip'],
      '#description' => t('A brief description of your web site.'),
      '#size' => 60,
    );

    $form['address1'] = array(
      '#type' => 'textfield',
      '#title' => t('Address Line 1'),
      '#default_value' => $this->options['address1'],
      '#size' => 30,
      '#description' => t('Ex. 31 Harvard Ave.'),
    );

    $form['address2'] = array(
      '#type' => 'textfield',
      '#title' => t('Address Line 2'),
      '#default_value' => $this->options['address2'],
      '#size' => 30,
      '#description' => t('Ex. Cambridge, MA 02138'),
    );

    $form['contact'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable contact form.'),
      '#default_value' => variable_get('show_email', variable_get('vsite_users_contact_form_anonymous', TRUE)),
      '#description' => t('Will print an Contact link to your contact form.'),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('os_boxes_site_info_submit'),
    );
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#limit_validation_errors' => array(),
    );

    return $form;
  }

  function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }
    $block = parent::render();

    $contact = variable_get('show_email', variable_get('vsite_users_contact_form_anonymous', TRUE));
    if (module_exists('vsite') && $vsite = spaces_get_space()) {
      $group = $vsite->group;
      $addr = field_get_items('node', $group, 'field_site_address');
      $addr = explode('<br>', $addr[0]['value']);
      $descrip = field_get_items('node', $group, 'field_site_description');
      $this->options = array_merge($this->options, array(
        'site_title' => $group->title,
        'descrip' => $descrip[0]['value'],
        'address1' => (isset($addr[0])) ? $addr[0] : '',
        'address2' => (isset($addr[1])) ? $addr[1] : '',
      ));
    }

    $tags = array('br', 'a', 'em', 'strong', 'cite', 'blockquote', 'code', 'ul', 'ol', 'li', 'dl', 'dt', 'dd');

    $output = '<h1>'.l($this->options['site_title'], '<front>').'</h1>
    '.(!empty($this->options['descrip'])?'<h2>'.filter_xss($this->options['descrip'], $tags).'</h2>':'').'
    <p>'.((!empty($this->options['address1']))?filter_xss($this->options['address1']).'<br>':'').((!empty($this->options['address2']))?filter_xss($this->options['address2']).'<br>':''). ($contact ? l('Contact', 'contact_owner') : '' . '</p>');

    $block['content'] = $output;
    return $block;
  }
}

function os_boxes_site_info_submit($form, &$form_state) {
  $box = $form_state['box'];
  if (module_exists('vsite') && $space = spaces_get_space()) {
    // Populate the VSite with this information.
    $node = $space->group;
    $node->title = $form_state['values']['site_title'];

    $node->field_site_description[LANGUAGE_NONE][0] = array(
      'value' => $form_state['values']['descrip'],
      'format' => NULL,
    );
    $node->field_site_address[LANGUAGE_NONE][0] = array(
      'value' => $form_state['values']['address1'].'<br>'.$form_state['values']['address2'],
      'format' => NULL,
    );
    if (empty($node->field_site_logo[LANGUAGE_NONE][0])){
      $node->field_site_logo = NULL;
    }

    try {
      node_save($node);
      $space->controllers->variable->set('show_email', $form_state['values']['contact']);
    }
    catch (Exception $e) {
      drupal_set_message(t('Error saving site information. Contact your site administrator.'), 'error');
      error_log($e);
    }
  }
  else {
    boxes_box_form_submit($form, $form_state);
    variable_set('show_email', $form_state['values']['contact']);
  }
}
