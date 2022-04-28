<?php

/**
 * @file
 * The box class for "Twitter feed" type widgets.
 */

class os_boxes_slideshow extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['gallery_nid'] = 'all';
    $options['pager'] = 'none';
    $options['controls'] = 'none';
    $options['slide_counter'] = 'none';
    $options['show_image_titles'] = 'none';
    $options['items_per_page'] = 10;
    $options['effect'] = 'fade';
    $options['style'] = 'dark';
    $options['size'] = 600;
    $options['timeout'] = 5000;
    $options['speed'] = 700;
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $private = os_get_bundles(array(0));
    $this->options += $this->options_defaults();
    
    if (isset($private['media_gallery'])) {
      return array(
        '#type' => 'markup',
        '#prefix' => '<div class="warning messages">',
        '#markup' => t('You must enable the !link to create a slideshow.', array('!link' => l('Media Gallery app', 'cp/build/features'))),
        '#suffix' => '</div>',
      );
    }
    
    $form = parent::options_form($form_state);

    // Get the media gallery nodes.
    $galleries = array();
    ctools_include('node', 'os');
    $efq = new OSNodeFieldQuery();
    $efq->propertyCondition('type', 'media_gallery');
    $result = $efq->execute();

    if (isset($result['node']) && count($result['node'])) {
      foreach(node_load_multiple(array_keys($result['node'])) as $node) {
        $galleries[$node->nid] = $node->title;
      }
    }

    if (count($galleries)) {
      $form['gallery_nid'] = array(
        '#type' => 'select',
        '#title' => t('Gallery'),
        '#description' => t('Select a gallery to use as a slideshow.'),
        '#options' => array('all' => t('All')) + $galleries,
        '#default_value' => $this->options['gallery_nid'],
      );
    }
    else {
      $form['no_gallery_message'] = array(
        '#type' => 'markup',
        '#markup' => '<div>' . t('You must create a gallery to use as a slideshow.') . '</div>',
      );
    }

    $form['new_gallery'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Create new gallery'), 'node/add/media-gallery'), //overlay?
    );

    $form['display'] = array(
      '#type' => 'fieldset',
      '#title' => t('Display Options'),
      '#collapsible' => TRUE,
    	'#collapsed' => TRUE,
    );

    $form['display']['style'] = array(
      '#title' => t('Player Styles'),
      '#type' => 'select',
      '#options' => array('dark' => t('Dark'), 'light' => t('Light')),
      '#default_value' => $this->options['style'],
      '#attributes' => array('class' => array('thumbnails')),
      '#description' => t('Choose between a black or light grey player style.'),
    );

    $form['display']['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Size'),
      '#default_value' => $this->options['size'],
      '#field_suffix' => 'px',
      '#description' => t('Enter the width of the slideshow in pixels. It should be between 400 and 960 pixels.'),
      '#element_validate' => array('os_boxes_slideshow_accepted_range'),
      '#attached' => array(
        'js' => array(
          drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_slideshow/os_slideshow_form.js'
        )
      ),
    );

    $display_options = array('pager' => t('Show Pager'), 'controls' => t('Show Controls'), 'slide_counter' => t('Slide Counter'));
    foreach ($display_options as $option => $title)
    $form['display'][$option] = array(
      '#type' => 'select',
      '#title' => $title,
      '#options' => array('none' => t('No'), 'bottom' => t('Yes')),
      '#default_value' => $this->options[$option],
    );

    $form['display']['show_image_titles'] = array(
      '#type' => 'select',
      '#title' => t('Show image titles'),
      '#options' => array('none' => t('No'), 'top' => t('Yes')),
      '#default_value' => $this->options['show_image_titles'],
    );

    $form['display']['items_per_page'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of items'),
      '#description' => t('Limit to this many images per slideshow.  Use 0 for no limit.'),
      '#default_value' => (int) $this->options['items_per_page'],
    );

    $form['display']['timeout'] = array(
      '#type' => 'select',
      '#title' => t('Duration'),
      '#options' => $this->_transition_speed_opts(1, 10, .5),  
      '#default_value' => $this->options['timeout'],
    );

    $form['display']['speed'] = array(
      '#type' => 'select',
      '#title' => t('Transition Speed'),
      '#options' => $this->_transition_speed_opts(0, 1, .1),
      '#default_value' => $this->options['speed'],
    );

    // Get jquery effects from view (include file should already be loaded).
    $slideshow_form_full = $state = array();
    if (module_exists('views_slideshow_cycle')) {
      if (!function_exists('views_slideshow_cycle_views_slideshow_slideshow_type_form')) {
        require_once drupal_get_path('module', 'views_slideshow_cycle') . '/views_slideshow_cycle.views_slideshow.inc';
      }
      @views_slideshow_cycle_views_slideshow_slideshow_type_form($slideshow_form_full, $state, $view);
      $form['display']['effect'] = $slideshow_form_full['views_slideshow_cycle']['effect'];
      $form['display']['effect']['#default_value'] = $this->options['effect'];
    }

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {

    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }
    $block = parent::render();
    $this->options += $this->options_defaults();
    
    //no slideshow when media gallery is disabled
    $private = os_get_bundles(array(0));
    if (isset($private['media_gallery'])) {
      return $block;
    }

    if ($view = views_get_view('os_image_gallery_slideshow')) {
      $bundles = array(
        'media_gallery',
        'slideshow_slide'
      );
      $this->set_cache_id($bundles);
      if (($block['content'] = $this->get_cache()) === FALSE) {

        drupal_add_css(drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_slideshow/theme/os-slideshow-' . $this->options['style'] . '.css');
        drupal_add_css(drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_slideshow/os_boxes_slideshow.css');

        $view->init_display();
        $handler = $view->display_handler;
        $style_options = $handler->get_option('style_options');

        // Add slideshow widgets.
        foreach(array('pager', 'controls', 'slide_counter') as $option) {
          foreach($style_options['widgets'] as $position => $info) {
            $style_options['widgets'][$position]['views_slideshow_' . $option]['enable'] = ($position == $this->options[$option]);
          }
        }

        // Limit to one gallery.
        if (((int)$this->options['gallery_nid']) > 0) {
          $filters = $handler->get_option('filters');
          $filters['nid'] = array(
            'id' => 'nid',
            'table' => 'node',
            'field' => 'nid',
            'relationship' => 'file_to_node',
            'group_type' => 'group',
            'ui_name' => '',
            'operator' => '=',
            'group' => '1',
            'exposed' => FALSE,
            'expose' => array(),
            'value' => array('min'=>'', 'max'=>'', 'value'=>$this->options['gallery_nid']),
          );
          $handler->set_option('filters', $filters);
        }

        // Effects.
        $style_options['views_slideshow_cycle']['effect'] = $this->options['effect'];
        $style_options['views_slideshow_cycle']['timeout'] = $this->options['timeout'];
        $style_options['views_slideshow_cycle']['speed'] = $this->options['speed'];
        $handler->set_option('style_options', $style_options);

        // Title.
        if ($this->options['show_image_titles'] == 'none') {
          $fields = $handler->get_option('fields');
          $fields['media_title']['exclude'] = TRUE;
          $handler->set_option('fields', $fields);
        }

        // Number of items.
        if ((int)$this->options['items_per_page'] > 0) {
          $view->set_items_per_page((int) $this->options['items_per_page']);
        }
        else {
          $handler->set_option('use_pager', FALSE);
        }

        $output = $view->preview();

        // Dynamic size adjustments.
        $styles = image_styles();
        $prefix = 'slideshow_';
        $data = array();
        foreach ($styles as $name => $presets) {
          if (strpos($name, $prefix) === 0) {
            $dim = current($presets['effects']);
            $dim = $dim['data'];
            $data[$dim['width']] = $dim;
          }
        }
        ksort($data);

        foreach ($data as $width => $dim) {
          if ($this->options['size'] <= $width) {
            $ratio = $dim['height']/$dim['width'];
            $preset = $prefix.$width;
            break;
          }
        }

        $width = $this->options['size'];
        $height = $width * $ratio;

        // Swap out the dimenions of the preset with the dimenions the user wants.
        $patt_w = '/(<img [^>]* width=")([^"]*)("[^>]*>)/';
        $patt_h = '/(<img [^>]* height=")([^"]*)("[^>]*>)/';
        $patt_url = '/<img [^>]*src="([^"]*)"[^>]*>/';

        $output = preg_replace($patt_w, '${1}'.($width).'px'.'${3}', $output);
        $output = preg_replace($patt_h, '${1}'.($height).'px'.'${3}', $output);
        $matches = array();
        preg_match_all($patt_url, $output, $matches, PREG_SET_ORDER);
        foreach ($matches as $m) {
          $output = str_replace($m[1], image_style_url($preset, $m[1]), $output);
        }
        $assets = array(
          'css' => array(
            drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_slideshow/theme/os-slideshow-' . $this->options['style'] . '.css',
            drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_slideshow/os_boxes_slideshow.css',
          ),
        );
        $block['content'] = $output;
        $block['assets'] = $assets;
        $this->set_cache($block['content'], $assets);
      }
    }

    return $block;
  }

  /**
   * Return the cache parameters for this box
   */
  public function cache_info() {
    return array();
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
    $decimals = (count($precision)>1) ? strlen($precision[1]) : 0;
  
    $return = array();
    for ($i = $min; $i <= $max; $i += $increment) {
      $return[$i*1000] = number_format($i, $decimals) . ' sec';
    }
    return $return;
  }
}

/**
 * Ensures the value given is within acceptable ranges
 */
function os_boxes_slideshow_accepted_range($element, &$form_state) {
  if ($element['#value'] < 400 || $element['#value'] > 960) {
    form_error($element, t('Slideshow width out of bounds: Width should be between 400 and 960 pixels.'));
  }
  $element['#value'] = (int)$element['#value'];
}
