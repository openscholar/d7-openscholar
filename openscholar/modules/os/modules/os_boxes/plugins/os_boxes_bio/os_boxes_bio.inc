<?php
// $Id$

/**
 * @file
 * Provides class for "Bio" type widgets.
 */

/**
 * Displays the Bio node if exists, provides display options and empty text.
 *
 * @see OS branch 2.0-RC1 ./sites/all/modules/os/modules/os_boxes/plugins/os_boxes_bio/os_boxes_bio.inc
 */
class os_boxes_bio extends os_boxes_default {

  /**
   * Implements boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['teaser'] = 'full';
    $options['show_title'] = 0;
    return $options;
  }

  /**
   * Implements boxes_content::options_form().
   */
  public function options_form(&$form_state) {

    $form = parent::options_form($form_state);

    $defaults = $this->options_defaults();

    $form['teaser'] = array(
      '#type' => 'radios',
      '#title' => 'Display Mode',
      '#default_value' => isset($this->options['teaser']) ? $this->options['teaser'] : $defaults['teaser'],
      '#options' => array('teaser' => 'Teaser Only', 'full' => 'Full Bio'),
      '#description' => 'Display your entire Bio in this widget, or just the teaser.',
    );

    $form['show_title'] = array(
      '#type' => 'checkbox',
      '#title' => 'Display title of your Bio',
      '#default_value' => isset($this->options['show_title']) ? $this->options['show_title'] : $defaults['show_title'],
    );

    return $form;
  }

  /**
   * Implements boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }
    // Prepares $block array with 'title', 'subject', 'delta' & empty 'content'.
    $block = parent::render();

    // Returns the bio node object, or FALSE if none.
    $node = os_biocv_get_bio_node();

    if ($node === FALSE) {
      $block['content'] = $this->render_no_bio();
    }
    else {
      $block['content'] = $this->render_bio($node->nid);
    }
    return $block;
  }

  /**
   * Renders the bio node according to selected options.
   */
  protected function render_bio($nid) {
    // Prepares node with selected view_mode and show_title options.
    $view_mode = ($this->options['teaser'] == 'teaser') ? 'teaser' : 'full';
    $node = node_load($nid);
    $node->show_title = $this->options['show_title'];
    $render = node_view($node, $view_mode);

    $content = render($render);
    return $content;
  }
  /**
   * Renders empty text for a vsite admin without a Bio node.
   */
  protected function render_no_bio() {

    $spaces_features = variable_get('spaces_features', array());

    // If the bio is disabled show nothing
    if (!isset($spaces_features["os_biocv"])) {
      return '';
    }
    // Shows nothing if a vsite can be loaded but this user isn't admin.
    if (function_exists('vsite_get_vsite') && $vsite = vsite_get_vsite()) {
      if (!$vsite->access_admin()) {
        return '';
      }
    }
    elseif (!node_access('create','bio')) {
      return '';
    }
    // Otherwise, displays vsite admin empty text, prompting bio configuration.
    $wrapper = array(
      '#type' => 'container',
      '#attributes' => array(
        'id' => array('os-boxes-bio-empty-text'),
      ),
    );
    $create_bio  = l(t('Create your bio now'), 'node/add/bio');
    $edit_layout = l(t('change what displays'), 'cp/build/layout', array('query' => array('page_type' => 'front')));
    $wrapper['empty_text'] = array(
      '#type' => 'markup',
      '#markup' => t("Your site's front page is set to display your bio by default. $create_bio or $edit_layout for the front page of your site."),
    );

    return render($wrapper);
  }

  /**
   * Returns the cache parameters for this box
   */
  public function cache_info() {

    // Clears when bio nodes are saved
    $cache_key = array(
      'os_og' => array('current' => 'current'),
      'node' => array('bio' => 'bio'),
    );

    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);
  }
}

