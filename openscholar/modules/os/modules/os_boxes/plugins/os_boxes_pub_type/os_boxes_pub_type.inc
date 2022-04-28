<?php

/**
 * @file
 * The class for "Publication by Type" type widgets.
 */

class os_boxes_pub_type extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['whitelist'] = array('all' => 'all');
    $options['display_num'] = 1;

    // needed for caching to work
    $options['content_type'] = 'biblio';

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $rows = db_select('biblio_types', 'bt')
            ->fields('bt', array('tid', 'name'))
            ->condition('tid', 0, '>')
            ->condition('visible', 1)
            ->execute();
    $options = array('all' => 'Select All');
    $value = $this->options['whitelist'];
    $all = in_array('all', $value) && $value['all'] === 'all';
    foreach ($rows as $r) {
      $options[$r->tid] = $r->name;
      if ($all) $value[$r->tid] = $r->tid;
    }

    $form['whitelist'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $value,
      '#title' => t('Show the following types:'),
      '#attributes' => array('class' => array('pub_type')),
      '#description' => t('Select the types you want to appear in the widget.'),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/misc/selectAll.js',
        ),
      ),
    );

    $form['display_num'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display number of items in category'),
      '#default_value' => $this->options['display_num'],
    );

    $form += parent::options_form($form_state);
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    $this->set_cache_id('node:biblio');
    if (($block['content'] = $this->get_cache()) === FALSE) {
      $delta = 'publication_types-block';
      list($name, $display_id) = explode('-', $delta);
      if ($view = views_get_view($name)) {
        if ($view->access($display_id)) {
          $view->set_display('block');
          if (!in_array('all', $this->options['whitelist']) || !$this->options['whitelist']['all']) {
            $filter = array(
              'operator' => 'in',
              'value' => $this->options['whitelist'],
              'group' => '0',
              'exposed' => FALSE,
              'expose' => array(
                'operator' => FALSE,
                'label' => '',
              ),
              'id' => 'tid',
              'table' => 'biblio_types',
              'field' => 'tid',
              'override' => array(
                'button' => 'Use default',
              ),
              'relationship' => 'none',
            );
            $view->set_item('block', 'filter', 'tid', $filter);
          }

          // Set the display num in the view (the count).
          $arguments = $view->display_handler->get_option('arguments');
          $arguments['name']['summary_options']['count'] = ($this->options['display_num'])?1:0;
          $view->display_handler->override_option('arguments', $arguments);

          // Tie this view to this box (this tells the view to call this box's view alter).
          $view->dom_id = "boxes-".$this->delta;

          $output = $view->execute_display('block');
          $view->destroy();
          $block['content'] = $output['content'];
        }
        $view->destroy();
      }
      $this->set_cache($block['content']);
    }
    return $block;
  }

  /**
   * Called from views_pre_view
   *
   * @param view $view
   */
  public function views_alter(&$view, &$display_id, &$args){
  }

  /**
   * Return the cache parameters for this box
   */
  public function cache_info() {
    // Caching disabled so ajax pager gets init'ed.
  	return array();
  }

}
