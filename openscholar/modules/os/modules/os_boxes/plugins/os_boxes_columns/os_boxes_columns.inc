<?php

class os_boxes_columns extends os_boxes_default {

  public function options_defaults() {
    return array(
      'layout' => 'three-3x33',
      'additional_class' => '',
      'widgets' => array(
        'region-1' => '',
        'region-2' => '',
        'region-3' => '',
        'region-4' => '',
        'region-5' => '',
      ),
    );
  }

  public function options_form(&$form_state) {

    // set up layout selection controls
    $form['layout_select'] = array(
      '#type' => 'container',
      '#id' => 'os-boxes-columns-layout-select'
    );

    if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
      $form['additional_class'] = array(
        '#type' => 'textfield',
        '#title' => t('Apply css class name to this widget\'s container'),
        '#description' => t('Warning: this is to be used for custom themeing purposes only.'),
        '#default_value' => (isset($this->options['additional_class'])) ? $this->options['additional_class'] : '',
        '#access' => og_user_access('node', $vsite->group->nid, 'add widget custom class', NULL, FALSE, TRUE),
        '#weight' => 9002,
      );
    }

    /*
    $form['layout_select']['num_cols'] = array(
      '#type' => 'radios',
      '#title' => t('Layout Selection'),
      '#options' => array(
        '0' => t('All'),
        '2' => t('Two'),
        '3' => t('Three'),
        '4' => t('Four'),
        '5' => t('Five')
      ),
      '#default_value' => '0',
    );*/

    $form['layout_select']['layout'] = array(
      '#type' => 'radios',
      '#options' => array(
        'two-2x50' => '50/50 (%)',
        'two-33-66' => '33/66 (%)',
        'two-66-33' => '66/33 (%)',
        'three-3x33' => '33/33/33 (%)',
        'three-25-25-50' => '25/25/50 (%)',
        'three-25-50-25' => '25/50/25 (%)',
        'three-50-25-25' => '50/25/25 (%)',
        'four-4x25' => '4x25 (%)',
        'five-5x20' => '5x20 (%)'
      ),
      '#default_value' => $this->options['layout'],
    );

    // set up unused widget container
    $form['unused'] = array(
      '#type' => 'container',
      '#id' => 'unused-widgets',
      'title' => array(
        '#markup' => t('<span class="formlabel">List of Widgets</span>'),
      ),
      'search' => array(
        '#type' => 'textfield',
        '#title' => t('Filter by Name'),
      ),
      'widgets' => array(
        '#type' => 'container',
      )
    );

    $form['help_text'] = array(
      '#markup' => t('<span class="helper columns">Choose a column configuration above, drag unused widgets into the <span class="downarrow">columns</span>, and save. Once saved, you\'ll be able to drag this widget into the main layout page.</span>')
    );

    // set up psuedo-regions
    // this element is for the hidden input elements
    $form['widgets'] = array(
      '#tree' => true,
    );

    // this element is for the actual regions
    // js will convert the input elements into a widget list
    // while
    $form['regions'] = array(
      '#type' => 'container',
    );

    for ($i = 1; $i <= 5; $i++) {
      $form['regions']['region-'.$i] = array(
        '#type' => 'container',
        '#id' => 'region-'.$i.'-container',
        '#attributes' => array(
          'class' => array(
            'region'
          ),
          'data-region_id' => $i
        ),
      );
      $form['widgets']['region-'.$i] = array(
        '#type' => 'hidden',
        '#default_value' => $this->options['widgets']['region-'.$i],
        '#id' => 'region-'.$i.'-storage',
        '#attributes' => array(
          'class' => array(
            'region_storage'
          ),
          'data-region_id' => $i
        ),
      );
    }

    /**
     * Get all widgets and sort them accordingly
     */
    ctools_include('layout', 'os');
    $blocks = os_layout_get_multiple(array('os_public'), true, true);

    $regions = array();
    $temp = $this->options['widgets'];
    if (!empty($form_state['input']) && !$form_state['submitted']) {
      $temp = $form_state['input']['widgets'];
    }
    foreach ($temp as $region => $widgets) {
      $regions[$region] = explode('|', $widgets);
    }

    foreach ($blocks as $bid => $b) {
      // prevent infinite loops
      if ($b['delta'] == $this->delta) continue;

      foreach ($regions as $region => $widgets) {
        if (($weight = array_search($bid, $widgets)) !== FALSE) {
          $form['regions'][$region][$bid] = array(
            '#theme' => 'cp_layout_widget',
            '#widget' => $b,
            '#weight' => $weight,
          );
          continue 2;
        }
      }
      if ($b['region'] == -1 || $b['region'] === FALSE) {
        $form['unused']['widgets'][$bid] = array(
          '#theme' => 'cp_layout_widget',
          '#widget' => $b,
          '#weight' => trim(strtolower($b['info']))
        );
      }
    }

    // add all the js and css we're gonna need
    $form['#attached'] = array(
      'library' => array(
        array('system', 'ui.sortable'),
      ),
      'js' => array(
        drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_columns/js/os_boxes_columns.form.js'
      ),
      'css' => array(
        drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_columns/css/os_boxes_columns.form.css'
      )
    );

    $form['clear'] = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'clearfix'
        )
      )
    );

    return $form;
  }

  /**
   * Hmm
   */
  public function render() {
    module_load_include('module', 'block', 'block');

    $block = parent::render();
    $block['allow_binding'] = true;
    $page = array();
    $layout = $this->options['layout'];
    $blocks = array();
    foreach ($this->options['widgets'] as $region => $widgets) {
      if (empty($widgets)) continue;

      $temp = explode('|', $widgets);
      $region = $this->get_gpanel_region($region, $layout);
      foreach ($temp as $weight => $id) {
        list($module, $delta) = explode('-', $id, 2);
        $blocks[$region][$id] = (object)array(
          'module' => $module,
          'delta' => $delta,
          'weight' => $weight,
          'region' => $region
        );
      }

      $blocks[$region] = _block_render_blocks($blocks[$region]);
      $page[$region] = _block_get_renderable_array($blocks[$region]);
      $page[$region] += array(
        '#sorted' => true,
        '#theme_wrappers' => array(
          'region',
        ),
        '#region' => $region
      );
    }
    if (!empty($page)) {
      $template = drupal_get_path('theme', 'adaptivetheme').'/layouts/gpanels/'.$layout.'.php';
      $block['content'] = @theme_render_template($template, array('page' => $page));
      drupal_add_css(drupal_get_path('theme', 'adaptivetheme').'/layouts/css/default.css');
    }

    return $block;
  }

  /**
   * For a given psuedo-region and layout, return the gpanel name of that region
   */
  private function get_gpanel_region($region, $layout) {
    $map = &drupal_static(__FUNCTION__, array(
      // Two column layouts
      'two-2x50' => array(
        'region-1' => 'two_50_first',
        'region-2' => 'two_50_second',
      ),
      'two-33-66' => array(
        'region-1' => 'two_33_66_first',
        'region-2' => 'two_33_66_second',
      ),
      'two-66-33' => array(
        'region-1' => 'two_66_33_first',
        'region-2' => 'two_66_33_second',
      ),
      // Three column layouts
      'three-3x33' => array(
        'region-1' => 'three_33_first',
        'region-2' => 'three_33_second',
        'region-3' => 'three_33_third',
      ),
      'three-25-25-50' => array(
        'region-1' => 'three_25_25_50_first',
        'region-2' => 'three_25_25_50_second',
        'region-3' => 'three_25_25_50_third',
      ),
      'three-25-50-25' => array(
        'region-1' => 'three_25_50_25_first',
        'region-2' => 'three_25_50_25_second',
        'region-3' => 'three_25_50_25_third',
      ),
      'three-50-25-25' => array(
        'region-1' => 'three_50_25_25_first',
        'region-2' => 'three_50_25_25_second',
        'region-3' => 'three_50_25_25_third',
      ),
      // Four column layouts
      'four-4x25' => array(
        'region-1' => 'four_first',
        'region-2' => 'four_second',
        'region-3' => 'four_third',
        'region-4' => 'four_fourth',
      ),
      // Five column layouts
      'five-5x20' => array(
        'region-1' => 'five_first',
        'region-2' => 'five_second',
        'region-3' => 'five_third',
        'region-4' => 'five_fourth',
        'region-5' => 'five_fifth',
      )
    ));

    return (isset($map[$layout][$region]))?$map[$layout][$region]:$region;
  }
}
