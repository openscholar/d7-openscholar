<?php
// $Id$

/**
 * Full Node View
 * Displays the citation in a field that can be clicked to select all of it
 * Also displays abstract, full text and notes if enabled.
 *
 * @param unknown_type $vars
 */
function theme_os_publications_tabular(&$vars) {
  $node = $vars['node'];
  $base = $vars['base'];

  $fields = _biblio_get_field_information($node->biblio_type, TRUE);
  _biblio_localize_fields($fields);

  /* This field hasn't been added yet
  if ($node->biblio_url) {
    $attrib           = (variable_get('biblio_links_target_new_window', null)) ? array('target' => '_blank') : array();
    $url_title        = db_result(db_query('SELECT biblio_url_title FROM {biblio} WHERE nid = %d', $node->nid));
    $url_title        = (isset($url_title) && $url_title) ? $url_title : 'Website';
    $node->biblio_url = l($url_title, $node->biblio_url, $attrib);
  }
  */

  $citation = _os_publications_plain_citation($node);

  // These fields should not be printed under any circumstances
  // They appear elsewhere with special handling, either in the citation
  // or elsewhere in this function
  $hidden = array(
    'biblio_pages',
    'biblio_publisher',
    'biblio_volume',
    'biblio_issue',
    'biblio_edition',
    'biblio_year',
    'biblio_place_published',
    'biblio_type_name',
    'biblio_secondary_title',
    'biblio_coins',
    'biblio_lang',
  );

  if (variable_get('biblio_citeproc_style') == 'harvard-chicago-author-date.csl') {
    $hidden = array_merge($hidden, array(
      'biblio_url',
      'biblio_section',
      'biblio_number',
      'biblio_number_of_volumes',
      'biblio_type_of_work',
      'biblio_date',
    ));
  }

  $rows = array();

  foreach ($fields as $f) {
    // field has already been processed. do nothing
    if (in_array($f['name'], $hidden)) {
      continue;
    }
    // field doesn't exist. do nothing
    if (!isset($node->{$f['name']})) {
      continue;
    }

    $value = $node->{$f['name']};
    $display_title = TRUE;

    switch ($f['name']) {
      case 'biblio_url':
        if(empty($node->biblio_url)) {
          $data = '<p></p>';
        }else {
          // no need to check plain. l does it for us
          $data = '<p>' . l(!empty($node->biblio_url_title) ? $node->biblio_url_title : 'Website', $value) . '</p>';
        }
        $display_title = FALSE;
        break;

      case 'biblio_url_title':
        break;

      default:
        if ($f['type'] == 'text_format') {
          $data = check_markup($value, isset($node->biblio_formats[$f['name']]) ? $node->biblio_formats[$f['name']] : filter_default_format());
        }
        else {
          $data = check_plain($value);
        }
    }
    if (trim($data)) {
      if ($display_title) {
        $rows[] = array(
          array(
            'data' => t($f['title']),
            'class' => 'biblio-row-title biblio-field-title-' . str_replace('_', '-', str_replace('biblio_', '', $f['name'])),
          ),
          array(
            'data' => $data,
            'class' => 'biblio-field-contents-' . str_replace('_', '-', str_replace('biblio_', '', $f['name'])),
          ),
        );
      }
      else {
        $rows[] = array(
          1 => array(
            'data' => $data,
            'class' => 'biblio-field-contents-' . str_replace('_', '-', str_replace('biblio_', '', $f['name'])),
          ),
        );
      }
    }
  }

  $output = '<div id="biblio-node">';
  $output .= filter_xss($node->biblio_coins, array('span'));
  $field_upload = '';
  if (isset($vars['node']->field_upload[LANGUAGE_NONE])) {
    $build = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'biblio-upload-wrapper',
        ),
      ),
      'label' => array(
        '#theme' => 'html_tag',
        '#tag' => 'label',
        '#attributes' => array(
          'class' => 'biblio-download-links-label',
        ),
        '#value' => t('Download'),
      ),
      'field_upload' => field_view_field('node', $vars['node'], 'field_upload', 'full'),
    );
    $field_upload .= drupal_render($build);
    unset($vars['node']->field_upload[LANGUAGE_NONE]);
  }

  // Add label to the table of uploaded files.
  $output .= '<div id="citation-wrapper">' . $citation . '' . theme('biblio_export_links', array('node' => $node)) . '</div>';
  // Add Upload Div.
  $output .= $field_upload;

  // Add pub-cover-content-wrapper.
  $output .= '<div id="pub-cover-content-wrapper">';

  // Move publication image up under the citation and remove HTML from title
  $book_images = field_get_items('node',$node,'field_biblio_image', $node->language);
  if (!empty($book_images)) {
    $book_image = reset($book_images);
    $theme_image = theme('image_style', array(
      'style_name' => 'book_cover',
      'path' => $book_image['uri'],
      'title' => html_entity_decode(strip_tags(htmlspecialchars_decode($book_image['title'])), ENT_COMPAT, "UTF-8"),
      'alt' => html_entity_decode(strip_tags(htmlspecialchars_decode($book_image['alt'])), ENT_COMPAT, "UTF-8"),
    ));
    $book_output = '<div class="publication-cover-container">' . $theme_image . '</div>';
    // Add the image to the output under the citation.
    $output .= $book_output;
  }

  // Wrap all remaining content in a div.
  $output .= '<div class="pubcontent-wrapper">';

  foreach ($rows as $row) {
    if (isset($row[0]['data']) && $row[0]['data']) {
      $output .= "<h3>" . $row[0]['data'] . ":</h3>";
    }
    $output .= $row[1]['data'];
  }

  if (isset($node->body[LANGUAGE_NONE][0]['value']) && strlen(trim($node->body[LANGUAGE_NONE][0]['value'])) && user_access('view full text')) {
    $output .= '<h3>Full Text</h3>';
    $output .= check_markup($node->body[LANGUAGE_NONE][0]['value'], $node->body[LANGUAGE_NONE][0]['format'], FALSE);
  }

  // End PubContent Wrapper
  $output .= '</div>';

  $output .= '</div>';

  // End "pub-cover-content-wrapper" div.
  $output .= '</div>';

  return $output;
}

/**
 * Get the citation for a biblio node as just plain text
 */
function _os_publications_plain_citation($node) {
  $style = biblio_get_style();
  $base = variable_get('biblio_base', 'biblio');

  // Note that this is a working paper after the title and before the year
  if ($style == 'chicago' && in_array($node->biblio_type, array(1000)) && empty($node->biblio_publisher)) {
    $node->biblio_publisher = "Working Paper";
  }

  $citation = theme('biblio_style', array('node' => $node, 'base' => $base, 'style_name' => $style));

  // Add the patent number, patent source and patent issuing country, if no
  // present in the citation.
  if ($node->biblio_type == 119) {
    if (!empty($node->biblio_section) && strpos($citation, ' ' . $node->biblio_section) === FALSE){
      $citation .= ' ' . $node->biblio_section;
    }
    if (!empty($node->biblio_volume) && strpos($citation, ' ' . $node->biblio_volume) === FALSE){
      $citation .= ' Patent ' . $node->biblio_volume . '.';
    }
    if (!empty($node->biblio_issue) && strpos($citation, $node->biblio_issue) === FALSE){
      $citation .= ' Patent source: ' . $node->biblio_issue . '.';
    }
  }

  if (variable_get('biblio_citeproc_style') == 'harvard-chicago-author-date.csl') {

    // Pushing day and month in those both are present in Publication for Harvard - Chicago Author cititation
    if (!empty($node->biblio_year) && strpos($citation, $node->biblio_date . '.') !== FALSE && !empty($node->field_biblio_pub_month['und'][0]['value'])) {
      // In case day value is present
      if (!empty($node->field_biblio_pub_day['und'][0]['value'])) {
        $day = $node->field_biblio_pub_day['und'][0]['value'] . '/';
      } else {
        $day = '';
      }
      $citation = str_replace($node->biblio_year . '.', $node->field_biblio_pub_month['und'][0]['value'] . '/' . $day . $node->biblio_year . '. ', $citation);
    }

    $fields = array(
      103 => array(
        'biblio_date' => 'Date Presented',
      ),
      108 => array(
        'biblio_type_of_work' => 'Thesis Type',
      ),
      110 => array(
        'biblio_type_of_work' => 'Medium',
      ),
      117 => array(
        'biblio_section' => 'Code Section',
      ),
      116 => array(
        'biblio_number' => 'Number'
      ),
      118 => array(
        'biblio_section' => 'Sections',
        'biblio_number_of_volumes' => 'Statute Number',
      ),
      120 => array(
        'biblio_number_of_volumes' => 'Communication Number',
        'biblio_type_of_work' => 'Type',
      ),
      124 => array(
        'biblio_type_of_work' => 'Type of Work',
      ),
      127 => array(
        'biblio_number_of_volumes' => 'Number of volumes',
      ),
      128 => array(
        'biblio_section' => 'Section Number',
        'biblio_number_of_volumes' => 'Session Number',
      ),
    );

    if (!empty($fields[$node->biblio_type])) {
      foreach ($fields[$node->biblio_type] as $field => $label) {
        if (!empty($node->{$field}) && strpos($citation, $node->{$field}) === FALSE) {
          $citation .= ' ' . t($label . ': @data.', array('@data' => $node->{$field}));
        }
      }
    }

    // Add the publisher into the citation.
    if (!empty($node->biblio_url) && strpos($citation, $node->biblio_url) === FALSE) {
      $citation .= " <a href='{$node->biblio_url}'>{$node->biblio_url_title}</a>";
    }
  }


  $allowed_html_elements = variable_get('html_title_allowed_elements', array('em', 'sub', 'sup', 'a'));
  $citation = filter_xss($citation, $allowed_html_elements);

  // add url to citation
  if (function_exists('shorten_url') && variable_get('os_publications_shorten_citations')) {
    /* replace punctuation at end of citation with period.
     * biblio_coins follow end of citation and block an easy RE match, so we're appending them.
     */
    $coins = filter_xss($node->biblio_coins, array('span'));
    $re = '/' . '[!-~] (<\/\w*>)?' . preg_quote($coins, '/') . '/';

    if (preg_match($re, $citation)) {
      $citation = preg_replace($re, ", </span>$coins", $citation);
    }

    $short_url = shorten_url(url('node/' . $node->nid, array('absolute' => TRUE)));
    if (!preg_match('|^http(s?)://|', $short_url)) {
      $short_url = 'http://' . $short_url;
    }

    $citation .= ' Copy at&nbsp;' . l($short_url, $short_url);
  }

  drupal_add_js('function selectText(ele) {
  try {
    var div = document.createRange();
    div.setStartBefore(ele);
    div.setEndAfter(ele) ;
    window.getSelection().addRange(div);
  }
  catch (e) {
    // we\'re in internet explorer
    div = document.selection.createRange();
    div.moveToElementText(ele);
    div.select();
  }
}', 'inline');

  $citation = "<h3>Citation:</h3><div class=\"biblio-citation\" onclick=\"if (event.target) { selectText(event.target); } else{ selectText(this);} \">{$citation}</div>";

  return $citation;
}

function theme_os_publications_export_links($variables) {
  if (module_exists('vsite') && !spaces_get_space()) {
    // Don't show export link when not in a VSite, to prevent from loading too
    // many nodes.
    return;
  }

  global $pager_total_items;
  $node   = $variables['node'];
  $filter = ($variables['node'] == NULL && isset($variables['filter'])) ? $variables['filter'] : array();
  $links  = array();
  $output = '';

  if (biblio_access('export')) {
    $show_link = variable_get('biblio_lookup_links', array('google' => TRUE));
    $lookup_links = module_invoke_all('biblio_lookup_link', $node);
    $nid          = (isset($node->nid)) ? $node->nid : NULL;
    $export_links = module_invoke_all('biblio_export_link', $nid, $filter);
    $links        = array_merge($lookup_links, $export_links);
  }
  $title = isset($node->nid) ? t('Download Citation') : (!isset($_REQUEST['page']) ? t('Download Citations') : '');

  if (!empty($links)) {
    return theme('ctools_dropdown', array('links' => $links, 'title' => $title, 'attributes' => array('class' => array('biblio-export-buttons'))));
  }
  else {
    return "";
  }
}

/**
 * Overrides theme_biblio_contributors().
 */
function theme_os_publications_contributors($variables) {
  $form         = $variables['form'];
  $rows         = array();
  drupal_add_tabledrag($form['#id'], 'order', 'sibling', 'rank');

  foreach (element_children($form) as $key) {
    // No need to print the field title every time.
    //    unset($form[$key]['name']['#title'], $form[$key]['auth_type']['#title'], $form[$key]['auth_category']['#title']);
    // Add class to group weight fields for drag and drop.
    $form[$key]['rank']['#attributes']['class'] = array('rank');

    // Build the table row.
    $row = array('');
    $row[] = array('data' => drupal_render($form[$key]['auth_type']),
      'class' => array('biblio-contributor-type'),
    );
    $row[] = array('data' => drupal_render($form[$key]['name']) . drupal_render($form[$key]['auth_category']) . drupal_render($form[$key]['delete']),
      'class' => array('biblio-contributor', 'biblio-contributor-category'),
    );
    $row[] = drupal_render($form[$key]['rank']);
    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }
  $output = theme('table', array('rows' => $rows, 'attributes' => array('id' => $form['#id'], 'class' => 'biblio-contributors')));
  //$output .= drupal_render_children($form);
  return $output;
}

/**
 * Replacement for the default biblio download links theme
 */
function theme_os_publications_download_links($variables) {
  static $langcode = NULL;
  $file_links = array();
  $node = $variables['node'];
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  $fields = field_attach_view('node', $node, 'full', $langcode);
  foreach (element_children($fields) as $field) {

    if ($fields[$field]['#access'] && $fields[$field]['#field_type'] == 'file' && $field != 'field_biblio_image') {
      foreach ($fields[$field]['#items'] as $delta => $item) {
        /*if (module_exists('filefield_paths')) {
          $alias = drupal_get_path_alias('filefield_paths/alias/' . $item['fid']);
          $item['uri'] = $alias;
        }*/

        $file_links[] = array(
          theme('file_link', array('file' => (object) $item)),
        );
      }
    }
  }
  $files = '';
  if (count($file_links) > 0 && (user_access('show download links') || user_access('show own download links'))) {
    $files .= '<span class="biblio_file_links">';
    // $files .= '&nbsp;' . t('Download') . ':&nbsp;';
    $file_count = 0;
    foreach ($file_links as $file) {
      $files .= $file[0];
    }
    $files .= '</span>';
  }

  return $files;
}

/**
 * Replacement for biblio's sorting tabs
 *
 * Puts them in a ctools dropdown.
 */
function theme_os_publications_sort_tabs($vars) {
  $links = $vars['links'];
  $title = '';
  $sort_boolean = FALSE;

  foreach ($links as &$l) {
    if (isset($l['arrow'])) {
      if ($l['active']) {
        $title = $l['text'];
      }
      $sort_boolean = TRUE;
      $l['title'] = $l['text'] . $l['arrow'];
      $l['href'] = $l['path'];
    }
  }

  // Make sure there was at least one sorting option selected.
  if($sort_boolean == FALSE) {
    return;
  }

  // this arrow points down for some reason, despite being named 'asc'
  return theme('ctools_dropdown', array('title' => 'Sort '.theme('image', array('path' => drupal_get_path('module', 'biblio').'/misc/arrow-asc.png')), 'links' => $links, 'image' => TRUE));
}
