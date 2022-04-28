<?php
// $Id$


/**
 * @author rbran100
 * @category boxes
 *
 * The class for filtering content by taxonomy.
 */
class os_taxonomy_fbt extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implements boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['vocabulary'] = FALSE;
    $options['bundles'] = array('--all--');
    $options['widget_type'] = 'list';
    $options['range'] = 10;
    $options['offset'] = 0;
    $options['show_empty_terms'] = FALSE;
    $options['show_term_descriptions'] = FALSE;
    $options['show_count'] = TRUE;
    $options['show_childs'] = TRUE;
    $options['add_childs'] = FALSE;
    $options['depth'] = 0;
    $options['behavior'] = 'contextual';
    $options['as_nav'] = FALSE;
    $options['additional_class'] = '';

    return $options;
  }

  /**
   * Implements boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    ctools_include('dependent');
    ctools_add_js('dependent');

    $form = $this->form_elements($form_state);

    // Reset the keys.
    foreach (array_keys($form) as $setting) {
      $option_value = $this->options[$setting];
      // The form field 'depth' has an option with the value "0", This is why
      // we check it literally so we won't reset it on that case.
      $setting_value = !empty($option_value) || $option_value === '0' ? $this->options[$setting] : NULL;
      $form[$setting]['#default_value'] = $setting_value;
    }

    $form['advanced']['as_nav'] = array(
      '#type' => 'radios',

      // The previous version of the form had a checkbox for the 'Menu' display type, so
      // for back-compatibility, checkbox true is equivalent to radio 'menu'.
      '#default_value' => (($this->options['as_nav'] === true) ? 'menu' : (is_string($this->options['as_nav']) ? $this->options['as_nav'] : 'classic')),
      '#title' => t('Display type'),
      '#return_value' => TRUE,
      '#options' => array(
          'classic' => t('Classic/List'),
          'menu' => t('Menu'),
          'slider' => t('Slide Down') . '<br><div class="description">' . t('Each taxonomy term must have its own description (see the term creation/edit form). Also, the Show term descriptions checkbox above must be checked, and the Show child terms checkbox above must be unchecked.')
      ),
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

    return $form;
  }

  /**
   * Returning the form for building the form and reset the form values with
   * $form keys.
   */
  public function form_elements(&$form_state) {
    $form = array();

    drupal_add_js(drupal_get_path('module', 'os_boxes') . '/plugins/os_taxonomy_fbt/os_taxonomy_fbt.js');

    $selected_vid = isset($form_state['values']['vocabulary']) ? $form_state['values']['vocabulary'] : $this->options['vocabulary'];
    $form['vocabulary'] = array(
      '#type' => 'select',
      '#description' => t('Which vocabularies would you like to use to filter posts.'),
      '#title' => t('Vocabularies'),
      '#options' => self::get_all_vocabularies(),
      '#required' => TRUE,
      '#attributes' => array(
        'ng-non-bindable' => '',
      ),
      '#ajax' => array(
        'callback' => 'os_boxes_taxonomy_fbt_vocab_ajax_callback',
        'wrapper' => 'bundles-wrapper',
      ),
    );

    $form['widget_type'] = array(
      '#type' => 'select',
      '#title' => t('Widget type'),
      '#description' => t('Select how to display the terms - in a select list or in html format'),
      '#options' => array(
        'list' => t('List'),
        'select' => t('Select list'),
      ),
      '#default_value' => $this->options['widget_type'] ? $this->options['widget_type'] : 'list',
    );

    $form['behavior'] = array(
      '#type' => 'select',
      '#title' => t('Post types'),
      '#options' => array(
        'contextual' => t("Determine for me"),
        '--all--' => t('All posts'),
        'select' => t('Select post type'),
      ),
      '#default_value' => $this->options['behavior'],
      '#description' => t('The post types that will included in the count. Determine for me will filter the content on the "current page".'),
    );

    $form['bundles'] = array(
      '#prefix' => '<div id="bundles-wrapper">',
      '#suffix' => '</div>',
      '#type' => 'select',
      '#title' =>  t('Type'),
      '#options' => $this->getTaxonomyBundles($selected_vid),
      '#multiple' => FALSE,
      '#size' => ceil(count($this->getTaxonomyBundles($selected_vid)) / 2),
      '#dependency' => array(
        'edit-behavior' => array('select'),
      ),
    );

    $form['range'] = array(
      '#type' => 'textfield',
      '#title' => t('Maximum number of parent terms to display'),
      '#element_validate' => array('element_validate_integer_positive'),
    );

    $form['offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Start at'),
      '#description' => t("By default the list of terms will start at the first one, if you specify (3) for example it will skip the first 3 terms and start the list at the fourth."),
    );

    $form['show_empty_terms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show empty terms (terms with no posts tagged with them)'),
    );

    $form['show_term_descriptions'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show term descriptions'),
    );

    $form['show_childs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show child terms'),
    );

    $form['depth'] = array(
      '#type' => 'select',
      '#title' => ('Taxonomy tree depth'),
      '#description' => t('How many levels would you like to display?'),
      '#options' => array(
        -1 => t("Show all children"),
        0 => t("Show only first level"),
        1 => t("2nd Level"),
        2 => t("3rd Level"),
        3 => t("4th Level"),
      ),
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-show-childs' => array(1),
      ),
    );

    $form['show_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show number of posts'),
      '#description' => t('Show number of posts tagged to a term next to the term.'),
    );

    return $form;
  }

  /**
   * Return the available bundles with the future and past events bundles.
   * 
   * @return array
   */
  protected function getTaxonomyBundles($vid = NULL) {
    $info = &drupal_static(__METHOD__);

    if ($info) {
      return $info;
    }

    $bundles = os_get_bundles(array(1,2));
    $allowed_bundles = array();

    if ($vid != NULL) {
      $allowed_bundles = db_select('og_vocab', 'ogv')
        ->fields('ogv', array('bundle'))
        ->condition('ogv.vid', $vid, '=')
        ->condition('ogv.entity_type', 'node', '=')
        ->execute()
        ->fetchCol();
    }

    // Removing Feed Importer and Slideshow Image from dropdown.
    foreach ($bundles as $key => $val) {
      if (in_array($key, array('feed_importer', 'slideshow_slide'))) {
        unset($bundles[$key]);
      }
      else if (!in_array($key, $allowed_bundles) && $vid != NULL) {
        unset($bundles[$key]);
      }
    }
    // Checking if event bundle is available in allowed list of bundles.
    $delta = array_search('event', array_keys($bundles));
    $events_type = array(
      'past_event' => t('Past event'),
      'upcoming_event' => t('Upcoming event'),
    );
    if (!empty($delta)) {
      $first = array_slice($bundles, 0, $delta);
      $second = array_slice($bundles, $delta + 1);
      $info = $first + $events_type + $second;
    }
    // Case when delta is 0, but event is presents.
    elseif (in_array('event', array_keys($bundles))) {
      $info = $events_type + $bundles;
      unset($info['event']);
    }
    else {
      return $bundles;
    }
    return $info;
  }

  /**
   * When submitting a box we need to invalidate here.
   */
  public function options_submit($form, $form_state) {
    if (module_exists('vsite') && !$vsite = vsite_get_vsite()) {
      return;
    }

    if (isset($form_state['values']['as_nav']) && $form_state['values']['as_nav'] == 'slider') {
      $this->options['additional_classes'] .= ' slider';
    }

    if (isset($this->options['vocabulary'])) {
      $id = 'vocabulary:' . $this->options['vocabulary'];
    }
    else {
      $id = NULL;
    }
    $cid = $this->set_cache_id($id);

    cache_clear_all($cid, 'cache_os_boxes', TRUE);
    cache_clear_all('assets:' . $cid, 'cache_os_boxes', TRUE);

    // Only continues the user is selecting individual types.
    // For "All types" and "Determine for me", this value is overridden in
    // $this::render().
    if ($form_state['values']['behavior'] != 'select') {
      return;
    }

    // The user can select only one bundle at a time therefore when there is
    // multiple bundles we need to select the first one.
    if (is_array($this->options['bundles'])) {
      $this->options['bundles'] = reset($this->options['bundles']);
    }
  }

  /**
   * Implements boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !$vsite = vsite_get_vsite()) {
      return;
    }

    $block = parent::render();
    if (empty($this->options['vocabulary'])) {
      $this->options['vocabulary'] = current(array_keys(self::get_all_vocabularies()));

      if (empty($this->options['vocabulary'])){
        // There are no vocabularies defined in the VSite.
        return $block;
      }
    }

    ctools_include('layout', 'os');

    // Adding the current absolute current path for the os taxonomy JS file.
    // This would help us select the current terms we search for. This part
    // cannot be cached.
    $settings['fbt'] = array(
      'url' => url($_GET['q'], array('absolute' => TRUE))
    );
    $tid = str_replace('taxonomy/term/', '', os_taxonomy_in_taxonomy_term_context());
    if ($term = taxonomy_term_load($tid)) {
      $settings['fbt']['vid'] = $term->vid;
    }

    drupal_add_js($settings, array('type' => 'setting'));
    drupal_add_js(drupal_get_path('module', 'os_boxes') . '/plugins/os_taxonomy_fbt/os_taxonomy_fbt.js');

    if ($this->options['behavior'] == '--all--'  || ($this->options['behavior'] == 'contextual' && empty(os_get_active_feature()))) {
      $this->options['bundles'] = array();
    }
    else if (!empty($this->options['behavior']) && $this->options['behavior'] == 'contextual' && ($feature = os_get_active_feature())) {
      if (isset($feature->info['features']['node'])) {
        // Building a list of the bundles in two ways:
        // 1. Selected bundles from a list
        // 2. If the widget defined as a "Determine for me" set the bundles
        //    which related to the app.
        $paths = os_get_app_path();
        $menu = menu_get_item();

        $this->options['bundles'] = array();
        if ($menu['map'][0] == 'node') {
          $this->options['bundles'] = array($menu['map'][1]->type);
        }
        else {
          foreach ($paths as $bundle => $path) {
            if ($path == $menu['original_map'][0]) {
              $this->options['bundles'][] = $bundle;
            }
          }
        }
      }
    }
    else if ($this->options['behavior'] == 'select') {
      // For specified node type behavior but non-string value, default to all
      // and try to grab any first value of an array.
      $bundles = $this->options['bundles'];
      $this->options['bundles'] = FALSE;
      if (is_array($bundles)) {
        foreach ($bundles as $value) {
          if (!empty($value)) {
            $this->options['bundles'] = array($value);
            break;
          }
        }
      }
      else if (is_string($bundles)) {
        $this->options['bundles'] = array($bundles);
      }
    }

    // Check user access to the content of the widget.
    if (!empty($this->options['bundles']) && !$this->checkWidgetAccess(reset($this->options['bundles']))) {
      return $block;
    }

    // Content type's machine name is added in cache id.
    // So that different set of cached links can be generated per content type for 'Determine for me' settings.
    if ($this->options['behavior'] == 'contextual' && !empty(os_get_active_feature())) {
      $id = 'vocabulary:' . $this->options['vocabulary'] . ':' . implode(":", $this->options['bundles']);
    }
    else {
      $id = 'vocabulary:' . $this->options['vocabulary'];
    }

    // Two cases bring about the navigation CSS class:
    //    * Menu display checkbox is checked (in old form)
    //    * New 'menu' radio is selected in *Display type* (in new form)
    if (($this->options['as_nav'] === true) || ($this->options['as_nav'] === 'menu')) {
      $this->options['additional_classes'] = 'navigation';
    }

    $this->set_cache_id($id);
    if (($block['content'] = $this->get_cache()) === FALSE) {
      if (is_array($this->options['vocabulary'])) {
        $t = array_filter($this->options['vocabulary']);
        $this->options['vocabulary'] = current($t);
      }

      $content = theme('os_taxonomy_vocabulary_item_list', array(
        'vid' => $this->options['vocabulary'],
        'options' => $this->options,
      ));

      // When showing events set expire time of 5 minutes.
      $event_bundles = array(
        'event',
        'upcoming_event',
        'past_event',
      );
      if (is_array($this->options['bundles']) && in_array(reset($this->options['bundles']), $event_bundles)) {
        $this->cache_time = "5 minutes";
      }

      $block['content'] = $content;
      $this->set_cache($block['content']);
    }

    return $block;
  }

  /**
   * Get all the vocabularies id using the taxonomy terms.
   */
  private function get_all_vocabularies() {
    ctools_include('taxonomy', 'os');
    $taxonomy_vocabularies = os_get_vocabularies();
    $vocabularies = array();

    foreach ($taxonomy_vocabularies as $taxonomy_vocabulary) {
      $vocabularies[$taxonomy_vocabulary->vid] = $taxonomy_vocabulary->name;
    }

    return $vocabularies;
  }
}

  /**
  * AJAX callback to update Bundles set as per selected vocabulary.
  */
  function os_boxes_taxonomy_fbt_vocab_ajax_callback($form, &$form_state) {
    return $form['options']['bundles'];
  }
