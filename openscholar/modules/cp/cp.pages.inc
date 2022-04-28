<?php

/**
 * @file
 * Provides page and form content for Control Panel module.
 */

/**
 * Form argument for menu item '/cp/build/apps/$app_name'.
 */
function cp_app_settings($js, $app_name) {

  if (!$js) {
    return drupal_get_form('cp_app_settings_form', $app_name);
  }

  ctools_include('modal');
  ctools_include('ajax');
  $app = feature_load($app_name);
  $app_title = features_get_feature_title($app);

  $form_state = array(
    'title' => $app_title,
    'ajax' => TRUE,
    'build_info' => array(
      'args' => array($app_name),
    ),
  );

  $output = ctools_modal_form_wrapper('cp_app_settings_form', $form_state, $app_name, $app_title);
  // Check to make sure there are no form errors before dismissing this form modal.
  if ($form_state['submitted'] && form_get_errors() == FALSE) {
    $output[] = ctools_modal_command_dismiss();
  }
  else {
    $output[] = ajax_command_append('#modal-content div.buttons','<input class="form-submit close" onClick="$(\'div.modal-header a.close\').click(); return false;" value="Cancel" />');
    $output[] = ajax_command_after('#modal-content #edit-reset','<input class="form-submit close" onClick="$(\'div.modal-header a.close\').click(); return false;" value="Cancel" />');
  }

  echo ajax_render($output);
  exit;
}

/**
 * Form wrapper for all OS App settings pages.
 *
 * Any modules that want to alter all settings forms should alter this form.
 *
 * @see cp_app_settings()
 */
function cp_app_settings_form($form, &$form_state, $app_name) {
  $item = menu_get_item('features/' . $app_name);
  $form['#attributes'] = array(
    'class' => array(
      $app_name . '-customize-form',
    )
  );
  if ($item['path'] == 'features/' . $app_name) {
    $form_id = array_shift($item['page_arguments']);
    if ($file = $item['include_file']) {
      require_once(DRUPAL_ROOT . '/' . $file);
    }

    if (count($item['page_arguments'])) {
      $form_state['build_info']['args'] = $item['page_arguments'];
    }

    $form += drupal_retrieve_form($form_id, $form_state);
  }
  else {
    $form['no_form'] = array(
      '#prefix' => '<p>',
      '#markup' => t('This App has no settings.'),
      '#suffix' => '</p>',
    );
    $form['ok'] = array(
      '#type' => 'submit',
      '#value' => t('Ok'),
    );
    $form['#after_build'][] = '_cp_app_settings_remove_no_form';
  }

  // Stores the current feature in the form data
  $form['#feature'] = $app_name;

  $form['#submit'][] = 'cp_app_settings_form_submit';

  return $form;
}

/**
 * After build callback for form `app_settings_form`.
 *
 * Processes specifically-structured form elements with system_settings_form().
 */
function _cp_app_settings_remove_no_form($element, $form_state) {
  $keys = array('no_form', 'ok', 'form_build_id', 'form_token', 'form_id');
  $children = element_children($element);
  $new = array_diff($children, $keys);
  if (count($new) != 0) {
    unset($element['no_form'], $element['ok']);
    $element = system_settings_form($element);
  }

  return $element;
}

/**
 * Submit callback for form `app_settings_form`.
 *
 * @see cp_app_settings_form()
 */
function cp_app_settings_form_submit($form, &$form_state) {
  $mode = overlay_get_mode();
  if ($mode== 'child') {
    overlay_close_dialog();
    $form_state['no_redirect'] = TRUE;
    $form_state['redirect'] = FALSE;
  } elseif (($mode == 'parent') && $form_state['ajax']) {
    $var = ($form_state['values']['os_publications_note_in_teaser']) ? 'public' : 'private';
    $cmd[] = ctools_modal_command_dismiss();
    $cmd[] = ajax_command_replace('#edit-7 div.description>strong>em', "<em class='placeholder'>$var</em>");
    print ajax_render($cmd);
    exit;
  } 
}
