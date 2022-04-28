<?php

/**
 * Implements hook_cp_settings().
 */
function os_taxonomy_cp_settings() {
  $form = array();
  // Get active content types
  $active_content_types = os_get_bundles(array(1,2));
  $content_type_term_not_required = array('page' => 'Page',
    'software_release' => 'Software Release',
    'slideshow_slide' => 'Slideshow Image',
    'class_material' => 'Class Material',
    'feed_importer' => 'Feed importer',
  );
  $active_content_types = array_diff($active_content_types, $content_type_term_not_required);
  foreach($active_content_types as $content_types_key => $content_types_val) {
    ${'display_term_under_content_' . $content_types_key} = variable_get('os_taxonomy_display_term_under_content_' . $content_types_key, 0);
    ${'default_value_' . $content_types_key} = isset(${'display_term_under_content_' . $content_types_key}) ? ${'display_term_under_content_' . $content_types_key} : 0;
  }

  $template = array(
    'group' => array(
      '#id' => 'os_taxonomy',
      '#title' => t('Taxonomy'),
      '#weight' => 50,
      '#help_link' => l(t('Learn more about taxonomy'), 'https://help.theopenscholar.com/categorizing-your-content-taxonomy', array('attributes' => array('target' => '_blank')))
    ),
    'rest_after_submit' => array(
      'os_taxonomy_clear_caches'
    )
  );

  $form['os_taxonomy_display_term_under_content'] = $template + array(
      'form' => array(
        '#type' => 'checkbox',
        '#prefix' => '<label class="os-taxonomy_label_heading settings-form-heading">Choose where "See Also" terms display</label>',
        '#title' => t('Under a page\'s main content area'),
        '#default_value' => variable_get('os_taxonomy_display_term_under_content', 1)
      )
    );

  foreach($active_content_types as $content_types_key => $content_types_val) {
    if ($content_types_val === reset($active_content_types)) {
      $active_content_types_prefix = '<label>Under these content types when displayed in a list:</label>';
    } else {
      $active_content_types_prefix = '';
    }
    $vname = 'os_taxonomy_display_term_under_content_' . $content_types_key;
    $form[$vname] = $template + array(
        'form' => array(
          '#prefix' => $active_content_types_prefix,
          '#type' => 'checkbox',
          '#title' => t($content_types_val),
          '#default_value' => variable_get($vname, 0),
        ),
      );
  }

  return $form;
}

/**
 * Submit callback for cp/settings form.
 *
 * Clears boxes cache if 'Display terms under content' checkbox settings is changed.
 *
 * @see os_taxonomy_cp_settings()
 */
function os_taxonomy_cp_settings_node_submit($form, &$form_state) {
  // Get the current vsite.
  $vsite = vsite_get_vsite();
  $current_term_settings = variable_get('os_taxonomy_display_term_under_content', 1);
  // Clearing os boxes cache if os_taxonomy_display_term_under_content field settings is changed.
  if (isset($form_state['values']['os_taxonomy_display_term_under_content']) && $vsite && ($current_term_settings != $form_state['values']['os_taxonomy_display_term_under_content'])) {
    cache_clear_all('os_boxes_cache:' . $vsite->id, 'cache_os_boxes', TRUE);
  }
  // Clearing os boxes cache if any of os_taxonomy_display_term_under_content content type field settings is changed.
  $active_content_types = os_get_bundles(array(1,2));
  foreach($active_content_types as $content_types_key => $content_types_val) {
    $current_term_settings = variable_get('os_taxonomy_display_term_under_content_' . $content_types_key, 1);
    if (isset($form_state['values']['os_taxonomy_display_term_under_content_' . $content_types_key]) && $vsite && ($current_term_settings != $form_state['values']['os_taxonomy_display_term_under_content_' . $content_types_key])) {
      cache_clear_all('os_boxes_cache:' . $vsite->id, 'cache_os_boxes', TRUE);
      break;
    }
  }
}