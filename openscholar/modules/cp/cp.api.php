<?php
/**
 * Allows modules to define a setting form to be included in the settings page for a site.
 * Returned settings can have the following keys:
 *   * form: (REQUIRED)
 *         A standard form array. Should handle itself entirely.
 *   * group: (optional)
 *         A standard form array. A container that multiple settings can go in.
 *         The #id field is required. All groups are made into fieldsets.
 *   * submit: (optional)
 *         An array of function names. If the form element is more than a simple variable,
 *         and needs its own submit handler, it goes here.
 */
function hook_cp_settings() {}

/**
 * Allows modules to alter setting forms from other modules before they're added to the settings form
 * All data should be in the same structure as above.
 */
function hook_cp_settings_alter(&$settings, &$form_state) {}
