<?php

class sweaver_layout extends sweaver_plugin {

  public function sweaver_menu(&$weight, $page_arguments, $base) {
    $items = array();

    // Theme classes groups administration.
    $base['access arguments'] = array('use editor');
    $items['sweaver_layout'] = $base + array(
      'title' => 'Widgets',
      'page arguments' => array($page_arguments),
      'type' => MENU_CALLBACK,
      'weight' => $weight++,
    );

    return $items;
  }

  public function sweaver_form() {
    $form = array();

    // get the list of widgets
    $ctx_blocks = context_get_plugin('reaction', 'block');
    $blocks = $ctx_blocks->get_blocks();
    dpm(array());    // This needs to be before _block_get_renderable array or krumo doesn't work properly

    // subtract the ones that are already in the active contexts
    $this->filter_blocks($blocks);

    // allow modules to modify or remove blocks
    drupal_alter('layout_available_blocks', $blocks);

    // make the factory list

    // setup tabs

    // render the widgets
    $rendered = _block_render_blocks($blocks);

    // remove blocks with no subject
    foreach ($rendered as $k => $v) {
      if (!isset($v->subject) || !$v->subject) {
        unset($rendered[$k]);
      }
    }

    $path = drupal_get_path('module', 'sweaver_layout');
    $form['widgets'] = array(
      '#type' => 'container',
      'tabs' => array(
        '#type' => 'markup',
        '#markup' => '<ul><li>Tabs</li></ul>',
      ),
      '#attached' => array(
        'css' => array(
          $path.'/theme/sweaver_layout.css',
        ),
        'js' => array(
          drupal_get_path('module', 'context_ui').'/jquery.pageEditor.js',
          $path.'/theme/sweaver_layout.js',
        ),
      ),
    );
    $form['widgets'] += _block_get_renderable_array($rendered);
    return $form;
  }

  /**
   * Filters out blocks that are used or unused
   * @param bool $used - set to true to filter out blocks that are unused in the context
   */
  private function filter_blocks(array &$blocks, $used = false) {
    foreach ($blocks as $k => $v) {
      if ($used && $v->region == -1) {
        unset($blocks[$k]);
      }
      elseif (!$used && $v->region != -1) {
        unset($blocks[$k]);
      }

      if (strpos($v->bid, 'boxes_add__')) {
        unset($blocks[$k]);
      }
    }
  }
}