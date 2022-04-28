<?php

/**
 * Returns HTML for a menu link and submenu.
 */
function hwpi_vibrant_menu_link(array $vars) {
  $element = $vars['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  if (!empty($element['#original_link'])) {
    if (!empty($element['#original_link']['depth'])) {
      $element['#attributes']['class'][] = 'menu-depth-' . $element['#original_link']['depth'];
    }
    if (!empty($element['#original_link']['mlid'])) {
      $element['#attributes']['class'][] = 'menu-item-' . $element['#original_link']['mlid'];
    }
  }

  // Add span tags to support the extra background images in the main menu
  $element['#title'] = '<span>' . $element['#title'] . '</span>';
  $element['#localized_options']['html'] = TRUE;

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>";
}


/**
 * Preprocess variables for page.tpl.php
 */
function hwpi_vibrant_preprocess_page(&$vars) {
  // Set a page class if a slideshow is enabled in the content top region
  if (isset($vars['page']['content_top'])) {
    if ($vars['page']['content_top']['boxes_%d']['#block']['boxes_plugin'] = 'os_boxes_slideshow') {
      $vars['classes_array'][] = 'content-top-slideshow-enabled';
    }
  }
}


/**
 * Preprocess variables for block.tpl.php
 */
function hwpi_vibrant_preprocess_block(&$vars) {
  if (isset($vars['block']->subject)) {
    if (!empty($vars['block']->subject)) {
      $vars['classes_array'][] = 'with-title';
    }
  }
}























