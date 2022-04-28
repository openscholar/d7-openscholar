<?php

/**
 * @file
 * The class for "Publication By Year" type widgets.
 */

class os_boxes_pub_year extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['display_num'] = 1;

    // needed for caching to work
    $options['content_type'] = 'biblio';

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $form = parent::options_form($form_state);

    $form['display_num'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display number of items in category'),
      '#default_value' => $this->options['display_num'],
    );
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    $this->set_cache_id('node:biblio');
    // Do not cache this block, it relies on non-ajax pagination see #6669
    //if (($block['content'] = $this->get_cache()) === FALSE) {
      $delta = 'publication_years-block';
      list($name, $display_id) = explode('-', $delta);
      if ($view = views_get_view($name)) {
        if ($view->access($display_id)) {
          $view->set_display('block');

          // Tie this view to this box (this tells the view to call this box's view alter).
          $view->dom_id = "boxes-".$this->delta;

          // Set the display num in the view (the count).
          $arguments = $view->display_handler->get_option('arguments');
          $arguments['biblio_year']['summary_options']['count'] = ($this->options['display_num'])?1:0;
          $view->display_handler->override_option('arguments', $arguments);
//          if($this->options['display_num']) {
//            $view->display_handler->override_option('use_ajax', true);
//          }

          $output = $view->execute_display($display_id);
          $block['content'] = $output['content'];
        }
        $view->destroy();
      //}
      //$this->set_cache($block['content']);
    }

    return $block;
  }

  /**
   * Return the cache parameters for this box
   */
  public function cache_info() {
    // Caching disabled so ajax pager gets init'ed.
  	return array();
  }

}
