<?php
// $Id$

/**
 * Provides edit form for an OS Widget.
 *
 * @param object $box
 * @param string $source
 * @return array $form
 */
function os_boxes_edit_widget($box, $source = '') {
  if (!$box) {
    // @todo Multi-step form to choosen plugin type?
    return MENU_NOT_FOUND;
  }

  $info = ctools_get_plugins('boxes', 'plugins', $box->plugin_key);

  if ($box->new) {
    drupal_set_title(t('Add New "@plugin" Widget', array('@plugin' => $info['title'])));
  }
  else {
    drupal_set_title(t('Edit "@plugin" Widget - !info', array('@plugin' => $info['title'], '!info' => $box->description)));
  }

  $form_state = array(
    'box' => $box,
    'source' => $source,
  );

  $form = drupal_build_form('boxes_box_form', $form_state);

  unset($form['submit']['#attributes']);
  unset($form['cancel']['#attributes']);

  return $form;
}

/**
 * Implements hook_form_FORM_ID_form_alter() for boxes_box
 */
function os_boxes_form_boxes_box_form_alter(&$form, &$form_state) {
  $form['boxes_adv']['#access'] = false;

  $form['description']['#title'] = t('Widget Description');
  $form['description']['#description'] = t('A brief description of the widget. Only displayed in the Layout section.');

  $form['title']['#title'] = t('Widget Title');
  $form['title']['#description'] = t('The title of the widget that is seen by your visitors.');

  $form['cancel']['#submit'][] = 'os_boxes_widget_submit';
  $form['#submit'][] = 'os_boxes_widget_submit';

  if (method_exists($form_state['box'], 'validate')) {
    $form['#validate'][] = 'os_boxes_alter_options_widget';
  }
}

/**
 * Allow widgets alter the values of the forms before process by boxes.
 */
function os_boxes_alter_options_widget($form, &$form_state) {
  $form_state['box']->validate($form_state['values']);
}

/**
 * Generates the HTML for widget and passes it as an AJAX command using
 * ctools overlay_commands
 */
function os_boxes_widget_submit($form, $form_state) {
  global $dummy_rendering;

  if ($form_state['values']['op'] !== t('Cancel')) {
    $box     = boxes_box_load($form_state['box']->delta);
    $html    = '';
    $target  = '';
    $op      = 'prepend';
    $new_box = isset($form_state['box']->new) && $form_state['box']->new;
    switch ($form_state['source']) {
      case 'cp-layout':
        $b = os_boxes_os_widget($box);
        drupal_alter('os_widget', $b);
        $r = array('#theme' => 'cp_layout_widget', '#widget' => $b);
        $html = drupal_render($r);
        if ($new_box) {
          $target = '.widget-container';
        }
        else {
          $target = '#boxes-' . $box->delta;
          $op = 'replace';
        }
        break;

      default:
        $dummy_rendering = TRUE;
        // Coming from public view can only edit boxes from this view.
        module_load_include('module', 'block', 'block');
        $b     = (object)os_boxes_os_widget($box);
        $t     = _block_render_blocks(array($b));
        $block = array_shift($t);
        $dummy_rendering = FALSE;

        // Do this or else notices will throw. absolutely nothing is done with
        // it but whatever.
        $block->region = 'dummy';
        $block->module = '';
        $block->delta = '';
        $block->subject = '';
        $array         = array('#theme' => 'block', '#block' => $block);
        $html          = drupal_render($array);
        $target        = '#boxes-box-' . $box->delta;
        $op            = 'replace';
    }

    $command = 'ajax_command_' . $op;
    $command = $command($target, $html);

    overlay_commands_add($command);
  }
  ctools_include('modal');
  // overlay_close_dialog doesn't work because the form resets it
  overlay_commands_add(ctools_modal_command_dismiss());
}

/**
 * Provides form to delete a widget.
 */
function os_boxes_delete_widget($box) {
  $form_state = array(
    'box' => $box,
  );

  drupal_set_title(t('Delete !info', array('!info' => $box->description)));

  $form = drupal_build_form('os_boxes_delete_widget_form', $form_state);

  return $form;
}

/**
 * Creates a form array for deleting a Box widget.
 *
 * @see os_boxes_delete_widget()
 */
function os_boxes_delete_widget_form($form_id, $form_state) {
  $box = $form_state['box'];
  $form = array();

  $form['bid'] = array(
    '#type' => 'value',
    '#value' => $box->delta,
  );

  $form['warning'] = array(
    '#markup' => t('Are you sure you want to delete this widget? This will delete the widget and remove it from all pages
    it is currently placed on. This action is cannot be undone.'),
  );
  $form['actions'] = array(
    '#type' => 'actions',
    'yes' => array(
      '#type' => 'submit',
      '#value' => t('Yes'),
    ),
    'no' => array(
      '#type' => 'submit',
      '#value' => t('No'),
    ),
  );

  $form['#submit'][] = 'os_boxes_widget_delete_submit';

  return $form;
}

/**
 * Submit callback; deletes widget.
 *
 * @see os_boxes_delete_widget_form()
 */
function os_boxes_widget_delete_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Yes')) {
    // Removes box from contexts
    ctools_include('layout', 'os');

    // Deletes box instance
    if (module_exists('spaces') && $space = spaces_get_space()) {
      $space->controllers->boxes->del($form_state['values']['bid']);
      os_layout_block_delete('boxes-' . $form_state['values']['bid'], NULL, $space);
    }
    else {
      os_layout_block_delete('boxes-' . $form_state['values']['bid']);
      if ($box = boxes_box_load($form_state['values']['bid'])) {
        $box->delete();
      }
      module_load_include('inc', 'block', 'block.admin');
      block_custom_block_delete_submit($form, $form_state);
    }
    unset($form_state['redirect']);
    $form_state['no_redirect'] = TRUE;

    overlay_commands_add(ajax_command_remove('#boxes-' . $form_state['values']['bid']));
  }
  overlay_commands_add(overlay_command_dismiss());
}

/**
 * Handles removing a widget from a theme region, provides undo link.
 */
function os_boxes_remove_widget($module, $delta, $region = FALSE) {
  $contexts = $_GET['contexts'];

  // Removes the box from the contexts.
  ctools_include('layout', 'os');
  $blocks = os_layout_get_multiple($contexts, false, true);

  $bid = $module . '-' . $delta;

  $regions = system_region_list(variable_get('theme_default'));

  if (isset($regions[$region]) || $region === FALSE) {
    $blocks[$bid]['region'] = $region;
    os_layout_set_multiple($contexts, $blocks);
  }
}

/**
 * Returns an embeddable version of a box
 * @param $box - a full box object
 * @param $embed_style - how the box should be embedded in the client
 *    can be inline or iframe
 */
function os_boxes_embed_code($box, $embed_style) {
  module_invoke_all('suppress');
  if ($box) {
    module_load_include('module', 'block', 'block');
    $plugin = context_get_plugin('reaction', 'block');
    $blocks = $plugin->get_blocks();
    $bid = 'boxes-'.$box->delta;
    if ($b = $blocks[$bid]) {
      web_widgets_inside_widget(true);
      $b->region = 'embed';
      if ($b_out = _block_render_blocks(array($b))) {
        $block_arr = _block_get_renderable_array($b_out);
        $block = $block_arr['boxes_'.$box->delta];
        $block['#attached']['js'] = array(
          drupal_get_path('module', 'os_boxes').'/misc/os_boxes.resize_child.js',
        );

        print web_widgets_render_widget($embed_style, drupal_render($block), '');
      }
    }
  }
  print '';
  exit;
}

/**
 * Placeholder function. Always returns a 404.
 */
function os_boxes_placeholder() {
  return MENU_NOT_FOUND;
}
