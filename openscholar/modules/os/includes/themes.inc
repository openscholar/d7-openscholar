<?php

/**
 * Theme api stuff. Get/set/list themes etc.
 */

/**
 * Returns all OS themes for current privacy settings.
 */

function os_get_themes($theme_name = NULL, $theme_type = 'vsite', $access_filter = TRUE) {
  $allthemes = list_themes();
  _os_get_theme_type($theme_type);

  $themes = (array_filter($allthemes, '_os_is_os_theme'));

  if ($access_filter) {
    $themes = (array_filter($themes, '_os_acess_os_theme'));
  }

  if ($theme_name) {
    if (isset($themes[$theme_name])) {
      return $themes[$theme_name];
    }
  } else {
    return $themes;
  }
}

/**
 * Filters keeps only OS themes.
 */
function _os_is_os_theme($theme) {

  return (array_key_exists('os',$theme->info)
     && array_key_exists('theme_type', $theme->info['os'])
     && _os_get_theme_type() == $theme->info['os']['theme_type']
     && !empty($theme->status));
}

/**
 * Filters private themes you don't have access to.
 */
function _os_acess_os_theme($theme) {
  $space = spaces_get_space();
  $site_key = ($space) ? $space->group->purl : false;

  return ((!isset($theme->info['os']['theme_access']) || $theme->info['os']['theme_access'] != "private") || user_access('access private scholar themes') || $site_key == strtolower($theme->name));
}

/**
 * Returns the theme type, optionally reseting to the passed value.
 *
 * @param string|bool $set_type
 *
 * @return string $theme_type
 */
function _os_get_theme_type($set_type = FALSE) {
  static $theme_type;
  if ($set_type) {
    $theme_type = $set_type;
  }
  return $theme_type;
}

/**
 * Gets flavors for theme
 */
function os_theme_get_flavors($theme = NULL) {
	ctools_include('plugins');
  $flavors = ctools_get_plugins('os', 'flavor');

  // Search through the flavors that the user uploaded.
  $sub_themes = cp_appearance_get_flavors();
  $flavors = array_merge($flavors, $sub_themes);

  if (!$theme) {
  	return $flavors;
  }

  // Get just the plugins for a specific theme.
  static $theme_plugins;
  if (!isset($theme_plugins)) {
  	foreach ($flavors as $key => $info) {
  	  $theme_plugins[$info['module']][$key] = $info;
  	}
  }

  if (!is_array($theme_plugins)) {
    return array();
  }

  return (array_key_exists($theme, $theme_plugins)) ? $theme_plugins[$theme] : array();
}

function os_theme_set($theme) {
  if ($space = spaces_get_space()) {
    return $space->controllers->variable->set('theme_default', $theme);
  }
  else {
    return variable_set('theme_default', $theme);
  }
}

function os_theme_get() {
  if ($space = spaces_get_space()) {
    return $space->controllers->variable->get('theme_default');
  }
  else {
    return variable_get('theme_default', '');
  }
}

/**
 * gets the flavor of the currently selected theme
 */
function os_theme_get_flavor() {
  global $theme_key;
  $themes = os_get_themes(NULL, 'vsite', FALSE);
  $flavors = os_theme_get_flavors($theme_key);

  if (!array_key_exists($theme_key, $themes)) {
    return array();
  }

  $flavor_key = 'os_appearance_' . $theme_key . '_flavor';

  if ($space = spaces_get_space()) {
    $flavor = $space->controllers->variable->get($flavor_key);
  }
  else {
    $flavor = variable_get($flavor_key, FALSE);
  }

  if ($flavor && $flavor != 'default') {
    return array($flavor => $flavors[$flavor]);
  }
  else {
    return array();
  }
}
