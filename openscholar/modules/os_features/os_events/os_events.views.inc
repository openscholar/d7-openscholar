<?php

/**
 * @file
 *
 * Creates calendar displays of Views results.
 */

/**
 * Implements hook_views_plugins_alter()
 */
function os_events_views_plugins_alter(&$plugins) {
  $plugins['style']['os_calendar_style'] = $plugins['style']['calendar_style'];
  $plugins['style']['os_calendar_style']['title'] = t('Os calendar');
  $plugins['style']['os_calendar_style']['handler'] = 'os_events_calendar_plugin_style';
  $plugins['style']['os_calendar_style']['path'] = drupal_get_path('module', 'os_events') . '/includes';
  $plugins['style']['os_calendar_style']['module'] = 'os_events';
  $plugins['style']['os_calendar_style']['file'] = 'os_events_calendar_plugin_style.inc';
  $plugins['style']['os_calendar_style']['name'] = 'os_calendar_style';
}
