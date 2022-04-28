<?php

/**
 * @file
 * The class for Dataverse Search boxes.
 */

class os_boxes_dataverse_search extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults(){
    $options = parent::options_defaults();
    $options['user_alias'] = '';
    $options['search_box_text'] = 'Search my dataverse';
    $options['visibility'] = 'feature';
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $form = parent::options_form($form_state);

    $base_url = variable_get("dataverse_base_url", "https://dataverse.harvard.edu") . "/dataverse/";

    $form['user_alias'] = array(
      '#type' => 'textfield',
      '#title' => t('Identifier'),
      '#required' => TRUE,
      '#field_prefix' => $base_url,
      '#default_value' => $this->options['user_alias'],
      '#description' => t('Need help finding your Dataset Persistent Identifier? Learn how to here: !link. ' .
        'Note: this widget should be placed in the main content area, not in sidebar.',
        array('!link' => l('https://help.theopenscholar.com/dataverse-search-box',
          'https://help.theopenscholar.com/dataverse-search-box',
          array('attributes' => array('target' => '_blank'))))),
    );

    $form['search_box_text'] = array(
      '#type' => 'textfield',
      '#title' => 'Search Box Text',
      '#default_value' => $this->options['search_box_text'],
    );

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }

    $base_url = variable_get("dataverse_base_url", "https://dataverse.harvard.edu") . "/resources/js/widgets.js?alias=" . $this->options['user_alias'] . "&amp;dvUrl=" . variable_get("dataverse_base_url", "https://dataverse.harvard.edu") . "&amp;widget=search&amp;text=" . str_replace("%20", "+", drupal_encode_path($this->options['search_box_text']));

    $block = parent::render();
    $block['content'] = "<script src=\"$base_url\"></script>";
    return $block;
  }

}
