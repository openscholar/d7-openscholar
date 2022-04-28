<?php

/**
 * @file
 * Provides class for os_boxes_tabs.
 */

/**
 * Provides class for os_boxes_tabs.
 */
abstract class os_boxes_widget_collection extends os_boxes_default {

  /**
   * Implements boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['widgets'] = array();
    return $options;
  }

  /**
   * Implements boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    $form['#attached'] = array(
      'js' => array(
        drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_widget_collection/os_boxes_widget_collection.form.js',
      ),
      'css' => array(
        drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_widget_collection/os_boxes_widget_collection.form.css',
      ),
    );

    $widgets = module_invoke_all('os_widget');

    // filter out factories
    $options = array();
    $widget_collection = array();
    ctools_include('layout', 'os');
    foreach ($widgets as $wgt => $info) {
      if (isset($info['info']) && $info['delta'] != $this->delta) {
        // not everything we get back from hook_os_widget has an info field.
        // also, don't list yourself because infinite loops are bad
        $options[$wgt] = trim($info['info']);
        if (!empty($info['title'])) {
          $options[$wgt] .= ' ('.trim($info['title']).')';
        }

        // Saving widget title and description in array to use in JavaScript Layer as Drupal.settings object.
        $widget_collection[$wgt]['title'] = empty($info['title']) ? trim($info['info']) : trim($info['title']);
        $widget_collection[$wgt]['desc'] = $options[$wgt];
      }
    }
    natcasesort($options);
    // Adding widget collection Drupal.settings object.
    drupal_add_js(array('os_boxes' => array('widget_collection' => $widget_collection)), 'setting');

    $form['add'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add Widget to Collection'),
      'new' => array(
        '#type' => 'select',
        '#options' => $options,
        '#field_suffix' => '<a class="button add-new">Add</a>'
      ),
      'help_text' => array(
        '#markup' => t('Note that any widgets added must have content or they WILL NOT display in this widget.
        If you add a widget to a page normally and it doesn\'t appear, it won\'t appear in this widget either.')
      )
    );

    $form['widgets'] = array(
      '#type' => 'markup',
      '#tree' => TRUE,
      '#description' => t('Drag the widgets to reorder them. The topmost widget will be the first shown.'),
      '#pre_render' => array(
        'os_boxes_widget_collection_table_prerender'
      )
    );

    $widgets = module_invoke_all('os_widget');

    // filter out factories
    $options = array();
    foreach ($widgets as $wgt => $info) {
      if (isset($info['info'])) {
        // not everything we get back from hook_os_widget has an info field.
        $options[$wgt] = $info['info'];
        if (!empty($info['title'])) {
          $options[$wgt] .= ' (' . trim($info['title']) . ')';
        } 
      }
    }

    $existing = $this->options['widgets'];

    $new_tab = array(
      'orig-title' => 'new',
      'title' => 'New Addition',
      // special case
      'weight' => '200000 new',
      'delta' => 'os_boxes_wc-remove',
      'bid' => '__blank__',
    );

    $widgets = array_merge($existing, array('new' => $new_tab));

    $links = array();

    foreach ($widgets as $k => $t) {
      if (strpos($k, 'widget-') === FALSE) {
        $id = 'widget-' . $k;
      }
      else {
        $id = $k;
      }

      $form['widgets'][$id] = array(
        '#weight' => $t['weight'],
        'bid' => array(
          '#type' => 'hidden',
          '#default_value' => $t['bid'],
        ),
        'orig-title' => array(
          '#markup' => '<span>'.(isset($options[$t['bid']])?$options[$t['bid']]:'').'</span>',
        ),
        'title' => array(
          '#type' => 'textfield',
          '#default_value' => $t['title'],
        ),
        'weight' => array(
          '#type' => 'hidden',
          '#default_value' => $t['weight'],
          // for js
          '#attributes' => array('class' => array('widget-weight')),
        ),
        'remove' => array(
          '#type' => 'markup',
          '#markup' => '<a href="#" class="remove" title="Remove Post">Remove</a>',
        ),
      );
    }


      // stores the number of real tabs
    // as soon as user picks a widget, this is incremented with js
    $form['widget_count'] = array(
      '#type' => 'hidden',
      '#default_value' => count($existing),
      '#process' => array('os_boxes_widget_collection_add_new_rows'),
      '#attributes' => array(
        'id' => 'edit-count',
      ),
    );

    return $form;
  }

  function render() {
    $block = parent::render();
    $block['allow_binding'] = true;

    return $block;
  }

  function render_widget($key) {
    module_load_include('module', 'block', 'block');
    $plugin = context_get_plugin('reaction', 'block');
    $blocks = $plugin->get_blocks();

    $widget = $this->options['widgets'][$key];

    if ($b = $blocks[$widget['bid']]) {
      $b->region = $this->delta;
      if ($b_out = _block_render_blocks(array($b))) {
        if (!empty($b_out[$b->module.'_'.$b->delta]->content)) {

          $block_output = _block_get_renderable_array($b_out);

          return $block_output[$b->module.'_'.$b->delta];
        }

      }
    }

    return array();
  }
}

/**
 * Includes new (dynamically added) tabs in form processing.
 *
 * After submission, the form has no idea how many tabs were added through js
 * I find out here and add new form elements for each new one.
 * This form element is placed before tabs so that the new elements go
 * through the builder properly. Removing a tab is handled in the submit.
 * Removing a tab and adding a new one should handle the same as editting an existing
 */
function os_boxes_widget_collection_add_new_rows(&$element, &$form_state, &$form) {
  if (!empty($form_state['input'])) {
    $new_widgets = array_diff_key($form_state['input']['widgets'], $form_state['values']['widgets']);
    foreach ($new_widgets as $new => $w) {
      $id = str_replace('widget-', '', $new);
      $form['options']['widgets'][$new] = array(
        '#attributes' => array('id' => $id),
        '#parents' => array('widgets', $new),
        '#array_parents' => array('options', 'widgets', $new),
        '#tree' => TRUE,
      );
      $form['options']['widgets'][$new] += $form['options']['widgets']['widget-new'];
      foreach ($form['options']['widgets'][$new] as $key => & $child) {
        if (strpos($key, '#') !== 0) {
          unset($child['#parents']);
          unset($child['#name']);
          unset($child['#value']);
        }
      }
      form_builder($form['form_id']['#value'], $form['options']['widgets'][$new], $form_state);
    }
  }
  unset($form_state['values']['widgets']['widget-new']);

  // Removes nids that aren't in the post
  if (!empty($form_state['values']['widgets'])) {
    foreach ($form_state['values']['widgets'] as $k => $v) {
      if (!isset($form_state['input']['widgets'][$k]) || !isset($form_state['input']['widgets'][$k]['bid']) || $form_state['input']['widgets'][$k]['bid'] == 'os_boxes_wc-remove') {
        unset($form_state['values']['widgets'][$k]);
      }
    }
  }
  return $element;
}

function os_boxes_widget_collection_table_prerender($element) {  // render the form as a table
  $rows = array();
  uasort($element, 'element_sort');
  foreach (element_children($element) as $c) {
    $rows[] = array(
      'data' => array(
        drupal_render($element[$c]['bid']).drupal_render($element[$c]['orig-title']),
        drupal_render($element[$c]['title']),
        drupal_render($element[$c]['remove']),
        drupal_render($element[$c]['weight']),
      ),
      'class' => (string)$c!='blank'?array('draggable'):array(),
    );
  }
  $element['#headers'] = array();
  $element['#attributes'] = array('id' => 'widget-list');
  $element['#rows'] = $rows;
  $element['#sticky'] = false;
  $element['#empty'] = true;
  //$element['#value'] = theme('table', array(), $rows, );
  $element['#theme'] = 'table';
  drupal_add_tabledrag('widget-list', 'order', 'sibling', 'widget-weight');
  return $element;
}