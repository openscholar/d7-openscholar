<?php

/**
 * @file
 * Helper functions for theming pager links.
 */

/**
 * Adds an HTML head link for rel='prev' or rel='next' for pager links.
 *
 * Note: Currently only adds at most one prev and one next link to a page, and
 * only adds links for the first pager links rendered on any given page load.
 *
 * @todo Somehow determine precedence for when multiple pagers appear on page.
 *
 * @see os_basetheme_process_pager_link()
 * @see hwpi_basetheme_process_pager_link()
 */
function _os_pager_add_html_head_link($variables) {
  // Only continues if we are processing a previous or next link.
  // Skips other pager link types like first, last, and numbered links.
  $text = $variables['text'];
  static $rel_prev = FALSE, $rel_next = FALSE;
  // Looks for both full pager link text and mini pager link text.
  $prev_values = array(t('‹'));
  $next_values = array(t('›'));
  $attributes = array();
  if (!$rel_prev && in_array($text, $prev_values)) {
    $rel_prev = TRUE;
    $attributes['rel'] = 'prev';
  }
  if (!$rel_next && $text == in_array($text, $next_values)) {
    $rel_next = TRUE;
    $attributes['rel'] = 'next';
  }
  if (!isset($attributes['rel'])) {
    return;
  }

  // Builds the pager query parameter array for the href attribute.
  $attributes['href'] = _os_pager_build_href_attribute($variables);

  // Adds the html head link like `<link rel="next" href="/example?page=1">`.
  drupal_add_html_head_link($attributes);
}

/**
 * Builds the pager query parameter array for the href attribute.
 *
 * @see theme_pager_link()
 */
function _os_pager_build_href_attribute($variables) {
  $parameters = $variables['parameters'];
  $page_new = $variables['page_new'];
  $element = $variables['element'];
  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }
  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }
  $href = url($_GET['q'], array('query' => $query));

  return $href;
}
