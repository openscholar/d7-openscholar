<?php

class os_boxes_breadcrumbs extends os_boxes_default {

  public function options_defaults() {
  	$options = parent::options_defaults();
	$options += array(
      'show_home' => FALSE,
	  'breadcrumb_divider' => 0,
	);
    return $options;
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    $radio_options = array(
      0 => '&raquo;',
      1 => '&#92;',
      2 => '&#47;',
      3 => '&#124;',
      4 => '-',
    );

    $form['breadcrumb_divider'] = array(
      '#type' => 'radios',
      '#title' => t('Breadcrumb Divider'),
      '#id' => 'breadcrumb_dividers',
      '#options' => $radio_options,
      '#default_value' => isset($this->options['breadcrumb_divider']) ? $this->options['breadcrumb_divider'] : 0,
      '#attributes' => array('class' => array('container-inline')),
    );

    $form['show_home'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show Breadcrumbs on your homepage'),
      '#default_value' => isset($this->options['show_home']) ? $this->options['show_home'] : FALSE,
      '#id' => 'show_home_check',
    );

    return $form;
  }

  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }
    $block = parent::render();
    $options = $this->options;
    $links = array();
    $path = '';
    $path_boolean = TRUE;

    $breadcrumbs_options = array(
      0 => '&raquo;',
      1 => '&#92;',
      2 => '&#47;',
      3 => '&#124;',
      4 => '-',
    );
    $space = spaces_get_space();
    // Get URL arguments.
    $arguments = explode('/', request_uri());

    // Check for vsites
    if(isset($space)) {
      // Check for the vsite name in the url.
      // Unset it if it exists.
      // Unsetting because the breadcrumb would look like this Home > VsiteName > Page Title.
      // Removing the vsite name removes it from the breadcrumb, so it will just be Home > Page Title.
      if($key = array_search($space->group->title, $arguments)) {
      	unset($arguments[$key]);
      }
    }

    // Remove empty values from the uri.
    foreach ($arguments as $key => $value) {
      if (empty($value)) {
        unset($arguments[$key]);
      }
    }
    $arguments = array_values($arguments);

    // Add 'Home' link
    $links[] = l(t('Home'), '<front>');

    // Setup the breadcrumb paths.
    if (!empty($arguments)) {
      // First, check for taxonomy and handle taxonomy properly.
      // We don't want home > people > taxonomy > terms > termid to be displayed in the breadcrumbs.
      if(strpos(request_uri(),'/term/') !== FALSE && strpos(request_uri(),'/cp/') === FALSE) {
      	// Load the newly built taxonomy breadcrumbs.
      	$arguments = $this->_os_boxes_breadcrumbs_build_taxnomy_menu($arguments);
      }

      // Check for sub-sections.
      if ($node = menu_get_object()) {
        $current_title = $node->title;
      	// only pages can have section nav
        if ($node->type == 'page' && isset($node->book['bid'])) {
          // With the new subsections, each subsection will still have a url base of
          // pages/subsectionname, the breadcrumb url needs to preserve this.
          $path_boolean = FALSE;
          $path_preserve = $arguments[0];
		  // Load the newly built subsection breadcrumbs.
		  $arguments = $this->_os_boxes_breadcrumbs_build_subsection_menu($arguments,$current_title);
        }
      }

      // Build the breadcrumbs off the constructed arguments array.
      foreach ($arguments as $key => $value) {
        // Don't make last breadcrumb a link
        if ($key == (count($arguments) - 1)) {
          // Check for a page title.
          $page_title = drupal_get_title();
          $links[] = !empty($page_title) ? $page_title : drupal_ucfirst($value);
        } else {
          // Check for subsection url  preservation.
          if($path_boolean == FALSE) {
          	$path = $path_preserve . '/' . $value;
          }elseif (!empty($path)) {
            $path .= '/'. $value;
          } else {
            $path .= $value;
          }
          $links[] = l(drupal_ucfirst($value), $path);
        }
      }
    }

    // Set custom breadcrumbs
    drupal_set_breadcrumb($links);
    // Get custom breadcrumbs
    $breadcrumbs = drupal_get_breadcrumb();

    // Show the breadcrumbs on the homepage if the user opts for it.
    if(drupal_is_front_page() && $options['show_home']) {
      $block['content'] = '<div class="breadcrumb">'. implode(' '.$breadcrumbs_options[$options['breadcrumb_divider']].' ', $breadcrumbs) .'</div>';
    }

    // Hide breadcrumbs if only 'Home' exists
    if (count($breadcrumbs) > 1) {
      $block['content'] = '<div class="breadcrumb">'. implode(' '.$breadcrumbs_options[$options['breadcrumb_divider']].' ', $breadcrumbs) .'</div>';
    }

    return $block;
  }

  /**
   * Function to reconstruct the breadcrumbs for taxonomy.
   */
  private function _os_boxes_breadcrumbs_build_taxnomy_menu($arguments) {
    // Preserve the parent content type.
    $content_type = $arguments[0];
	// Load the term id.
    $tid = end($arguments);
    if(isset($tid)) {
      // Load the term name.
      $term_load = taxonomy_term_load($tid);
      $term_name = isset($term_load) ? $term_load->name : '';
      // Load the parents of this term.
      $term_hierarchy = taxonomy_get_parents_all($tid);
 	  // Rebuild the array for the links.
      if(!empty($term_hierarchy)) {
        // Reverse the order of the terms array.
        $terms_ordered = array_reverse($term_hierarchy);
		// Empty the array.
        unset($arguments);
        // Rebuild the array.
        // Preserved content_type first.
        if(isset($content_type)) {
          $arguments[] = $content_type;
        }
        // Parent Vocab.
        $parent_vocab = $terms_ordered[0]->vocabulary_machine_name;
        if(isset($parent_vocab)) {
      	  $arguments[] = $parent_vocab;
        }
        foreach($terms_ordered as $term_name) {
          $arguments[] = $term_name->name;
        }
      }
    }
    return $arguments;
  }

  /**
   * Function to reconstruct the breadcrumbs for pages sub-sections
   */
  private function _os_boxes_breadcrumbs_build_subsection_menu($arguments,$current_title) {
  	// Grab all the menu items associated with this node.
    $menu_active_trail = menu_get_active_trail();
    if(isset($menu_active_trail)) {
      // Empty the originial arguments array
      unset($arguments);
      // Get the last menu item.
      $last_menu = end($menu_active_trail);
      // Flatten the menu based off the last menu item.
      $flat_menu = book_get_flat_menu($last_menu);
      // Rebuild the array of arguments for the breadcrumbs.
      foreach($flat_menu as $menu_search) {
        if($menu_search['link_title'] == $current_title) {
    	  break;
      	}else {
      	  // Lookup the drupal alias for the link path.
   		  $drupal_path = drupal_lookup_path('alias',$menu_search['link_path']);
   		  // Get the url alias for the link.
    	  $path_explode = explode('/', $drupal_path);
    	  $menu_path = end($path_explode);
    	  $arguments[] = $menu_path;
        }
      }
      // Push the end section.
      $arguments[] = $current_title;
    }
    return $arguments;
  }
}

