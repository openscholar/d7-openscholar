<?php

class os_boxes_widget_slider extends os_boxes_widget_collection {

  public $embed_as = array(
    'iframe',
  );

  public function options_defaults() {
    $options = parent::options_defaults();
    return $options + array(
      'effect' => 'fade',
      'timeout' => 5000,
      'speed' => 700,
      'display_scrollbar' => FALSE,
      'display_nav_arrow' => FALSE,
      'slider_height' => '400'
    );
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    // Fetching only HTML widgets and personal_contact widget as it is also an HTML widget.
    $widgets = module_invoke_all('os_widget');
    $widget_options = array();
    foreach ($widgets as $wgt => $info) {
      if ($info['module'] == 'boxes' && isset($info['delta']) && (boxes_box_load($info['delta'])->plugin_key == 'os_boxes_html' || boxes_box_load($info['delta'])->plugin_key == 'os_boxes_columns')) {
        $widget_options[$wgt] = trim($info['info']);
        if (!empty($info['title'])) {
          $widget_options[$wgt] .= ' ('.trim($info['title']).')';
        }
      }
    }
    asort($widget_options, SORT_FLAG_CASE | SORT_NATURAL);
    // Rendering HTML widgets in Widget select list.
    $form['add']['new']['#options'] = $widget_options; 
    
    $defaults = $this->options_defaults();
    $form['advanced']['#after_build'][] = '_os_boxes_widget_slider_field_display_handler';
    $form['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display Options'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 10,
    );

    $form['display']['timeout'] = array(
      '#type' => 'select',
      '#description' => t('Each slide will be displayed for this length of time.'),
      '#title' => t('Duration'),
      '#default_value' => isset($this->options['timeout']) ? $this->options['timeout'] : $defaults['timeout'],
      '#options' => $this->_transition_speed_opts(1, 10, .5),
    );

    $form['display']['speed'] = array(
      '#type' => 'select',
      '#description' => t('Transitions between slides will take this long.'),
      '#title' => t('Transition Speed'),
      '#default_value' => isset($this->options['speed']) ? $this->options['speed'] : $defaults['speed'],
      '#options' => $this->_transition_speed_opts(0, 1, .1),
    );

    $form['display']['display_scrollbar'] = array(
      '#type' => 'checkbox',
      '#description' => t('Render a vertical scrollbar to view longer contents.'),
      '#title' => t('Display scrollbar'),
      '#default_value' => isset($this->options['display_scrollbar']) ? $this->options['display_scrollbar'] : $defaults['display_scrollbar'],
    );

    $form['display']['display_nav_arrow'] = array(
      '#type' => 'checkbox',
      '#description' => t('Left and right arrows will display on either side of the navigation dots.'),
      '#title' => t('Display navigation arrows'),
      '#default_value' => isset($this->options['display_nav_arrow']) ? $this->options['display_nav_arrow'] : $defaults['display_nav_arrow'],
    );

    $form['display']['slider_height'] = array(
      '#type' => 'textfield',
      '#description' => t('Enter a numeric value for height of the slider in pixels. If left blank, Slider height will be calculated as 400px.'),
      '#title' => t('Slider height in pixels'),
      '#element_validate' => array('os_boxes_widget_slider_height_validate'),
      '#default_value' => isset($this->options['slider_height']) ? $this->options['slider_height'] : $defaults['slider_height'],
    );

    return $form;
  }

   /**
   * @function _transition_speed_opts()
   *
   * Helper function for making for time selects.  All args are seconds.
   *
   * @returns array( $milliseconds => $seconds . 's')
   */
  private function _transition_speed_opts($min, $max, $increment) {
    $precision = explode('.', $increment);
    $decimals = (count($precision) > 1) ? strlen($precision[1]) : 0;

    $return = array();
    for ($i = $min; $i <= $max; $i += $increment) {
      $return[$i * 1000] = number_format($i, $decimals) . ' sec';
    }
    return $return;
  }
  
  public function render() {
    $block = parent::render();
    $defaults = $this->options_defaults();
    $slides = array();
    // Sorting slides according to their weight
    uasort($this->options['widgets'], 'drupal_sort_weight');
    // Iterating all selected widgets to be rendered.
    foreach ($this->options['widgets'] as $key => $widget) {
      list(, $delta) = explode('-', $widget['bid'], 2);
      if ($box = boxes_box_load($delta)) {
        $content = $box->render();
        // Setting inline height attributes for div wrapper for each widget content.
        $inline_height = !empty($this->options['slider_height']) ? $this->options['slider_height'] : $defaults['slider_height'];
        $title = !empty($widget['title']) ? '<h2>' . $widget['title'] . '</h2>' : '';
        $slides[] = '<div style="height:' . $inline_height . 'px;">' . $title . $content['content'] . '</div>';
      }
    }

    $vars = array();
    $vars['slides'] = $slides;
    // Setting inline height for outer most ul tag.
    $vars['widget_height'] = !empty($this->options['slider_height']) ? $this->options['slider_height'] : $defaults['slider_height'];
    $vars['display_scrollbar'] = !empty($this->options['display_scrollbar']) ? $this->options['display_scrollbar'] : FALSE;
    $block['content'] = theme('os_slideshow_widget_slideshow', $vars);
    // Assigning slideshow timing control variables.
    $timeout = isset($this->options['timeout']) ? $this->options['timeout'] : $defaults['timeout'];
    $speed = isset($this->options['speed']) ? $this->options['speed'] : $defaults['speed'];
    $assets = $this->setup_assets($this->delta, $timeout, $speed, 0);
    $this->add_assets($assets);

    if ($this->options['display_nav_arrow']) {
      $this->options['additional_classes'] = 'arrow-nav';
    }

    return $block;
  }

  /**
   * Setup needed assets for the os slideshow widget, which will be used with add_assets().
   *
   * @param $delta
   *   The delta of the box.
   * @param $timeout
   *   The flexslider timeout.
   * @param $speed
   *   The flexslider speed.
   * @param $random
   *   The flexslider random order boolean.
   * @return array
   *   Structured array of assets.
   */
  private function setup_assets($delta, $timeout, $speed, $random) {
    $assets = array();
    $assets['library'] = array();
    $assets['library'][] = array('os_slideshow', 'responsiveslides');

    $assets['js'] = array();
    $assets['js'][] = drupal_get_path('module', 'os_slideshow') . '/os_slideshow_slider.js';

    $assets['settings'] = array();
    $assets['settings'][] = array(
      // Settings reference from http://www.woothemes.com/flexslider/
      'os_slideshow' =>
        array('boxes-box-' . $delta => array(
          'pager' => TRUE,
          'nav' => TRUE,
          // Flex starts the timer after transition. Responsive starts it during.
          // Adding them will let responsive look like flex did.
          'timeout' => intval($timeout + $speed),
          'speed' => $speed,
          'random' => $random,
        ),
      )
    );

    $assets['css'] = array();
    $assets['css'][] = _os_slideshow_add_css();

    return $assets;
  }
}

/**
 * #after_build handler in options_form to display settings form field for widget_slider widget.
 * To hide the 'Widget Title' field while editing widget slider widget.
 */
function _os_boxes_widget_slider_field_display_handler($element, &$form_state) {
  $form_state['complete form']['title']['#access'] = FALSE;  
}

/**
 * Ensures the pixel value given is a numeric digit
 */
function os_boxes_widget_slider_height_validate($element, &$form_state) {
  if (!empty($element['#value']) && !is_numeric($element['#value'])) {
    form_error($element, t('Please enter a numeric value as pixels.'));
  }
  return $element;
}