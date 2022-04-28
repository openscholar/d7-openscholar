<?php

/**
 * @file
 * The base class for OS custom widgets. It defines an "abstract box"
 * to be extended by  "real boxes" like "listings", "feeds", "images" etc.
 *
 * It provides the widget visibility optoins. Users will decide where the
 * widget will be shown (i.e. in the current section / feature or everywhere in
 * the site
 *
 * One idea would be to even remove the widget visiblity options from here and
 * let the higher layers to deal with it
 */


abstract class os_boxes_default extends boxes_box {

  /**
   * If this box has been overridden the changed timestamp will be held here
   */
  public $changed;

  /**
   * Holds the array of contexts that this item is in.
   */
  protected $os_contexts;

  /**
   * Holds the cache id.
   */
  protected $cache_id;

  /**
   * Holds the box assets.
   */
  protected $existing_assets = array('js' => array(), 'css' => array());

  /**
   * Holds the unix time stamp until when the plugin suppose to be cached.
   *
   * The value can be a string which the function strtotime accept i.e 5 minutes
   * from now or a unix time. When set to NULL the caching will be kept until:
   *  - User created/updated or deleted a content.
   *  - Update the plugin settings
   *  - The cache was clear.
   */
  protected $cache_time = NULL;

  /**
   * Given an embed type, return a label
   */
  public function embed_label($type) {
    $types = &drupal_static(__FUNCTION__, array(
      'inline' => t('HTML5'),
      'iframe' => t('Iframe'),
      'oembed' => t('URL')
    ));

    if (isset($types[$type])) {
      return $types[$type];
    }
    return '';
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    return array(
      'make_embeddable' => false,
      'additional_class' => '',
    );
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = array();

    $form['make_embeddable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make Embeddable'),
      '#description' => t('Allow this widget to be embedded on other websites.'),
      '#default_value' => (isset($this->options['make_embeddable'])) ? $this->options['make_embeddable'] : FALSE,
      '#access' => !empty($this->embed_as),
      '#weight' => 9001,
    );
    // The below widgets will have the settings for custom class addition is outermost wrapper div/section tag.
    $additional_class_allowed_plugins = array(
      'os_sv_list_box',
      'os_sv_list_file',
      'os_boxes_manual_list',
      'os_boxes_tabs',
      'os_boxes_accordion',
      'os_boxes_media',
      'os_boxes_rand_widget',
      'os_boxes_widget_slider',
      'os_boxes_html'
    );

    if (in_array($this->plugin_key, $additional_class_allowed_plugins) && module_exists('vsite') && $vsite = vsite_get_vsite()) {
      $form['additional_class'] = array(
        '#type' => 'textfield',
        '#title' => t('Apply css class name to this widget\'s container'),
        '#description' => t('Warning: this is to be used for custom themeing purposes only.'),
        '#default_value' => (isset($this->options['additional_class'])) ? $this->options['additional_class'] : '',
        '#access' => og_user_access('node', $vsite->group->nid, 'add widget custom class', NULL, FALSE, TRUE),
        '#weight' => 9002,
      );
    }

    $form['advanced'] = array(
      '#type' => 'fieldset',
      '#access' => user_access('use boxes advanced settings'),
      '#title' => t('Advanced Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 1000,
      '#after_build' => array('_os_boxes_default_advanced_handler'),
    );

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    // Default cache id set with default (NULL) bundle.
    $this->set_cache_id();

    $title = isset($this->title) ? $this->title : NULL;
    $block['title'] = $title;
    $block['delta'] = $this->delta;
    $block['content'] = '';
    if (!empty($this->options['make_embeddable'])) {
      $block['embed_link'] = true;
    }
    if (!empty($this->options['additional_class'])) {
      $block['additional_class'] = $this->options['additional_class'];
    }
    // Get the assets from the cache.
    $block['assets'] = $this->get_assets();
    return $block;
  }

  /**
   * Allow widgets to validate the form before processed by boxes.
   *
   * @param $values
   *   The form values.
   */
  public function validate(&$values) {}

  /**
   * Get the caching of the box.
   *
   * @param $bundle
   *  The bundle of the current widget.
   * @param $page
   *  Passing the page number when we handling a pager.
   *
   * @return String
   *  The cache ID string.
   */
  protected function set_cache_id($bundle = NULL, $page = NULL) {
    if (module_exists('vsite') && !$vsite = vsite_get_vsite()) {
      return;
    }

    $this->cache_id = 'os_boxes_cache:' . $vsite->id . ':' . $this->delta;

    if ($bundle && is_string($bundle)) {
      $this->cache_id .= ':' . $bundle;
    }

    if ($page) {
      $this->cache_id .= ':' . $page;
    }

    return $this->cache_id;
  }

  /**
   * Retrieve the cache.
   *
   * If we get a cache hit, put the original assets that we stored earlier back into place
   *
   * @return
   *   The cached data, or FALSE if no cache exists or is expired.
   *   NULL is a valid return value for an empty box.
   */
  protected function get_cache() {
    if (!variable_get('os_boxes_cache_enabled', TRUE)) {
      return FALSE;
    }

    if (empty($this->cache_id)) {
      // No valid cache id.
      return FALSE;
    }

    // wipe out the static arrays so we can get an accurate record of what assets this box adds
    $js = &drupal_static('drupal_add_js');
    $css = &drupal_static('drupal_add_css');

    $this->existing_assets = array(
      'js' => $js,
      'css' => $css,
    );

    $js = array();
    $css = array();
    $uid = $GLOBALS['user']->uid;
    if (!$cache = cache_get($this->cache_id . ':' . $uid, 'cache_os_boxes')) {
      // No cache exists.
      return FALSE;
    }

    if ($cache->expire != CACHE_PERMANENT && $cache->expire < REQUEST_TIME) {
      // Cache object was expired.
      return FALSE;
    }

    // Cache Hit
    // This box isn't going to add any assets, so we should restore what was already in the static arrays
    $js = $this->existing_assets['js'];
    $css = $this->existing_assets['css'];

    if ($assets = $this->get_assets()) {
      $this->add_assets($assets);
    }

    // For Admin users, add indicator that the returned block came from cache.
    if ($cache->data && user_access('administer openscholar')) {
      if (is_string($cache->data)) {
        $cache->data .= "<!--From Cache CID[{$this->cache_id}]-->";
      }
      else if (is_array($cache->data) && is_string($cache->data['content'])) {
        $cache->data['content'] .= "<!--From Cache CID[{$this->cache_id}]-->";
      }
    } elseif ($cache->data === FALSE) {
      // In the case where we have a valid cache hit but the contents of the box are FALSE,
      // we should return empty string instead.  FALSE will be treated as a cache miss.
      $cache->data = '';
    }

    drupal_add_http_header('x-drupal-cache-os-boxes-plugin', $this->plugin_key, TRUE);
    drupal_add_http_header('x-drupal-cache-os-boxes-cache-id', $this->cache_id . ':'. $uid, TRUE);

    return $cache->data;
  }

  /**
   * Helper function for retrieve the assets easily.
   */
  protected function get_assets() {
    if (!variable_get('os_boxes_cache_enabled', TRUE)) {
      return array();
    }

    if ($assets_cache = cache_get('assets:' . $this->cache_id.':'.$GLOBALS['user']->uid, 'cache_os_boxes')) {
      $assets = unserialize($assets_cache->data);
      return $assets;
    }
  }

  /**
   * Adding the cached assets.
   *
   * @param $assets
   *  Lists of assets.
   */
  protected function add_assets($assets) {
    foreach ($assets as $key => $asset) {
      switch ($key) {
        case 'library' :
          foreach ($asset as $library) {
            drupal_add_library($library[0], $library[1]);
          }
          break;
        case 'js' :
          foreach ($asset as $js) {
            if (!is_array($js)) {
              drupal_add_js($js);
            }
            else {
              drupal_add_js($js['data'], $js);
            }
          }
          break;
        case 'settings' :
          foreach ($asset as $setting) {
            drupal_add_js($setting, 'setting');
          }
          break;
        case 'css' :
          foreach ($asset as $css) {
            if (!is_array($css)) {
              drupal_add_css($css);
            }
            else {
              drupal_add_css($css['data'], $css);
            }
          }
          break;
        default:
          drupal_set_message(t('Invalid cache assets key "%key"', array('%key' => $key)), 'error');
      }
    }
  }

  /**
   * Set the cache for the next time.
   *
   * @param $cache
   *  The information we want to cache.
   * @param $assets
   *  The assets of the box. Optional.
   */
  protected function set_cache($cache, $assets = NULL) {
    if (!variable_get('os_boxes_cache_enabled', TRUE)) {
      return FALSE;
    }

    // Holds boolean whether rendering is done as dummy. Needs in order to avoid
    // caching of dummy rendering.
    // @todo: remove the dummy rendering fully.
    global $dummy_rendering;
    if ($dummy_rendering) {
      return;
    }

    if (empty($this->cache_id)) {
      // No valid cache id.
      return;
    }

    // add the current user id to the cache id
    $cid = $this->cache_id . ':' . $GLOBALS['user']->uid;

    if (!empty($this->cache_time)) {
      $expire = is_string($this->cache_time) ? strtotime($this->cache_time) : $this->cache_time;
    }
    else {
      $expire = CACHE_PERMANENT;
    }

    // the block should have rendered everything out into an html string, and attached any js or css it needed.
    // earlier, we blanked out the static arrays that hold these assets for all of Drupal
    // what's in those arrays now should only contain when this block added,
    $jquery = drupal_get_library('system', 'jquery');
    $once = drupal_get_library('system', 'jquery.once');
    $new_assets = array(
      'js' => array(),
      'css' => array(),
      'settings' => array(),
    );

    $static_js = &drupal_static('drupal_add_js');
    // if this is empty, the block didn't add any js, and we don't need to do anything
    if (!empty($static_js)) {
      $common_js = array(
        'settings' => array(),
        'misc/drupal.js' => array(),
      );
      $common_js += $jquery['js'];
      $common_js += $once['js'];
      $new_assets['js'] = array_diff_key($static_js, $common_js);
      $new_assets['settings'] = $static_js['settings']['data'];
    }

    $static_css = &drupal_static('drupal_add_css');
    if (!empty($static_css)) {
      $common_css = array();
      $common_css += $jquery['css'];
      $common_css += $once['css'];
      $new_assets['css'] = array_diff_key($static_css, $common_css);
    }

    // now we need to merge the new assets with what we stored earlier)
    $static_js = array_merge($this->existing_assets['js'], $new_assets['js']);
    $static_css = array_merge($this->existing_assets['css'],$new_assets['css']);
    // any box which within another box won't have any settings come in through existing_assets
    if (isset($this->existing_assets['js']['settings'])) {
      $static_js['settings']['data'] = array_merge($this->existing_assets['js']['settings']['data'], $new_assets['settings']);
    }
    else {
      $static_js['settings']['data'] = $new_assets['settings'];
    }

    // cache everything we just found
    cache_set($cid, $cache, 'cache_os_boxes', $expire);
    $cache_id = 'assets:' . $cid;
    cache_set($cache_id, serialize($new_assets), 'cache_os_boxes', $expire);
  }

  /**
   * When submitting a box we need to invalidate here.
   */
  public function options_submit($form, $form_state) {
    if (module_exists('vsite') && !$vsite = vsite_get_vsite()) {
      return;
    }

    // We try to delete the cache of a newly selected bundle, leaving the old cached
    // info unused. If we will re-select the old bundle its cache will be cleared
    // next time.
    if (isset($this->options['content_type']) && is_string($this->options['content_type'])) {
      $bundle = 'node:' . $this->options['content_type'];
    }
    else {
      $bundle = NULL;
    }
    $cid = $this->set_cache_id($bundle);

    cache_clear_all($cid, 'cache_os_boxes', TRUE);
    cache_clear_all('assets:' . $cid, 'cache_os_boxes', TRUE);
  }

  /**
   * Get the contexts where this box is located.
   */
  public function get_contexts() {

    if(!isset($this->os_contexts) && module_exists('vsite_layout') && $this->delta) {
      // Get the relationship between blocks and contexts.
      $this->os_contexts = vsite_layout_get_block_contexts('boxes', $this->delta);
    }

    return $this->os_contexts;
  }

  /**
   * Check access to the the box.
   *
   * @param $bundle
   *  The bundle displayed in the widget.
   *
   * @return bool
   *  Returns TRUE if the user can view the widget's content.
   *  FALSE otherwise.
   */
  public function checkWidgetAccess($bundle) {
    $bundles = $this->getBundles();

    if (in_array($bundle, $bundles['public_bundles'])) {
      // Widget displays a single public bundle.
      return TRUE;
    }

    if (in_array($bundle, $bundles['private_bundles'])) {
      // Widget displays a single private bundle.
      return node_access('view', $bundle);
    }
    return TRUE;
  }

  /**
   * Get the associated bundles for an App type.
   *
   * @param string $app_type
   *  The App type to get the bundles for. Can be OS_PRIVATE_APP to get the
   *  private bundles or OS_PUBLIC_APP to get the public bundles. If empty, get
   *  all the bundles.
   *
   * @param string $entity_type
   *  Type of the entity for which to get bundles for.
   *
   * @return array
   *  An array keyed by the app type and valued with the matching bundles.
   */
  protected function getBundles($app_type = '', $entity_type = 'node') {
    if ($entity_type == 'node') {
      switch ($app_type) {
        case OS_PRIVATE_APP:
          $bundles = array_keys(os_get_bundles(array(OS_PRIVATE_APP)));
          break;
        case OS_PUBLIC_APP:
          $bundles = array_keys(os_get_bundles(array(OS_PUBLIC_APP)));
          break;
        default:
          $bundles['public_bundles'] = array_keys(os_get_bundles(array(OS_PUBLIC_APP)));
          $bundles['private_bundles'] = array_keys(os_get_bundles(array(OS_PRIVATE_APP)));
          break;
      }
    }
    elseif ($entity_type == 'file') {
      switch ($app_type) {
        case OS_PRIVATE_APP:
          $bundles = array();
          break;
        case OS_PUBLIC_APP:
          $file_bundles = os_files_get_bundles();
          $bundles = array_keys($file_bundles);
          break;
        default:
          $file_bundles = os_files_get_bundles();
          $bundles['public_bundles'] = array_keys($file_bundles);
          $bundles['private_bundles'] = array();
          break;
      }
    }
    else {
      $bundles = array();
    }

    return $bundles;
  }
}

function _os_boxes_default_advanced_handler($element, $form_state) {
  $children = element_children($element);
  if (empty($children)) {
    $element['#access'] = false;
  }

  return $element;
}
