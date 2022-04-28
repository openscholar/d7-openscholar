<?php

function cp_layout_full_form($form, $form_state){
  ctools_include('ajax');
  ctools_include('modal');
  ctools_modal_add_js();
  ctools_include('layout', 'os');

  $ctxs = array_slice(func_get_args(), 2);

  // Temporary: To be replaced by API function later
  $contexts = os_layout_get_contexts(array(1, 2));
  $ctxs = $ctxs ? $ctxs : array('os_front', 'os_public');

  // Sort the site content alphabetically in the site section dropdown.
  $content = array_splice($contexts, 2);
  asort($content);
  $contexts = array_merge($contexts, $content);

  $current = '';
  if ($ctxs == array('os_public')) {
    $current = 'os_public';
  }
  else {
    $current = current(array_diff($ctxs, array('os_public')));
  }

  $form = array(
    '#attributes' => array(
      'class' => $ctxs
    )
  );

  if (isset($contexts[$current])) {
    $form['context_selection'] = array(
      '#type' => 'select',
      '#title' => t('Site section:'),
      '#options' => $contexts,
      '#default_value' => $current,
      '#multiple' => false
    );
  }

  if (!in_array('os_public', $ctxs)) {
    $ctxs[] = 'os_public';
  }

  // take custom themes into account
  ctools_include('themes', 'os');
  $theme = os_theme_get();
  $layout_css = drupal_get_path('theme', $theme).'/cp-layout-map.css';
  // if the theme hasn't given us a layout map, give users the ability to choose their layout
  // only private themes should have a layout map
  if (!file_exists($layout_css)) {
    $form['layout_selection'] = array(
      '#type' => 'markup',
      '#markup' => ctools_modal_text_button(t('Change layout'), 'cp/build/layout/nojs/select', t('Change your layout'), 'cp-layout-change')
    );

    include_once(drupal_get_path('theme', 'adaptivetheme') . '/inc/get.inc');
    $layout_css = drupal_get_path('module', 'cp_layout') . '/theme/layouts/' . at_get_setting('bigscreen_layout', os_theme_get()) . '.css';
  }

  $blocks = os_layout_get_multiple($ctxs, true, true);

  $check_lock = !(user_access('edit locked regions') || user_access('manage locked regions'));
  $locked = variable_get('os_locked_regions');

  $form['context'] = array(
    '#type' => 'value',
    '#value' => $ctxs
  );

  $regions = array(
    'header_first',
    'header_second',
    'header_third',
    'menu_bar',
    'sidebar_first',
    'content',
    'sidebar_second',
    'content_top',
    'content_first',
    'content_second',
    'content_bottom',
    'footer_first',
    'footer',
    'footer_third',
    'footer_bottom',
  );

  $weights = _cp_layout_get_weight_map($regions);

  $form['unused'] = array(
    '#theme_wrappers' => array(
      'cp_layout_unused_widgets'
    ),
    '#tags' => os_boxes_get_tags(),
    '#factories' => os_boxes_get_factories()
  );

  $form['layout'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'cp_layout') . '/theme/cp_layout.js'
      ),
      'css' => array(
        drupal_get_path('module', 'cp_layout') . '/theme/cp_layout.common.css',
        $layout_css,
      )
    ),
    '#pre_render' => array(
      '_cp_layout_rearrange_content_regions'
    ),
    'unused' => array(
      'widgets' => array(
        '#type' => 'hidden',
        '#default_value' => ''
      )
    )
  );

  foreach ( $regions as $r ) {
    $form['layout'][$r] = array(
      '#type' => 'container',
      '#weight' => $weights[$r],
      '#attributes' => array(
        'class' => array(
          'cp-region',
          drupal_html_class('cp-region-' . $r),
          ($check_lock && isset($locked[$r]) && $locked[$r])?'locked-down':'unlocked',
        )
      ),
      'widgets' => array(
        '#type' => 'hidden',
        '#default_value' => ''
      )
    );
  }

  $use_content_regions = FALSE;
  foreach ( $blocks as $bid => $b ) {
    if ($b['region'] == - 1 || $b['region'] === FALSE) {
      $form['unused'][$bid] = array(
        '#theme' => 'cp_layout_widget',
        '#widget' => $b
      );
    }
    elseif (isset($form['layout'][$b['region']])) {
      if (strpos($b['region'], 'content_') !== FALSE) {
        $use_content_regions = TRUE;
      }
      $form['layout'][$b['region']][$bid] = array(
        '#theme' => 'cp_layout_widget',
        '#widget' => $b,
        '#weight' => $b['weight']
      );
    }
  }

  if (in_array('os_front', $ctxs)) {
    $use_content_regions = TRUE;
  }

  if ($use_content_regions) {
    $form['layout']['#attributes'] = array(
      'class' => array(
        'content-regions'
      )
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save')
    ),
  );

  $form['#submit'][] = 'cp_layout_full_submit';

  return $form;
}

function cp_layout_ajax(){
}

function _cp_layout_rearrange_content_regions($elements){
  foreach ( $elements as $r => $region ) {
  if (strpos($r, 'content_') !== FALSE) {
    unset($elements[$r]);
    $elements['content'][$r] = $region;
  }
  }

  return $elements;
}

function _cp_layout_get_weight_map($regions){
  $map = array_fill_keys($regions, 1);
  $i = 1;
  foreach ( $map as &$w ) {
  $w = $i ++;
  }
  // drupal doesn't make including files from themes easy
  $path = drupal_get_path('theme', 'adaptivetheme');
  require_once ($path . '/inc/get.inc');
  ctools_include('themes', 'os');

  // This is terrible
  switch (at_get_setting('bigscreen_layout', os_theme_get())) {
  case 'two_sidebars_left' :
    $map['sidebar_second'] = 5.5;
    break;
  case 'two_sidebars_right' :
    $map['sidebar_first'] = 6.5;
    break;
  }

  return $map;
}

function cp_layout_full_submit($form, &$form_state){

  // Standard form submit, save off the settings
  $regions = $form_state['values']['layout'];
  $ctxs = $form_state['values']['context'];
  $blocks = os_layout_get_multiple($ctxs, false, true);

  // go through our regions and change the blocks to match their new position
  // and weight
  foreach ( $regions as $r => $d ) {
    $widgets = explode('|', $d['widgets']);
    $weight = 0;
    if ($r == 'unused')
      $r = false;
    foreach ( $widgets as $w ) {
      if (isset($blocks[$w])) {
        $blocks[$w]['region'] = $r;
        $blocks[$w]['weight'] = $weight ++;
      }
    }
  }

  os_layout_set_multiple($ctxs, $blocks);
}

/**
 * Menu callback for ajax form
 */
function cp_layout_layout_select_ajax($ajax){
  if ($ajax) {
    ctools_include('modal');
    $form_state = array(
      'ajax' => true
    );

    $output = ctools_modal_form_wrapper('cp_layout_layout_select', $form_state);
    if ($form_state['executed']) {
      $output[] = ctools_modal_command_dismiss();
      if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Save')) {
      ctools_include('ajax');
      $output[] = ctools_ajax_command_reload();
      }
    }
    echo ajax_render($output);
    exit();
  }
  else {
    return drupal_get_form('cp_layout_layout_select');
  }
}

/**
 * Form for selecting which layout type the user wants for their site
 */
function cp_layout_layout_select($form, $form_state){
  $form = array();
  ctools_include('themes', 'os');
  $theme_name = os_theme_get();
  $options = _cp_layout_get_layout_options($theme_name);
  $form['layout'] = array(
    '#type' => 'radios',
    '#options' => array_combine($options, $options),
    '#default_value' => at_get_setting('bigscreen_layout', $theme_name),
    '#description' => t('Click a layout to select which you would like your site to use. This selection will not affect the header of your site,
    only the area below it.'),
  );
  $form['#attached']['css'] = array(drupal_get_path('module', 'cp_layout').'/theme/cp_layout_select.css');

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save')
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel')
    )
  );

  $form['#submit'] = array(
    'cp_layout_select_submit'
  );

  return $form;
}

function cp_layout_select_submit($form, $form_state){
  // initialize some variables the adaptive theme inc file will need
  if ($form_state['values']['op'] != t('Save'))
  return;

  // Helper functions for processing the page layout and font families.
  $path_to_at_core = drupal_get_path('theme', 'adaptivetheme');
  require_once ($path_to_at_core . '/inc/forms/at_core.submit.builders.inc');

  $theme_name = os_theme_get();
  $path = variable_get('theme_' . $theme_name . '_files_directory', '');
  if (module_exists('vsite') && $space = spaces_get_space()) {
    $path = 'public://' . $space->og->purl . '/css';
    $space->controllers->variable->set('theme_' . $theme_name . '_files_directory', $path);
  }

  file_prepare_directory($path, FILE_CREATE_DIRECTORY);
  $path_to_responsive_css = drupal_get_path('theme', $theme_name) . '/css/';

  $settings = variable_get('theme_' . $theme_name . '_settings', array());
  $settings['bigscreen_layout'] = $form_state['values']['layout'];
  $settings['global_default_layout'] = 'standard-layout';

  if (module_exists('vsite') && $space = spaces_get_space()) {
    $space->controllers->variable->set('theme_' . $theme_name . '_settings', $settings);
  }
  else {
    variable_set('theme_' . $theme_name . '_settings', $settings);
  }

  /*
   * responsive.inc needs several keys in the values array filled These are
   * required or we will get notices global_default_layout
   * enable_cascading_media_queries smartphone_portrait_media_query
   * smartphone_landscape_layout tablet_portrait_layout tablet_landscape_layout
   * bigscreen_layout disable_responsive_styles load_ltie18css load_respondjs
   * These are optional and I don't know what they do enable_extensions
   * enable_markup_overides global_gutter_width In addition, the following
   * prefixes exist for other variables smartphone_portrait smartphone_landscape
   * tablet_portrait tablet_layout bigscreen They can be combined with the
   * following suffixes to create more keys in the values array layout
   * media_query page_width page_unit sidebar_first sidebar_second sidebar_unit
   */
  $values = array(
    // this can also be smartphone-portrait.
    // We're coming from a standard design first, so it should never be
    // smartphone
    'global_default_layout' => 'standard-layout',
    // no reason not to to be enabled
    'enable_cascading_media_queries' => true,
    // The following are strings that refer tooo... something
    'smartphone_portrait_media_query' => 'only screen and (max-width:320px)',
    'smartphone_landscape_layout' => 'one-col-vert',
    'tablet_portrait_layout' => 'one-col-vert',
    'tablet_landscape_layout' => $form_state['values']['layout'],
    'bigscreen_layout' => $form_state['values']['layout'],

    // most of these will be defaults we don't want users messing with them
    // I need to set them all or the files won't be generated correctly

    // bigscreen layout stuff
    'bigscreen_media_query' => 'only screen and (min-width:1025px)',
    'bigscreen_page_width' => 980,
    'bigscreen_page_unit' => 'px',
    'bigscreen_sidebar_first' => 200,
    'bigscreen_sidebar_second' => 290,
    'bigscreen_sidebar_unit' => 'px',

    // tablet landscape stuff
    'tablet_landscape_media_query' => 'only screen and (min-width:769px) and (max-width:1024px)',
    'tablet_landscape_page_width' => 100,
    'tablet_landscape_page_unit' => '%',
    'tablet_landscape_sidebar_first' => 20,
    'tablet_landscape_sidebar_second' => 20,
    'tablet_landscape_sidebar_unit' => '%',

    // tablet portrait stuff
    'tablet_portrait_media_query' => 'only screen and (min-width:481px) and (max-width:768px)',
    'tablet_portrait_page_width' => 100,
    'tablet_portrait_page_unit' => '%',
    'tablet_portrait_sidebar_first' => 50,
    'tablet_portrait_sidebar_second' => 50,
    'tablet_portrait_sidebar_unit' => '%',

    // smartphone landscape stuff
    'smartphone_landscape_media_query' => 'only screen and (min-width:321px) and (max-width:480px)',
    'smartphone_landscape_page_width' => 100,
    'smartphone_landscape_page_unit' => '%',
    'smartphone_landscape_sidebar_first' => 50,
    'smartphone_landscape_sidebar_second' => 50,
    'smartphone_landscape_sidebar_unit' => 'px',

    // smartphone portrait
    'smartphone_portrait_layout' => 'one_col_stack',
    'smartphone_portrait_page_width' => 100,
    'smartphone_portrait_page_unit' => '%',
    'smartphone_portrait_sidebar_first' => 100,
    'smartphone_portrait_sidebar_second' => 100,
    'smartphone_portrait_sidebar_unit' => '%'
  );

  // AT separates its submit handler into several inc files that are just
  // included as is
  // Now that I have properly formed data, I can just ship it off to that tile
  @require_once ($path_to_at_core . '/inc/forms/at_core.submit.responsive.inc');
}

function _cp_layout_get_layout_options($theme_name){
  require_once drupal_get_path('theme', 'adaptivetheme').'/inc/get.inc';
  require_once drupal_get_path('theme', 'adaptivetheme').'/inc/plugins.inc';

  // hack so at gets options for our theme
  global $theme_key;
  $theme_key_orig = $theme_key;
  $theme_key = $theme_name;

  // hack to make sure it doesn't get options for cp theme
  cache_clear_all('at_page_layouts_data_structure', 'cache');

  $layouts = responsive_page_layouts_device_group_options($theme_name);
  $theme_key = $theme_key_orig;

  return $layouts['bigscreen'];
}
