<?php

function cp_theme_preprocess_page(&$vars) {
  $vars['breadcrumb'] = '';
}

function cp_theme_status_messages(&$vars) {
  $display = $vars['display'];
  $output = '';
  $allowed_html_elements = '<'. implode('><', variable_get('html_title_allowed_elements', array('em', 'sub', 'sup'))) . '>';

  $status_heading = array(
    'status' => t('Status update'),
    'error' => t('Error'),
    'warning' => t('Warning'),
  );
  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= '<div class="messages ' . $type . '"><div ng-non-bindable class="message-inner"><div class="message-wrapper">';
    if (!empty($status_heading[$type])) {
      $output .= '<h2>' . $status_heading[$type] . "</h2>";
    }
    if (count($messages) > 1) {
      $output .= " <ul>";
      foreach ($messages as $message) {
        if (strpos($message, 'Biblio') === 0 || strpos($message, 'Publication') === 0) {
          // Allow some tags in messages about a Biblio.
          $output .= '  <li ng-non-bindable>' . strip_tags(html_entity_decode($message), $allowed_html_elements) . "</li>";
        }
        else {
          $output .= '  <li ng-non-bindable>' . $message . "</li>";
        }
      }
      $output .= " </ul>";
    }
    elseif (strpos($messages[0], 'Biblio') === 0 || strpos($messages[0], 'Publication') === 0) {
      // Allow some tags in messages about a Biblio.
      $output .= strip_tags(html_entity_decode($messages[0]), $allowed_html_elements);
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div></div></div>";
  }
  return $output;
}