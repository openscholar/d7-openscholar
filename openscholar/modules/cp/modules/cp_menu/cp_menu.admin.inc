<?php 

function cp_menu_admin() {
  $form = array(
    'cp_menu_link_options_new_window' => array(
      '#title' => t('New Window'),
      '#description' => t('Allow users to create links using in new windows.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('cp_menu_link_options_new_window', FALSE),
    ),
    'cp_menu_link_options_tooltip' => array(
      '#title' => t('Tooltips'),
      '#description' => t('Allow users to create links with custom tooltips.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('cp_menu_link_options_tooltip', FALSE),
    ),
  );

  return system_settings_form($form);
}
