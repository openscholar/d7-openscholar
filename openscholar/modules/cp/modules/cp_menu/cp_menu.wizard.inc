<?php
// $Id$


/**
 * @file cp_menu.wizard.inc
 *
 * Include file for cp_menu link maker forms and for helper functions that only assist the wizard
 **/

/**
 * Multi-step form wizard for adding links to menu.
 *
 * @param unknown_type $js
 * @param unknown_type $step
 */
function cp_menu_page($step = NULL, $menu = NULL) {
  // can we collect the original menu?
  // should that be part of the menu link so the result can be added back in?

  // Gets cached results
  $object_id = 'cp_menu';
  if (empty($step) || $step == 'new') {
    $step = 'type';
    cp_menu_cache_clear($object_id);
  }
  $object = cp_menu_cache_get($object_id);

  // Checks whether step is a menu?  if so, stash it and clear it.
  ctools_include('menu', 'os');
  if (in_array($menu, array_flip(os_get_menus()))) {
    $object['menu'] = $menu;
    cp_menu_cache_set($object_id, $object);
  }

  // Initializes a form_state
  $form_state = array(
    'object_id' => $object_id,
    'object' => & $object,
  );

  // Prepares multistep form info
  $form_info = array(
    'id' => 'cp_menu_form',
    'path' => 'cp/build/menu/link/%step',
    'show trail' => FALSE,
    'show back' => TRUE,
    'show cancel' => TRUE,
    'next callback' => 'cp_menu_wizard_next',
    'finish callback' => 'cp_menu_wizard_finish',
    'cancel callback' => 'cp_menu_wizard_cancel',
    'forms' => array(
      'type' => array(
        'form id' => 'cp_menu_type',
        'title' => t('Link Type'),
      ),
    ),
  );

  // Add type dependent pages based on type form's results
  if (isset($_POST['type'])) {
    $type = $_POST['type'];
    $object['type'] = $type;
    cp_menu_cache_set($object_id, $object);
  }
  elseif (isset($object['type'])) {
    $type = $object['type'];
  }
  else {
    $type = NULL;
  }

  // Sets new node finish text to "continue" instead of "finish",
  // because node creation is another step, albeit outside of wizard
  if (isset($object['type']) && $object['type'] == 'page') {
    $form_info['finish text'] = t('Continue');
  }

  if ($type == 'list') {
    $form_info['forms']['list_type'] = array('form id' => 'cp_menu_list_type', 'title' => t('Type of List'));

    $list_type = (isset($object['list_type'])) ? $object['list_type'] : $_POST['list_type'];
    if ($list_type == 'taxonomy') {
      $form_info['forms']['term'] = array('form id' => 'cp_menu_list_term', 'title' => t('Taxonomy Terms'));
    }
    elseif ($list_type == 'publications') {
      // pubs.
      $form_info;
    }
  }

  // Adds final page after type sensitive pages have been included
  $form_info['forms']['title'] = array('form id' => 'cp_menu_title', 'title' => t('Link Title'));

  // Builds and renders the form.
  ctools_include('wizard');
  $form = ctools_wizard_multistep_form($form_info, $step, $form_state);
  $output = drupal_render($form);
  overlay_set_regions_to_render();

  return $output;
}

/**
 * Multistep form page; presents radio options page and url.
 */
function cp_menu_type($form, &$form_state) {
  $form_state['title'] = t('Link Type');

  $form['type'] = array(
    '#title' => 'Item Type',
    '#type' => 'radios',
    '#options' => array(
      'page' => t('Post'),
      'url' => t('URL'),
      'home' => t('Home'),
      'heading' => t('Menu Heading')),
    '#description' => t('Select heading or type of content to link to.'),
    '#required' => TRUE,
  );

  if (isset($form_state['object']) && isset($form_state['object']['type'])) {
    $form['type']['#default_value'] = $form_state['object']['type'];
  }

  return $form;
}


/**
 * Verifying the user didn't reached the maximum menu items the DB can hold.
 */
function cp_menu_title_validate($form, $form_state) {
  $vsite = vsite_get_vsite();
  $item = $form_state['values'];

  $removing_keys = array('previous', 'return', 'cancel', 'form_build_id', 'form_token', 'form_id', 'op');
  foreach ($removing_keys as $key) {
    unset($item[$key]);
  }
  $item = $item + array('link_path' => '');

  // Add the menu name to check the specifically the menu for which we add
  // this link.
  $item['menu_name'] = $form_state['object']['menu'];
  list(, $existing_menu) = _vsite_menu_prepare_menu_item($item, $vsite);

  if (strlen(serialize($existing_menu)) > 65000) {
    form_set_error('', t('You have reached the maximum number of items for this menu.'));
  }
}

/**
 * Multistep form page; presents title and advanced options for a URL link.
 */
function cp_menu_title($form, &$form_state) {
  $form_state['title'] = t('Title');

  // Can this be prepopulated if we're coming from an existing node?
  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#description' => t('Name your link or heading.'),
    '#required' => TRUE,
  );

  // Adds type dependent fields
  switch ($form_state['object']['type']) {
    case 'url':
      $form['new_node_type'] = array('#type' => 'hidden', '#value' => 0);
      $form['url'] = array(
        '#type' => 'textfield',
        '#title' => t('URL'),
        '#required' => TRUE,
        '#description' => t('The address of the link.'),
      );

      $form['#validate'][] = 'cp_menu_url_validate';
      break;
    case 'home':
      $form['new_node_type'] = array(
        '#type' => 'hidden',
        '#value' => 0
      );
      $form['url'] = array(
        '#type' => 'hidden',
        '#value' => '<front>',
      );
      break;
    case 'page':
      $form['new_or_select'] = array(
        '#type' => 'radios',
        '#title' => t('Destination'),
        '#options' => array('new' => t('New page'), 'select' => t('Choose existing')),
        '#description' => t('Choose a page you have already created or make a new page as the destination for your link'),
        '#required' => TRUE,
      );

      include_once (drupal_get_path('module', 'vsite') . '/includes/nodeautocomplete.inc');
      $autocomplete_element = array(
        '#title' => t('Page'),
        '#description' => t('Select an existing page.'),
        '#states' => array(
          'visible' => array(
            ':input[name="new_or_select"]' => array('value' => 'select'),
          ),
        ),
      );
      $form['node'] = vsite_node_autocomplete_element($autocomplete_element);

      // Load the bundles.
      $content_types = os_get_bundles();
      $content_types_list = array();
      if (module_exists('os_biocv')) {
        // Check for an already created bio.
        $bio_check = _os_biocv_bio_exists();
        // Check for an already created cv.
        $cv_check = _os_biocv_cv_exists();
        // Remove bio || cv if they're already created.
        if ($bio_check && array_key_exists('bio', $content_types)) {
          unset($content_types['bio']);
        }
        if ($cv_check && array_key_exists('cv', $content_types)) {
          unset($content_types['cv']);
        }
      }
      // Remove the "link" content type from the bundles array.
      if (array_key_exists('link', $content_types)) {
        unset($content_types['link']);
      }
      // Sort content types alphabetically.
      asort($content_types);
      // String replace for os_get_bundles, underscores should be hyphens.
      foreach($content_types as $key => $value) {
        $url_str = str_replace('_', '-', $key);
        $content_types_list[$url_str] = $value;
      }

      $form['new_node_type'] = array(
          '#type' => 'select',
          '#title' => t('Create new'),
          '#description' => t('Create a new page.'),
          '#options' => $content_types_list,
          '#states' => array('visible' => array(':input[name="new_or_select"]' => array('value' => 'new'),),),
      );


      break;
  }

  //advanced options present depending on site configuration
  if (variable_get('cp_menu_link_options_new_window', FALSE)) {
    $advanced['new_window'] = array(
      '#type' => 'checkbox',
      '#title' => t('New Window'),
      '#description' => t('Open link in new tab or window'),
      '#default' => FALSE,
    );
  };

  if (variable_get('cp_menu_link_options_tooltip', FALSE)) {
    $advanced['tooltip'] = array(
      '#type' => 'textfield',
      '#title' => t('Tooltip'),
      '#description' => t('Text displayed when mouse hovers over link'),
    );
  }

  if (isset($advanced)) {
    $form['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced Options'),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE
    ) + $advanced;
  }

  return $form;
}

/**
 * Validation callback for cp_menu_url; Checks that URL exists.
 */
function cp_menu_url_validate(&$form, &$state) {
  if ($state['values']['url'] == '<front>') {
    return;
  }

  // Remove preceding slashes from url
  $state['values']['url'] = ltrim($state['values']['url'], '/');

  // Remove the query string and fragments.
  list($url, ) = explode('?', $state['values']['url'],2);

  // Parses the URL into it's components.
  $components = parse_url($state['values']['url']);
  // Builds a string to add to the end of the normalized path.
  $extra = '';
  if (!empty($components['query'])) {
    $extra .= "?{$components['query']}";
  }
  if (!empty($components['fragment'])) {
    $extra .= "#{$components['fragment']}";
  }

  $url = check_url($url);

  $domains = array(variable_get('purl_base_domain'));
  if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
    $vsite_domain = variable_get('vsite_domain_name', FALSE);
    if ($vsite_domain) {
      $domains[] = 'http://' . $vsite_domain;
    }
  }

  //for internal urls, figure out if we're in the same vsite or an external vsite.
  if (isset($vsite_domain) && $vsite_domain) {
    $our_domain = 'http://' . (variable_get('vsite_domain_shared')) ? $vsite_domain . '/' . $vsite->og->purl . '/' : $vsite_domain;
  } else {
    $our_domain = variable_get('purl_base_domain') . '/' . $vsite->og->purl . '/';
  }
  $external_vsite = (strpos($url, $our_domain) !== 0);
  
  //absolute url
  if (url_is_external($url)) {
    $path = explode('/', $url);
    $domain = implode('/', array_slice($path, 0, 3));
    $original_path = implode('/', array_slice($path, 3));
    list($original_path, ) = explode('#', $original_path, 2);

    //external url
    if (!in_array($domain, $domains)) {
      $ch = curl_init($url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($ch, CURLOPT_HEADER, TRUE);
      curl_setopt($ch, CURLOPT_SSLVERSION, 3);
      $data = curl_exec($ch);
      $http_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);

      if($http_code == 404) {
        form_set_error('url', t('Could not connect to url: %url', array('%url' => $url)));
      }
      return;
    } else {
      //absolute path to internal url could be same vsite or external vsite
      if ($external_vsite) {
        $path = $url;
      } else {
        $path = ($original_path) ? $original_path : '<front>';
      }
    }
  } else {
    //relative url
    $path = $url;
  }

  list($path, ) = explode('#', $path, 2);
  $normal_path = drupal_get_normal_path($path);
  $item = menu_get_item($path);
  if ($normal_path == $path && !drupal_valid_path($normal_path)) {
    //prepend site purl if missing.
    if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
      $purl = $vsite->group->purl;

      if (strpos($path, $purl.'/') !== 0) {
        $path = $purl . '/' . $path;
        $normal_path = drupal_get_normal_path($path);
        $item = menu_get_item($normal_path);
      }
    }
  }

  //check if internal path exists or needs to be created.
  if (!$item && !drupal_valid_path($normal_path)) {
    if ($state['input']['new_node_type']) {
      //path does not exist but user has opted to create a new node there.
      $values = cp_menu_cache_get($state['object_id']) + $state['input'];
      $values['new_node_type'] = $state['input']['new_node_type'];
      $values['new_node_url'] = $path;
      cp_menu_cache_set($state['object_id'], $values);

      return;  //Node type will be handled by submit function, cp_menu_wizard_finish.
    } else {   
      //this path does not exist.  offer to create it by presenting list of available content types.
      if (!$external_vsite) {
        form_set_error('url', t('Path does not exist.  Please correct your URL or select a content type to create at %url', array('%url' => $url)));
        $fake_state = array('object'=> array('type' => 'page'));
        $cp_menu_title = cp_menu_title(array(), $fake_state);
        $form['new_node_type'] = array_merge($form['new_node_type'], $cp_menu_title['new_node_type']);
        $form['new_node_type']['#theme'] = 'select';
        $form['new_node_type']['#weight'] = $form['url']['#weight'] + 1;
        $form['new_node_type']['#value'] = $state['input']['new_node_type'];
        $form['new_node_type']['#prefix'] = '<div class="messages warning"><p>If you would like to create content at the URL you have entered, please choose a page type and click finish.</p>';
        $form['new_node_type']['#suffix'] = '</div>';
        array_unshift($form['new_node_type']['#options'], '-- Select Type --');
      }
    }
  } else {
    //path is valid.  use normal path instead of aliased path
    $values = array_merge(cp_menu_cache_get($state['object_id']), $state['input']);
    $values['url'] = "{$normal_path}{$extra}";
    cp_menu_cache_set($state['object_id'], $values);
  }
}

/**
 * Multistep form page; presents options for type of list to link to.
 *
 * @todo is this used?
 */
function cp_menu_list_type($form, &$formstate) {
  $form_state['title'] = t('Type of List');

  $form['list_type'] = array(
    '#title' => t('Type of List'),
    '#type' => 'radios',
    '#options' => array('publications' => 'Publications', 'taxonomy' => 'Taxonomy'),
    '#description' => t('Type of content to link to.'),
    '#required' => TRUE,
  );

  $form['publications_type'] = array(
    '#type' => 'radios',
    '#title' => 'Publications by ...',
    '#options' => array('year' => 'Year', 'type' => 'Type', 'co' => 'Co-author'),
    '#states' => array(
      'visible' => array(
        ':input[name="list_type"]' => array('value' => 'publications'),
      ),
    ),
  );

  // Is there a vsite aware version of this?
  $options = array();
  foreach (taxonomy_get_vocabularies() as $vid => $vocab) {
    $options[$vocab->vid] = $vocab->name;
  }

  if (count($options)) {
    $form['vocabulary'] = array(
      '#type' => 'radios',
      '#title' => 'Vocabulary',
      '#options' => $options,
      '#states' => array(
        'visible' => array(
          ':input[name="list_type"]' => array('value' => 'taxonomy'),
        ),
      ),
    );
  }
  else {
    $form['taxonomy_not_found'] = array(
      '#type' => 'item',
      '#title' => 'No vocabularies found.  Please create a vocabulary or choose another type of list.',
      '#states' => array(
        'visible' => array(
          ':input[name="list_type"]' => array('value' => 'taxonomy'),
        ),
      ),
    );
  }

  return $form;
}

/**
 * Validation callback for cp_menu_list_type.
 *
 * Publication lists need a type (year, co-auth, etc) and taxonomy lists need a
 * vocabulary. FAPI #states weren't quite up to the task of imposing conditional
 * requirements.
 */
function cp_menu_list_type_validate(&$form, &$formstate) {
  if ($formstate['values']['list_type'] == 'publications' && strlen($formstate['values']['publications_type']) == 0) {
    form_set_error('publications_type', t('Publication list selected, but missing publication type.'));
  }
  if ($formstate['values']['list_type'] == 'taxonomy' && strlen($formstate['values']['vocabulary']) == 0) {
    form_set_error('vocabulary', t('Taxonomy list selected, but missing vocabulary.'));
  }
}

/**
 * Multistep form page; presents available terms to display in a vocab list.
 *
 * @todo is this used?
 */
function cp_menu_list_term($form, &$formstate) {
  $vid = $formstate['object']['vocabulary'];
  $terms = taxonomy_get_tree($vid, 0);

  $options = array();
  foreach ($terms as $term) {
    $options[$term->tid] = $term->name;
  }

  $form['term'] = array(
    '#title' => t('Term'),
    '#type' => 'radios',
    '#options' => $options,
  );

  return $form;
}

/**
 * CP menu cache storage
 */
function cp_menu_cache_set($id, $object) {
  ctools_include('object-cache');
  ctools_object_cache_set('cp_menu', $id, $object);
}

/**
 * CP menu cache retrieval
 */
function cp_menu_cache_get($id) {
  ctools_include('object-cache');
  $object = ctools_object_cache_get('cp_menu', $id);
  return ($object) ? $object : array();
}

/**
 * Clear the wizard cache.
 */
function cp_menu_cache_clear($id) {
  ctools_include('object-cache');
  ctools_object_cache_clear('cp_menu', $id);
}

/**
 * Advance the cp menu wizard to the next page.  Concatenate cached and submitted values
 */
function cp_menu_wizard_next(&$form_state) {
  $form_state['object'] += $form_state['values'];
  cp_menu_cache_set($form_state['object_id'], $form_state['object']);
}

/**
 * CP menu wizard cancelled.
 */
function cp_menu_wizard_cancel(&$form_state) {
  $cached = cp_menu_cache_get($form_state['object_id']);
  cp_menu_cache_clear($form_state['object_id']);

  if ($form_state['ajax'] || $cached['js']) {
    ctools_include('modal');
    ctools_include('ajax');

    $commands = array();
    $commands[] = ctools_modal_command_dismiss();
    print ajax_render($commands);
    exit;
  }
  else {
    overlay_commands_add(overlay_command_dismiss());
  }
}

/**
 * CP menu wizard has completed.  Add results to a menu.
 */
function cp_menu_wizard_finish(&$form_state) {
  $values = cp_menu_cache_get($form_state['object_id']) + $form_state['values'];
  form_clear_error();

  // Provides node add form
  if (($values['type'] == 'page' && $values['new_or_select'] == 'new') || ($values['type'] == 'url' && isset($values['new_node_url']))) {
    cp_menu_cache_set($form_state['object_id'], $values);
    if (!empty($form_state['ajax']) || !empty($values['js'])) {
      ctools_include('modal');
      ctools_include('ajax');
      $commands = array();

      $commands[] = ctools_ajax_command_redirect('node/add/' . $values['new_node_type']);
      print ajax_render($commands);
      exit;
    }
    else {
      $node_type = ($values['new_node_type'] != 'media_gallery') ? $values['new_node_type'] : 'media-gallery';
      drupal_goto('node/add/' . $node_type);
    }
  }

  $options = array();

  // Builds the link from the collected data
  switch ($values['type']) {
    case 'home':
    case 'url':
      // Parse out the query string and fragments.
      $query = parse_url($values['url'], PHP_URL_QUERY);
      $fragment = parse_url($values['url'], PHP_URL_FRAGMENT);
      $url = $values['url'];

      if (strlen($query)) {
        $options['query'] = array();
        // Parse the string to an array.
        parse_str($query,$options['query']);
        // Removes query string.
        list($url,) = explode('?', $url,2);
      }

      if (strlen($fragment)) {
        $options['fragment'] = $fragment;
        // Removes fragment from URL;
        list($url,) = explode('#', $url, 2);
      }

      $url = ($values['url'] == '<front>') ? '<front>' : check_url($url);
      break;

    case 'page':
      if (isset($form_state['vsite_autocomplete_nid'])) {
        $nid = $form_state['vsite_autocomplete_nid'];
      }
      $url = 'node/'.$nid;
      break;

    case 'heading':
      $url = false;
      $options['attributes']['class'][] = "menu-heading";

    case 'list':
      break;

    default:
      return;
  }

  $menu_name = isset($values['menu']) ? $values['menu'] : 'primary-menu';

  if (isset($values['new_window']) && $values['new_window']) {
    $options['attributes']['target'] = '_blank';
  }

  if (isset($values['tooltip']) && $values['tooltip']) {
    $options['attributes']['title'] = $values['tooltip'];
  }

  // Saves to vsite or whole site
  ctools_include('menu', 'os');

  $item = array('link_path' => $url, 'menu_name' => $menu_name, 'link_title' => $values['title'], 'options' => $options);
  if ($values['type'] == 'heading') {
    //If we don't have a path set, tell drupal that this link is external
    //so that it does not try and run access functions
    $item['external'] = true;
  }
  $mlid = os_menu_link_save($item);
  cp_menu_cache_clear($form_state['object_id']);

  //ctools_ajax_command_reload();
  if (!empty($form_state['ajax']) || !empty($values['js'])) {
    ctools_include('modal');
    ctools_include('ajax');

    $commands   = array();
    $commands[] = ctools_modal_command_dismiss();
    $commands[] = ctools_ajax_command_reload();
    print ajax_render($commands);
    exit;
  }
  else {
    overlay_commands_add(overlay_command_dismiss());
    overlay_close_dialog();
  }
}

