<?php

/**
 * @function theme_citation_distribute_plugin_theme
 *
 * Theme function wrapper activates plugin and calls its theme function.
 */
function theme_citation_distribute_plugin_theme($variables) {
  $plugin = $variables['plugin'];
  $service = _citation_distribute_load_plugin($plugin);
  if ($service && method_exists($service, 'render')) {
    return $service->render($variables);
  }
}