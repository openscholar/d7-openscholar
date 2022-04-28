<?php

/**
 * @file
 * Provides class for os_boxes_tabs.
 */

/**
 * Provides class for os_boxes_tabs.
 */
class os_boxes_tabs extends os_boxes_widget_collection {

  public $embed_as = array(
    'iframe',
  );

  // for backwards compatibility
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['tabs'] = array();
    return $options;
  }

  public function options_form(&$form_state) {
    // tabs widgets created before os_boxes_widget_collection was abstracted out will have keys that begin with tab-,
    // instead of the new widget-. They also had a bug where you could get something like tab-tab-tab-tab-1 as a key.
    // This replaces all occurrences of tab- with a single widget- in the key, so older tab widgets will slowly be
    // migrated over to the new format over time, as they're edited.
    if (!empty($this->options['tabs'])) {
      foreach ($this->options['tabs'] as $k => $v) {
        $k = str_replace('tab-', '', $k);
        if (strpos($k, 'widget-') === FALSE) {
          $id = 'widget-' . $k;
        }
        else {
          $id = $k;
        }

        $this->options['widgets'][$id] = $v;
      }
    }
    return parent::options_form($form_state);
  }

  /**
   * Implements render().
   */
  public function render() {
    $block = parent::render();
    // disable caching for this block for now
   // if (($block['content'] = $this->get_cache()) === FALSE) {
      $plugin = context_get_plugin('reaction', 'block');
      $blocks = $plugin->get_blocks();
      $render = array(
        '#prefix' => '<div class="os-boxes-tabbed">',
        '#suffix' => '</div>',
        'links' => array(
          '#theme' => 'links',
          '#links' => array(),
          '#attributes' => array(
            'class' => array('tab-links'),
          )
        ),
        '#attached' => array(
          'library' => array(
            array('system', 'ui.tabs'),
          ),
          'js' => array(
            drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_tabs/os_boxes_tabs_display.js',
          ),
        ),
      );

      if (!empty($this->options['tabs'])) {
        $this->options['widgets'] = $this->options['tabs'];
      }

      module_load_include('module', 'block', 'block');
      foreach ($this->options['widgets'] as $k => & $tab) {
        if ($block_tab = $blocks[$tab['bid']]) {
          $block_tab->region = $this->delta;
          if ($block_arr = _block_render_blocks(array($block_tab))) {
            if (!empty($block_arr["{$block_tab->module}_{$block_tab->delta}"]->assets)) {
              $sub_assets = $block_arr["{$block_tab->module}_{$block_tab->delta}"]->assets;
              foreach ($sub_assets as $type => $data) {
                if (!isset($render['#attached'][$type])) {
                  $render['#attached'][$type] = array();
                }
                $render['#attached'][$type] = array_merge($render['#attached'][$type], $data);
              }
            }

            if (!empty($block_arr["{$block_tab->module}_{$block_tab->delta}"]->content)) {
              // tabs widgets created before os_boxes_widget_collection was abstracted out will have keys that begin with tab-,
              // instead of the new widget-. They also had a bug where you could get something like tab-tab-tab-tab-1 as a key.
              // This replaces all occurrences of tab- with a single widget- in the key, so older tab widgets will slowly be
              // migrated over to the new format over time, as they're edited.
              $k = str_replace('tab-', '', $k);
              if (strpos($k, 'widget-') === FALSE) {
                $k = 'widget-'.$k;
              }

              $render['links']['#links'][$tab['weight']] = array(
                'title' => $tab['title'],
                'href' => '',
                'fragment' => $k,
                'external' => TRUE,
              );

              $build = _block_get_renderable_array($block_arr);
              $build['#attributes']['id'] = '';
              $render[$tab['bid']] = array(
                '#type' => 'container',
                '#attributes' => array('id' => $k),
                $tab['bid'] => $build,
              );
            }
          }
        }
      }


      ksort($render['links']['#links']);
      $block['content'] = drupal_render($render);
      $block['assets'] = $render['#attached'];
    //  $this->set_cache($block['content'], $render['#attached']);
    //}


    return $block;
  }
}
