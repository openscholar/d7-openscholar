<?php
/**
 * Preprocess and Process Functions SEE: http://drupal.org/node/254940#variables-processor
 * 1. Rename each function and instance of "adaptivetheme_subtheme" to match
 *    your subthemes name, e.g. if your theme name is "footheme" then the function
 *    name will be "footheme_preprocess_hook". Tip - you can search/replace
 *    on "adaptivetheme_subtheme".
 * 2. Uncomment the required function to use.
 */

/**
 * add classes to the body of a page
 */
function os_basetheme_preprocess_html(&$vars) {
  if ($node = menu_get_object('node')) {
    if ($node->type == 'page' && isset($node->field_os_css_class)) {
      // Add custom css class
      if (!empty($node->field_os_css_class)) {
        $vars['classes_array'][] = $node->field_os_css_class[LANGUAGE_NONE][0]['value'];
      }
    }
  }
  if (isset($vars['page']['menu_bar'])) {
    $vars['classes_array'][] = 'navbar-on';
  }
  else {
    $vars['classes_array'][] = 'navbar-off';
  }

  $vars['use_responsive_behaviors'] = (bool) variable_get('enable_responsive', true);
}

/**
 * Adds classes to the page element
 */
function os_basetheme_preprocess_page(&$vars) {
  $item = menu_get_item();

  // Remove the node view tab.
  if (!empty($vars['tabs']['#primary'])) {
    foreach ($vars['tabs']['#primary'] as $k => $l) {
      if ($l['#link']['path'] == 'node/%/view') {
        unset($vars['tabs']['#primary'][$k]);
      }
    }
  }

  //Adds OpenScholar header region awareness to body classes
  $header = array(
    'header-left' => $vars['page']['header_first'],
    'header-main' => $vars['page']['header_second'],
    'header-right' => $vars['page']['header_third'],
  );
  $content = array(
    'content-top' => $vars['page']['content_top'],
    'content-left' => $vars['page']['content_first'],
    'content-right' => $vars['page']['content_second'],
    'content-bottom' => $vars['page']['content_bottom'],
  );
  $footer = array(
    'footer-left' => $vars['page']['footer_first'],
    'footer' => $vars['page']['footer'],
    'footer-right' => $vars['page']['footer_third'],
  );
  foreach (array('header', 'content', 'footer') as $var) {
    $visible = array_filter($$var, "_os_basetheme_is_empty");
    if (count($visible)) {
      $vars['classes_array'] = array_merge($vars['classes_array'], array_keys($visible));
     }
    else {
      $vars['classes_array'][] = $var.'-none';
    }
  }

  if (module_exists('overlay') && overlay_get_mode() == 'child') {
    // overlay does this, but adaptive theme renders them in a different way that overlay doesn't expect
    $vars['primary_local_tasks'] = $vars['title'] = false;
  }

  if (!isset($vars['use_content_regions'])) {
    $vars['use_content_regions'] = FALSE;
  }

  // Do not show the login button on the following pages, redundant.
  $login_pages = array(
  'user',
  'private_site',
  'user/password'
  );
  if(isset($item) && !in_array($item['path'], $login_pages)) {
    $vars['login_link'] = theme('openscholar_login');
  }

  //hide useless tabs - drupal uses $vars['tabs'], but adaptive loads primary and secondary menu local tasks.
  $vars['primary_local_tasks'] = !empty($vars['tabs']['#primary']) ? $vars['tabs']['#primary'] : '';
  $vars['secondary_local_tasks'] = $vars['tabs']['#secondary'];

  $theme_name = $GLOBALS['theme_key'];

  // Adds skip link var to page template
  $vars['skip_link'] = 'main-content';
  if (at_get_setting('skip_link_target', $theme_name)) {
    $skip_link_target = at_get_setting('skip_link_target', $theme_name);
    $vars['skip_link'] = trim(check_plain($skip_link_target), '#');
  }

  // Load install-specific css here
  if (module_exists('scholar')) {
    if (variable_get('os_appearance_'.$theme_name.'_flavor', 'default') == 'default') {
      $path = drupal_get_path('module', 'scholar').'/css/'.$GLOBALS['theme_key'].'_override.css';
      if (file_exists($path)) {
        drupal_add_css ($path, array (
          'group' => CSS_THEME,
          'weight' => 9999,
          'preprocess' => FALSE
        ));
      }
    }
  }
}

/**
 * For header region classes
 */
function _os_basetheme_is_empty($s){
  return $s ? TRUE : FALSE;
}

/**
 * Hide adapativetheme's status message complaining about missing files
 */
function os_basetheme_preprocess_status_messages(&$vars) {
  if (isset($_SESSION['messages']['warning'])) {
    foreach ($_SESSION['messages']['warning'] as $k => $v) {
      if (strpos($v, 'One or more CSS files were not found') !== FALSE) {
        unset($_SESSION['messages']['warning'][$k]);
      }
    }

    if (count($_SESSION['messages']['warning']) == 0) {
      unset ($_SESSION['messages']['warning']);
    }
  }
}

function os_basetheme_preprocess_overlay(&$vars) {
  // we never want these. They look awful.
 $vars['tabs'] = strpos($_GET['q'], 'os-importer/') ? FALSE : menu_primary_local_tasks();
}

/**
 * Returns HTML for a menu link and submenu.
 *
 * Adaptivetheme overrides this to insert extra classes including a depth
 * class and a menu id class. It can also wrap menu items in span elements.
 *
 * @param $vars
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 */
function os_basetheme_menu_link(array $vars) {
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

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>";
}

/**
 * Preprocess variables for node.tpl.php
 */
function os_basetheme_preprocess_node(&$vars) {
  // Event nodes, inject variables for date month and day shield
  if ($vars['node']->type == 'event' && (!empty($vars['sv_list']) || !$vars['page'])) {
    $vars['event_start'] = array();
    $delta = 0;
    if (isset($vars['node']->date_id)) {
      list(,,, $delta,) = explode('.', $vars['node']->date_id . '.');
    }
    if (isset($vars['field_date'][$delta]['value']) && !empty($vars['field_date'][$delta]['value'])) {
      // Define the time zone in the DB as a UTC.
      $date = new DateTime($vars['field_date'][$delta]['value'], new DateTimeZone('utc'));

      // Get the timezone of the site and apply it on the date object.
      $time_zone = date_default_timezone();
      $date->setTimezone(new DateTimeZone($time_zone));

      $vars['event_start']['month'] = check_plain($date->format('M'));
      $vars['event_start']['day'] = check_plain($date->format('d'));
      $vars['event_start']['year'] = check_plain($date->format('Y'));
      $vars['classes_array'][] = 'event-start';

      // For events with a repeat rule we add the delta to the query string.
      if ($vars['content']['field_date']['#items'][0]['rrule']) {
        $vars['node_url'] .= '?delta=' . $delta;
      }

      // Unset the date id to avoid displaying the first repeat in all the
      // event's results on the page.
      $vars['node']->date_id = NULL;
    }
  }
  elseif ($vars['node']->type == 'event' && $vars['page'] && !empty($vars['content']['field_date']['#items'][0]['rrule'])) {
    // We are in a page of a repeated event so we need to display the date
    // according to the delta.
    $delta = isset($_GET['delta']) ? $_GET['delta'] : 0;

    // We move the wanted delta to be the first element in order to get the
    // desired markup.
    $vars['node']->field_date['und'][0] = $vars['node']->field_date['und'][$delta];

    // Get the repeat rule.
    $rule = theme('date_repeat_display', array(
      'item' => array('rrule' => $vars['content']['field_date']['#items'][0]['rrule']),
      'field' => field_info_field('field_date'),
    ));

    // Get the date field. The delta we want to display will be returned.
    $field = field_view_field('node', $vars['node'], 'field_date', array('full'));

    // Rebuild the markup for the date field.
    $vars['content']['field_date'][0]['#markup'] = $rule . ' ' . $field[0]['#markup'];

    // Don't display the repeats in full view mode.
    foreach ($vars['content']['field_date'] as $index => $repeat ) {
      if ($index && is_integer($index)) {
        hide($vars['content']['field_date'][$index]);
      }
    }
  }

  // Event persons, change title markup to h1
  if ($vars['type'] == 'person') {
    if (!$vars['teaser'] && $vars['view_mode'] != 'sidebar_teaser') {
      $vars['title_prefix']['#suffix'] = '<h1 class="node-title">' . $vars['title'] . '</h1>';

      if ($vars['view_mode'] == 'slide_teaser') {
        $vars['title_prefix']['#suffix'] = '<div class="toggle">' . $vars['title_prefix']['#suffix'] . '</div>';
      }
    }

  }

  // Show the body last in a presentation.
  if ($vars['type'] == 'presentation' && $vars['view_mode'] == 'full') {
    $vars['content']['body']['#weight'] = 999;
  }
}

/**
 * Returns HTML for a link
 *
 * Only change from core is that this makes href optional for the <a> tag
 */
function os_basetheme_link(array $variables) {
  $href = ($variables['path'] === false)?'':'href="' . check_plain(url($variables['path'], $variables['options'])) . '" ';
  return '<a ' . $href . drupal_attributes($variables['options']['attributes']) . '>' . ($variables['options']['html'] ? $variables['text'] : check_plain($variables['text'])) . '</a>';
}

/**
 * The adaptive theme implements a hook_menu_tree but return a rendered ul
 * wrapped with a ul - this will cause to the menu html to be"
 *  <ul...>
 *    <ul ...>
 *      ...
 *    </ul>
 *  </ul>
 *
 * We need to implement our own hook_menu_tree to prevent a double ul tag
 * wrapping.  But when we're not using nice menus, use the original adaptive theme.
 */
function os_basetheme_menu_tree(&$variables) {
  if (isset($variables['os_nice_menus']) && $variables['os_nice_menus']) {
    return $variables['tree'];
  }

  return adaptivetheme_menu_tree($variables);
}

/**
 * Implements template_preprocess_HOOK() for theme_menu_tree().
 *
 * template_preprocess_menu_tree has been removed.  This replaces it and sets a flag
 * when we're using nice_menus.
 */
function os_basetheme_preprocess_menu_tree(&$variables) {
  if (isset($variables['tree']['#theme'])) {
    $variables['os_nice_menus'] = ($variables['tree']['#theme'] == 'os_nice_menus');
  }
  else {
    $variables['os_nice_menus'] = false;
  }
  $variables['tree'] = $variables['tree']['#children'];
}

/**
 * Implements template_process_HOOK() for theme_pager_link().
 */
function os_basetheme_process_pager_link($variables) {
  // Adds an HTML head link for rel='prev' or rel='next' for pager links.
  module_load_include('inc', 'os', 'includes/pager');
  _os_pager_add_html_head_link($variables);
}

/**
 * Implements hook_theme_registry_alter().
 * Set OS profiles preprocess to run before any other preprocess function.
 */
function os_basetheme_theme_registry_alter(&$theme_registry) {
  array_unshift($theme_registry['image_formatter']['preprocess functions'], "os_profiles_preprocess_image_formatter");
}

/**
 * Returns HTML for status and/or error messages, grouped by type.
 *
 * @param $vars
 *   An associative array containing:
 *   - display: (optional) Set to 'status' or 'error' to display only messages
 *     of that type.
 */
function os_basetheme_status_messages($vars) {
  $display = $vars['display'];
  $output = '';
  $allowed_html_elements = '<'. implode('><', variable_get('html_title_allowed_elements', array('em', 'sub', 'sup'))) . '>';

  foreach (drupal_get_messages($display) as $type => $messages) {
    $output .= '<div id="messages"><div class="messages ' . $type . '">';
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
    $output .= "</div></div>";
  }
  return $output;
}

/**
 * Implements theme_views_view_field.
 *
 * Here we add the delta to the query string for the node title's link and
 * returning the new output.
 */
function os_basetheme_views_view_field($vars) {
  $view = $vars['view'];
  if ($view->name != 'os_events') {
    return $vars['output'];
  }

  $field = $vars['field'];
  if ($field->field != 'title') {
    return $vars['output'];
  }
  $row = $vars['row'];

  $options = array(
    'query' => array(
      'delta' => $row->field_data_field_date_delta,
    ),
  );
  return l($row->node_title, 'node/' . $row->nid, $options);
}

/**
 * Returns HTML for a date element formatted as a range.
 *
 * Changes the "to" in a date range to a "-" for the month/week/day views.
 *
 * @see theme_date_display_range().
 */
function os_basetheme_date_display_range($variables) {
  $date1 = $variables['date1'];
  $date2 = $variables['date2'];
  $timezone = $variables['timezone'];
  $attributes_start = $variables['attributes_start'];
  $attributes_end = $variables['attributes_end'];

  $displays = array(
    'month',
    'week',
    'day',
  );
  $from_to = ' to ';
  if (os_events_in_view_context($displays))  {
    $from_to = ' - ';
    $date1 = str_replace(array('am', 'pm'), array('a', 'p'), $date1);
    $date2 = str_replace(array('am', 'pm'), array('a', 'p'), $date2);
  }

  // Wrap the result with the attributes.
  return t('!start-date ' . $from_to .' !end-date', array(
    '!start-date' => '<span class="date-display-start"' . drupal_attributes($attributes_start) . '>' . $date1 . '</span>',
    '!end-date' => '<span class="date-display-end"' . drupal_attributes($attributes_end) . '>' . $date2 . $timezone . '</span>',
  ));
}

/**
 * Returns HTML for a date element formatted as a single date.
 *
 * Changes am/pm to be a/p and adds a span around the date.
 */
function os_basetheme_date_display_single($variables) {
  $date = $variables['date'];
  $timezone = $variables['timezone'];
  $attributes = $variables['attributes'];

  $displays = array(
    'month',
    'week',
    'day',
  );
  if (os_events_in_view_context($displays))  {
    $date = str_replace(array('am', 'pm'), array('a', 'p'), $date);
    $formatted_date = $variables['dates']['value']['formatted_date'];
    $date = str_replace($formatted_date, '<span class="event-date">' . $formatted_date . '</span>', $date);
  }

  // Wrap the result with the attributes.
  return '<span class="date-display-single"' . drupal_attributes($attributes) . '>' . $date . $timezone . '</span>';
}

/**
 * Check if we are in the os_events view context.
 *
 * @param array $display_titles
 *   The display titles to check for. If not provided we check only for
 *   the view context with no particular display.
 * @return bool
 *   Returns TRUE if we are in the os_events view context with the optional
 *   supplied display titles. FALSE otherwise.
 */
function os_events_in_view_context($display_titles = array()) {
  $view = views_get_current_view();
  if (!$view || $view->name != 'os_events') {
    return FALSE;
  }

  if (empty($display_titles)) {
    return $view->name == 'os_events';
  }

  $display_names = array();
  foreach ($view->display as $name => $display) {
    if (in_array(strtolower($display->display_title), $display_titles)) {
      $display_names[] = $name;
    }
  }

  return in_array($view->current_display, $display_names);
}

/*
 * Implements override for theme_date_repeat_display of contrib module date_repeat
 * @param array $vars
 * theme parameters
 * Here we are altering the display text of the event occurrences in repetitive events
 * @return string
 * returning the output as HTML.
 */
function os_basetheme_date_repeat_display($vars) {
  $field = $vars['field'];
  $item = $vars['item'];
  $entity = !empty($vars['node']) ? $vars['node'] : NULL;
  $output = '';
  if (!empty($item['rrule'])) {
    $output = os_date_repeat_rrule_description($item['rrule']);
    $output = '<div class="date-repeat-rule">' . $output . '</div>';
  }
  return $output;
}

/**
 * Returns HTML for an image field.
 *
 * Output image fields as figure with figcaption for captioning.
 */
function os_basetheme_field__image($vars) {
  global $theme_key;
  $theme_name = $theme_key;
  $output = '';

  // Render the label, if it's not hidden.
  if (!$vars['label_hidden']) {
    $output .= '<h2 class="field-label"' . $vars['title_attributes'] . '>' . $vars['label'] . ':&nbsp;</h2>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $vars['content_attributes'] . '>';

  foreach ($vars['items'] as $delta => $item) {

    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<figure class="clearfix ' . $classes . '"' . $vars['item_attributes'][$delta] .'>';
    $output .= drupal_render($item);

    // Captions
    if (isset($item['#item']['os_file_description'][LANGUAGE_NONE][0]['value'])) {
      // Ouch this is ugly, please tell me how to get the image style width?
      $styles = '';
      $width = 'auto';
      preg_match('/< *img[^>]*width *= *["\']?([^"\']*)/i', $item['#children'], $matches);
      $width = $matches[1] . 'px';
      $styles = 'style="width:' . $width . ';"';
      $image_caption = strip_tags($item['#item']['os_file_description'][LANGUAGE_NONE][0]['value']);
      if ($vars['field_view_mode'] == 'full') {
        $output .= '<figcaption ' . $styles . '>' . $image_caption . '</figcaption>';
      }
    }

    $output .= '</figure>';
  }

  $output .= '</div>';

  // Render the top-level wrapper element.
  $tag = $vars['tag'];
  $output = "<$tag class=\"" . $vars['classes'] . '"' . $vars['attributes'] . '>' . $output . "</$tag>";

  return $output;
}

/**
 * Implements override for theme_pager_lite_next of contrib module views_litepager
 * @param array $variables
 * theme parameters
 * @return string
 * returning the output as HTML.
 */
function os_basetheme_pager_lite_next($variables) {
  $text = $variables['text'];
  $element = $variables['element'];
  $interval = $variables['interval'];
  $parameters = $variables['parameters'];

  global $pager_page_array, $pager_total;
  $output = '';

  // If we are anywhere but the last page
  if ($pager_page_array[$element] <= ($pager_total[$element] - 1) || $pager_page_array[$element] == 0) {
    $page_new = pager_load_array($pager_page_array[$element] + $interval, $element, $pager_page_array);
    // If the next page is the last page, mark the link as such.
    if ($pager_page_array[$element] == $pager_total[$element]) {
      $output = theme('pager_last', array('text' => $text, 'element' => $element, 'parameters' => $parameters));
    }
    // The next page is not the last page.
    else {
      $output = theme('pager_link', array('text' => $text, 'page_new' => $page_new, 'element' => $element, 'parameters' => $parameters));
    }
  }

  return $output;
}

/**
 * Override theme_facetapi_count
 */
function os_basetheme_facetapi_count($variables) {
  return '<span class="facetapi-count">(' . (int) $variables['count'] . ')</span>';
}

/**
 * Override theme_views_mini_pager
 */
function os_basetheme_views_mini_pager($vars) {
  global $pager_page_array, $pager_total;
  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];

  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  if ($pager_total[$element] > 1) {

    $li_previous = theme('pager_previous',
      array(
        'text' => t('«'),
        'element' => $element,
        'interval' => 1,
        'parameters' => $parameters,
      )
    );
    if (empty($li_previous)) {
      $li_previous = "&nbsp;";
    }

    $li_next = theme('pager_next',
      array(
        'text' => t('»'),
        'element' => $element,
        'interval' => 1,
        'parameters' => $parameters,
      )
    );

    if (empty($li_next)) {
      $li_next = "&nbsp;";
    }

    $items[] = array(
      'class' => array('pager-previous'),
      'data' => $li_previous,
    );

    $items[] = array(
      'class' => array('pager-current'),
      'data' => t('@current of @max', array('@current' => $pager_current, '@max' => $pager_max)),
    );

    $items[] = array(
      'class' => array('pager-next'),
      'data' => $li_next,
    );
    return theme('item_list',
      array(
        'items' => $items,
        'title' => NULL,
        'type' => 'ul',
        'attributes' => array('class' => array('pager mini-pager')),
      )
    );
  }
}

/**
 * Override theme_file_link
 */
function os_basetheme_file_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = file_create_url($file->uri);
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }
  if (isset($file->view_mode) && $file->view_mode == 'os_files_link') {
    // If Link with no icon is selected for LOF widget
    return '<span class="file">' . l($link_text, $url, $options) . '</span>';
  } else{
    return '<span class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</span>';
  }
}
