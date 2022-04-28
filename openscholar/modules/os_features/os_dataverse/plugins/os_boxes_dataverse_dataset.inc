<?php

/**
 * @file
 * The class for Dataverse Dataset boxes.
 */

class os_boxes_dataverse_dataset extends os_boxes_default {

  private $dataverse_base_url_default = "https://dataverse.harvard.edu/";

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults(){
    $options = parent::options_defaults();
    $options['persistent_id'] = '';
    $options['doi_or_hdl'] = 'doi';
    $options['embed_height'] = '500px';
    $options['visibility'] = 'feature';
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $form = parent::options_form($form_state);

    $base_url = variable_get("dataverse_base_url", $this->dataverse_base_url_default);

    $form['persistent_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Dataset Persistent Identifier'),
      '#description' => t('Need help finding your Dataset Persistent Identifier? Learn how to here: !link. ' .
                          'Note: this widget should be placed in the main content area, not in sidebar.',
              array('!link' => l('https://help.theopenscholar.com/dataverse-dataset-widget',
                                'https://help.theopenscholar.com/dataverse-dataset-widget',
                                array('attributes' => array('target' => '_blank'))))),
      '#required' => TRUE,
      '#field_prefix' => $base_url,
      '#default_value' => $this->options['persistent_id'],
    );

    $form['doi_or_hdl'] = array(
      '#type' => 'radios',
      '#required' => TRUE,
      '#default_value' => $this->options['doi_or_hdl'],
      '#options' => array('doi' => t('DOI'), 'hdl' => t('HDL')),
    );

    $form['embed_height'] = array(
      '#type' => 'textfield',
      '#title' => 'Embed Height',
      '#default_value' => $this->options['embed_height'],
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

    $embed_height = preg_replace('/\D/', '', $this->options['embed_height']);
    $persistent_id = preg_replace('/^\s*(doi|hdl)/', '', $this->options['persistent_id']);
    $base_url = variable_get("dataverse_base_url", $this->dataverse_base_url_default) . "/resources/js/widgets.js?" . 
                "persistentId=" . strtolower($this->options['doi_or_hdl']) . ":" . trim($persistent_id) . "&amp;" .
                "dvUrl=" .  variable_get("dataverse_base_url", $this->dataverse_base_url_default) . "&amp;" .
                "widget=iframe&amp;" . 
                "heightPx=" . $embed_height;

    $block = parent::render();
    $block['content'] = "<script src=\"$base_url\"></script>";
    return $block;
  }

}
