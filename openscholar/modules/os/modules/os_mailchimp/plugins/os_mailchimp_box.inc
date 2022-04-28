<?php

/**
 * @file
 * The class for "mailing list" type widgets.
 */
class os_boxes_mailchimp extends os_boxes_default {
  
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    
    $options['list'] = '';
    $options['text'] = t('Subscribe to our mailing list');
    $options['display'] = 'link';
    $options['mailchimp_iframe_height'] = 800;
    $options['mailchimp_iframe_width'] = 600;
    
    return $options;
  }
  
  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    // using spaces_get_space instead of vsite_get_vsite to remove the vsite
    if (($space = spaces_get_space()) != FALSE) {
      $description = 'Choose a mailing list to subscribe users to.  If none are available, check your MailChimp API key in <a href="/' . $space->group->purl . '/cp/settings" target="_new">site settings</a>.';
    } else {
      $description = 'Choose a mailing list to subscribe users to.  If none are available, check your MailChimp API key';
    }
    
    $api = variable_get('os_mailchimp_vsite_api_key', '');
    
    if (!$api) {
	    drupal_set_message(t('You have not set a <strong>MailChimp API key</strong>.  Until you do so, mailing lists will not work.  You can set up an API key in Settings > Global Settings > Mailing List.'), 'error');
    } else {
      $form['subscribe']['list'] = array(
        '#type' => 'select', 
        '#title' => t('Mailing List'), 
        '#default_value' => $this->options['list'], 
        '#options' => _os_mailchimp_get_lists($api), 
        '#description' => $description,
      );
      
      $defaults = $this->options_defaults();
      $form['subscribe']['text'] = array(
        '#type' => 'textfield', 
        '#title' => 'Subscribe Text', 
        '#default_value' => $this->options['text'], 
        '#description' => t('Text for subscribe link.  Default: ' . $defaults['text'] . '.'),
      );
      
      $form['subscribe']['display'] = array(
        '#type' => 'radios', 
        '#title' => 'Display', 
        '#options' => array('link' => t('Link'), 'form' => t('Basic Form'), 'mailchimp_form' => t('MailChimp Form')), 
        '#default_value' => $this->options['display'], 
        '#description' => t('Show a link to subscription popup, simple form, or an advanced form customizable at mailchimp.com'),
      );
      
      $form['subscribe']['mailchimp_iframe_height'] = array(
        '#type' => 'textfield',
        '#title' => t('MailChimp form height.'),
        '#default_value' => (isset($this->options['mailchimp_iframe_height'])) ? $this->options['mailchimp_iframe_height'] : $defaults['mailchimp_iframe_height'],
        '#states' => array('visible' => array('input[name="display"]' => array('value' => 'mailchimp_form'))),
      );

      $form['subscribe']['mailchimp_iframe_width'] = array(
        '#type' => 'textfield',
        '#title' => t('MailChimp form width.'),
        '#default_value' => (isset($this->options['mailchimp_iframe_width'])) ? $this->options['mailchimp_iframe_width'] : $defaults['mailchimp_iframe_width'],
        '#states' => array('visible' => array('input[name="display"]' => array('value' => 'mailchimp_form'))),
        '#description' => t('For flexibility, recommended values should be in percentages. (ex. 100%)'),
      );
    }
    
    return $form;
  }
  
  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }

    $block = parent::render();
    global $user;
    $defaults = $this->options_defaults();
    $this->options += $defaults; 
    
    switch ($this->options['display']) {
      case 'form':
        $block['content'] = drupal_render(drupal_get_form('os_mailchimp_form', array('delta' => $this->delta)));
        break;
      
      case 'mailchimp_form':
        $info = (_os_mailchimp_get_list_info(variable_get('os_mailchimp_vsite_api_key', '')));
        if ($info['lists']) {
          foreach ($info['lists'] as $list_info) {
            if ($list_info->id == $this->options['list']) {
              $subscribe_url_long = $list_info->subscribe_url_long;
              // change "manage1" or "manage2" to "manage" in domain part
              $subscribe_url_long = str_replace(array('manage1', 'manage2'), 'manage', $subscribe_url_long);
              $subscribe_url_long = str_replace('http://', '//', $subscribe_url_long);
              $block['content'] = t('<iframe src="@url" width="@width", height="@height"></iframe>', array(
                '@url' => $subscribe_url_long,
                '@width' => (intval($this->options['mailchimp_iframe_width']) > 0) ? $this->options['mailchimp_iframe_width'] : $defaults['mailchimp_iframe_width'],
                '@height' => (intval($this->options['mailchimp_iframe_height']) > 0) ? $this->options['mailchimp_iframe_height'] : $defaults['mailchimp_iframe_height'],
              ));
              break;
            }
          }
        }
        break;
      
      case 'link':
      default :
        // link to a popup containing the form
        ctools_include('ajax');
        ctools_include('modal');
        ctools_modal_add_js();
        $modal_style = array(
          'mailchimp-modal-style' => array(
            'modalSize' => array(
              'type' => 'fixed',
              'width' => '100%',
              'height' => 360,
              'addWidth' => 10,
              'addHeight' => 10,
              'contentRight' => 0,
              'contentBottom' => 0,
            ),
          )
        );
        drupal_add_js($modal_style, 'setting');
        $block['content'] = ctools_ajax_text_button($this->options['text'], 'os_mailchimp/js/subscribe/' . $this->delta, 'Subscribe to this mailing list', 'ctools-use-modal ctools-modal-mailchimp-modal-style');
        break;
    }
    
    return $block;
  }
}
