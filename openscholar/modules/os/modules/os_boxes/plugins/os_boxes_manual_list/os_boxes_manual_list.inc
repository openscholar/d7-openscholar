<?php

class os_boxes_manual_list extends os_boxes_default {

  public $embed_as = array(
    'iframe',
    'oembed',
  );

  public function options_defaults() {
    return parent::options_defaults() + array(
      'nodes' => array(),
      'style' => 'titles',
      'random' => false,
      'hide_title' => 0,
      'callout_styling' => 1,
    );
  }

  public function options_form(&$form_state) {
    // Will prevent fatal error and the FAPI from failing.
    include_once (drupal_get_path('module', 'vsite') . '/includes/nodeautocomplete.inc');
    $form_state['build_info']['files'][] = drupal_get_path('module', 'vsite') . '/includes/nodeautocomplete.inc';
    $form = parent::options_form($form_state);

    $form['#attached']['js'][] = drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_manual_list/os_boxes_manual_list.js';

    $form['search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add New Post'),
      'node_to_add' => vsite_node_autocomplete_element(),
      'add' => array(
        '#type' => 'button',
        '#value' => t('Add post'),
        '#href' => '',
        '#add' => TRUE,
        '#ajax' => array(
          'callback' => 'os_boxes_manual_add',
          'wrapper' => 'nodes',
        ),
      ),
    );

    // Keep which nodes removed.
    $form_state['storage']['nodes'] = isset($form_state['storage']['nodes']) ? $form_state['storage']['nodes'] : 0;

    $form['nodes'] = array(
      '#type' => 'markup',
      '#tree' => TRUE,
      '#prefix' => '<div id="nodes">',
      '#suffix' => '</div>',
      '#pre_render' => array('os_boxes_manual_list_table'),
    );

    if (!empty($form_state['triggering_element']['#add'])) {

      if (empty($form_state['values']['node_to_add'])) {
        form_set_error('', t('Please choose a node to add.'));
      }
      else {
        // Collecting all the weight so we could get the maximum and place the
        // new added node to the bottom.
        $weights = array();
        foreach ($this->options['nodes'] as $node) {
          $weights[] = $node['weight'];
        }

        // Get the node.
        $explode = explode('[', $form_state['values']['node_to_add']);
        $nid = intval(end($explode));
        $node = node_load($nid);
        // The node is added again, remove from the list of removed nodes.
        unset($form_state['storage']['removed'][$nid]);
        $this->options['nodes'][$nid] = array('title' => $node->title, 'nid' => $node->nid, 'weight' => max($weights) + 1);
      }
    }

    // Check which node was removed.
    $form_state['storage']['removed'] = empty($form_state['storage']['removed']) ? array() : $form_state['storage']['removed'];

    if (!empty($form_state['triggering_element']['#remove'])) {
      $form_state['storage']['removed'][$form_state['triggering_element']['#remove']] = $form_state['triggering_element']['#remove'];
    }

    $nodes = &$form['nodes'];

    $delta = 0;
    foreach ($this->options['nodes'] as &$node) {
      if (in_array($node['nid'], $form_state['storage']['removed'])) {
        // Don't add the node to the list.
        continue;
      }
      $nodes[$node['nid']] = $this->addNode($node, $delta);
      $delta++;
    }

    // Blank element for copying purposes.
    $form['style'] = array(
      '#type' => 'select',
      '#title' => t('Display Style'),
      '#options' => array(
        'full' => t('List of full posts'),
        'teasers' => t('List of teasers'),
        'titles' => t('List of titles')
      ),
      '#required' => TRUE,
      '#default_value' => $this->options['style']
    );

    $form['callout_styling'] = array(
      '#type' =>'checkbox',
      '#title' => t('Apply call-out styling to the widget'),
      '#default_value' => $this->options['callout_styling'],
      '#description' => t('When checked, the widget will use background shading and/or unique font styling.'),
    );

    $form['random'] = array(
      '#type' =>'checkbox',
      '#title' => t('Random Post'),
      '#default_value' => $this->options['random'],
      '#description' => t('Check this box to show one random post out of the list, instead of all of them.'),
    );

    $form['hide_title'] = array(
      '#type' => 'checkbox',
      '#title' => 'Hide Titles',
      '#default_value' => isset($this->options['hide_title']) ? $this->options['hide_title'] : $defaults['hide_title'],
      '#description' => t('Hide titles of posts.'),
      '#states' => array(
        'disabled' => array(
          '#edit-style' => array('value' => 'titles'),
        ),
      ),
    );

    $form['count'] = array(
      '#type' => 'hidden',
      '#default_value' => count($this->options['nodes'])-1,
      '#attributes' => array(
        'id' => 'edit-count',
      ),
      '#process' => array('os_boxes_manual_list_add_new_fields'),
    );
    return $form;
  }

  public function render() {
    $block = parent::render();

    // Avoid caching if set to random.
    $random = $this->options['random'];

    if ($random || ($block['content'] = $this->get_cache()) === FALSE) {

      $links = array();

      $nodes = $this->options['nodes'];
      if ($random) {
        $nodes = array($nodes[array_rand($nodes)]);
      }

      foreach ($nodes as $n) {
        if ($node = node_load($n['nid'])) {
          if (!node_access('view', $node)) {
            continue;
          }
          // nullify title unless user has selected to show only titles
          if($this->options['style'] != 'titles' && $this->options['hide_title']) {
            $node->title = NULL;
          }
          if ($this->options['style'] == 'teasers') {
            // Theme the nodes as a teaser.

            // Node may not load if it has since been deleted.
            $rendered_node = node_view($node, 'teaser');
            $links[$n['weight']] = render($rendered_node);
          }
          elseif ($this->options['style'] == 'full') {
            $node = node_load($n['nid']);
            $rendered_node = node_view($node, 'full');
            if (empty($rendered_node['#contextual_links'])) {
              $rendered_node['#contextual_links']['node'] = array('node', array($n['nid']));
            }            
            $links[$n['weight']] = render($rendered_node);
          }
          else {
            $l = array('#node' => $node, '#theme' => 'node', '#view_mode' => 'title', '#children' => NULL);
            $l['#contextual_links']['node'] = array('node', array($n['nid']));
            $links[$n['weight']] = render($l);
          }
        }
      }
      ksort($links);
      if (empty($links)) {
        $block['title'] = $block['subject'] = '';
      }
      elseif  ($this->options['style'] == 'teasers' || $this->options['style'] == 'full') {
        $block['content'] = theme('item_list', array('items' => $links, 'attributes' => array('class' => array('featured-links'))));
      }
      else {
        $block['content'] = theme('item_list', array('items' => $links, 'attributes' => array('class' => array('featured-links title-only'))));
      }

      if (!$random) {
        $this->set_cache($block['content']);
      }
    }
    
    // Adding 'styled' class in parent block wrapper if 'callout styling' option is checked.
    if (isset($this->options['callout_styling']) && $this->options['callout_styling']) {
      $block['additional_classes'] = 'styled';
    }
    
    return $block;
  }

  /**
   * Adding a single node.
   *
   * @param $node
   *   The node array.
   * @param $delta
   *   The delta of the node.
   *
   * @return array
   *   A new table row.
   */
  private function addNode($node, $delta) {
    return array(
      '#weight' => $node['weight'],
      'nid' => array(
        '#type' => 'hidden',
        '#default_value' => $node['nid'],
      ),
      'text' => array(
        '#type' => 'markup',
        '#markup' => $node['title'],
      ),
      'title' => array(
        '#type' => 'hidden',
        '#default_value' => $node['title'],
      ),
      'remove' => array(
        '#type' => 'button',
        '#name' => 'remove_' . $node['nid'],
        '#value' => t('Remove'),
        '#remove' => $node['nid'],
        '#ajax' => array(
          'callback' => 'os_boxes_manual_add',
          'wrapper' => 'nodes',
        ),
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $node['weight'],
        '#size' => 3,
        '#attributes' => array(
          'class' => (string)$delta != 'blank' ? array('field-weight') : array(),
        ),
      ),
    );
  }

  /**
   * {@inheritDoc}
   */
  public function options_submit($form, $form_state) {
    // When assigning the first element as the last element and then adding
    // another without the changing the order the weight of the nodes get messy.
    // The loop will verify that nodes won't have the same weight.
    $weights = array();
    foreach ($this->options['nodes'] as &$node) {

      if (in_array($node['weight'], $weights)) {
        $node['weight'] = max($weights) + 1;
      }

      $weights[] = $node['weight'];
    }

    parent::options_submit($form, $form_state);
  }
}

function os_boxes_manual_list_table($element) {
  // Render the form as a table.
  $rows = array();
  uasort($element, 'element_sort');
  foreach (element_children($element) as $c) {
    $rows[] = array(
      'data' => array(
        drupal_render($element[$c]['nid']) . drupal_render($element[$c]['text']) . drupal_render($element[$c]['title']),
        drupal_render($element[$c]['weight']),
        drupal_render($element[$c]['remove']),
      ),
      'class' => (string)$c!='blank'?array('draggable'):array(),
    );
  }
  $element['#headers'] = array();
  $element['#attributes'] = array('id' => 'manual-nodes-list');
  $element['#rows'] = $rows;
  $element['#sticky'] = false;

  $element['#theme'] = 'table';
  drupal_add_tabledrag('manual-nodes-list', 'order', 'sibling', 'field-weight');
  return $element;
}

function os_boxes_manual_list_add_new_fields($element, &$form_state, &$form) {
  for ($i=$element['#default_value']; $i < $element['#value']; $i++) {
    $form['options']['nodes'][$i] = array(
      '#attributes' => array('id' => $i),
      '#parents' => array('nodes', $i),
      '#array_parents' => array('options', 'nodes', $i),
      '#tree' => TRUE,
    );
    $form['options']['nodes'][$i] += $form['options']['nodes']['blank'];
    foreach ($form['options']['nodes'][$i] as $key => &$child) {
      if (strpos($key, '#') !== 0) {
        unset($child['#parents']);
        unset($child['#name']);
        unset($child['#value']);
      }
    }
    form_builder($form['form_id']['#value'], $form['options']['nodes'][$i], $form_state);
  }
  unset($form_state['values']['nodes']['blank']);

  // Remove nids that aren't in the post.
  if (!empty($form_state['values']['nodes'])) {
    foreach ($form_state['values']['nodes'] as $k => $v) {
      if (!isset($form_state['input']['nodes'][$k]) || !isset($form_state['input']['nodes'][$k]['nid'])) {
        unset($form_state['values']['nodes'][$k]);
      }
    }
  }

  return $element;
}

/**
 * Ajax form callback.
 */
function os_boxes_manual_add($form, $form_state) {
  return $form['options']['nodes'];
}
