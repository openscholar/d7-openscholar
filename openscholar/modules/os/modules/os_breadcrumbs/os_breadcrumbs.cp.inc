<?php

/**
 * Implements hook_cp_settings().
 */
function os_breadcrumbs_cp_settings() {
  $form = array();
  $form['os_breadcrumbs_show_breadcrumbs'] = array(
    'group' => array(
      '#id' => 'breadcrumbs',
      '#title' => t('Breadcrumbs'),
      '#weight' => 50,
      '#menu_parent' => 'appearance',
      '#help_link' => l(t('Learn more about breadcrumbs'), 'https://help.theopenscholar.com/enable-breadcrumbs', array('attributes' => array('target' => '_blank')))
    ),
    'form' => array(
      '#prefix' => '<p>'
        . t('A breadcrumb trail is a navigation aid used for web interfaces. It allows visitors to keep track of their locations within sections your site, displaying links above the main content region that lead back to the home page and to the parent pages of the current page.')
        . '</p>'
        . '<p>'
        . t('i.e. <strong><a name="breadcrumbs" href="#breadcrumbs">Home</a> / <a href="#breadcrumbs">About</a> / <a href="#breadcrumbs">Contact</a></strong>')
        . '</p>',
      '#type' => 'checkbox',
      '#title' => t('Show breadcrumbs on my site'),
      '#default_value' => variable_get('os_breadcrumbs_show_breadcrumbs', 1),
      '#help_link' => l(t('Learn more about breadcrumbs'), 'https://help.theopenscholar.com/enable-breadcrumbs', array('attributes' => array('target' => '_blank')))
    ),
  );

  return $form;
}