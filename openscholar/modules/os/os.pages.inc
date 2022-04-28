<?php

/**
 * Default front page, stub
 */
function os_frontpage(){

  return " ";
}

/**
 * 404 page
 */
function os_page_not_found_page() {
  $t = '<strong>' . t('This page can not be found.') . '</strong>';
  $t = t('The URL might be incorrect or have changed.  Please go back to the !link',
      array('!link' => l('homepage', '<front>')));
  return $t;
}

/**
 * User Page (replacement for core /user)
 *
 * This adds redirection if you click a stale login link
 */
/**
 * Access callback for path /user.
 *
 * Displays user profile if user is logged in, or login form for anonymous
 * users.
 */
function os_user_page() {
  // If site is in read-only mode display a message for users.
  if (variable_get('os_readonly_mode', FALSE)) {
    return variable_get('os_readonly_mode_message', OS_READONLY_MODE_MESSAGE);
  }

  global $user;
  module_load_include('pages.inc', 'user');

  if (user_is_anonymous()) {
    // Display login page.
    return os_in_readonly_mode('login');
  }

  if (isset($_GET['destination']) && $_GET['destination'] != 'user') {
    // Redirect to destination.
    drupal_goto();
  }

  // Display user page.
  menu_set_active_item('user/' . $user->uid);
  return menu_execute_active_handler(NULL, FALSE);
}
