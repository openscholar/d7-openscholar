<?php
class os_boxes_cache_test extends os_boxes_default {

  /**
   * Create a new box.
   */
  protected function __construct() {
    parent::__construct();
    $this->cache_time = '10 seconds';
  }

  public function options_defaults() {
    $options = parent::options_defaults();

    return $options;
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    return $form;
  }

  public function render() {
    $block = parent::render();

    if (($block['content'] = $this->get_cache()) === FALSE) {
      if (($output = variable_get('os_boxes_cache_test_value')) && $output !== 'bbbb') {
        $output = 'bbbb';
      }
      else {
        $output = 'aaaa';
      }
      variable_set('os_boxes_cache_test_value', $output);

      $block['content'] = $output;
      $this->set_cache($block['content']);
    }

    return $block;
  }
}
