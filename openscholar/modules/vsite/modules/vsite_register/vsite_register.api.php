<?php

/**
 * @file
 * Example implementations of hooks for Vsite Register.
 */

/**
 * Modify the success message displayed when vsite register form is submitted.
 */
function hook_vsite_register_message(&$message, $form, $domain) {
  // Calls it a profile, not a site.
  $message = str_replace('site', 'profile', $message);
}