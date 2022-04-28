<?php
class os_boxes_eval extends os_boxes_default {

  public function options_defaults() {
    $options = parent::options_defaults();

    $options['eval'] = array(
      'value' => '',
    );

    return $options;
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    $form['eval'] = array(
      '#type' => 'textarea',
      '#default_value' => $this->options['eval']['value'],
      '#title' => t('Content'),
      '#description' => t('You can put a PHP function name for eval')
    );

    return $form;
  }

  public function render() {
    $block = parent::render();

    $block['content'] = eval($this->options['eval']['value']);

    return $block;
  }
}
