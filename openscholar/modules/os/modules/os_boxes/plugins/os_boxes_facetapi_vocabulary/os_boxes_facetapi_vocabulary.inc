<?php

class os_boxes_facetapi_vocabulary extends os_boxes_default {

  /**
   * Create a new box.
   */
  protected function __construct() {
    parent::__construct();
    $this->title = t('Filter by taxonomy');
  }

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();
    $options['bundle'] = FALSE;
    $options['vocabularies'] = array();
    $options['sort_by'] = 'alpha';
    $options['sort_by_term'] = 'rel';
    $options['show_count'] = TRUE;
    $options['as_nav'] = FALSE;

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    $bundle = $this->options['bundle'];
    $selected_vocab = $this->options['vocabularies'];
    $sort_by = $this->options['sort_by'];
    $sort_by_term = $this->options['sort_by_term'];
    $as_nav = $this->options['as_nav'];
    $show_count = $this->options['show_count'];

    if (!empty($form_state['values']['bundle'])) {
      // We have the bundle from the AJAX.
      $bundle = $form_state['values']['bundle'];
      $selected_vocab = 'all';
      $sort_by = $form_state['values']['sort_by'];
      $sort_by_term = $form_state['values']['sort_by_term'];
      $as_nav = $form_state['as_nav'];
      $show_count = $form_state['show_count'];
    }

    $bundles = os_get_bundles(array(OS_PUBLIC_APP, OS_PRIVATE_APP));
    // Use first bundle of the list in case none was selected before.
    if (!$bundle) {
      $bundle = reset(array_keys($bundles));
    }

    $form['bundle'] = array(
      '#type' => 'select',
      '#description' => t('Which bundle would you like to use.'),
      '#title' => t('Content type'),
      '#options' => $bundles,
      '#required' => TRUE,
      '#default_value' => $bundle,
      '#ajax' => array(
        'callback' => 'os_boxes_facetapi_vocabulary_ajax_callback',
        'wrapper' => 'vocab-wrapper',
      ),
    );

    $form['vocab_wrapper'] = array(
      '#prefix' => '<div id="vocab-wrapper">',
      '#suffix' => '</div>',
      '#parents' => array('vocab_wrapper'),
    );

    $vocabs = os_search_solr_get_bundle_vocabularies($bundle);
    $options = array('all' => t('All vocabularies')) + $vocabs;

    $form['vocab_wrapper']['vocabularies'] = array(
      '#title' => t('Select vocabularies'),
      '#type' => 'select',
      '#options' => $options,
      '#multiple' => TRUE,
      '#default_value' => $selected_vocab,
    );

    $options_vocab = array(
      'alpha' => t('Alphabetical'),
      'weight' => t('Vocabulary weight')
    );

    $options_term = array(
      'rel' => t('Relevance'),
      'alpha' => t('Alphabetical'),
      'weight' => t('Term Weight')
    );

    $form['sort_by'] = array(
      '#title' => t('Vocabulary Order:'),
      '#type' => 'select',
      '#options' => $options_vocab,
      '#default_value' => $sort_by,
    );

    $form['sort_by_term'] = array(
      '#title' => t('Term Order:'),
      '#type' => 'select',
      '#options' => $options_term,
      '#default_value' => $sort_by_term,
    );

    // Extra option added like taxonomy widget
    $form['show_count'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show number of posts'),
      '#description' => t('Show number of posts tagged to a term next to the term.'),
      '#default_value' => $show_count,
    );

    $form['as_nav'] = array(
      '#type' => 'radios',
      '#title' => t('Display Styles'),
      '#default_value' => empty($as_nav) ? 0 : $as_nav,
      '#options' => array(
        0 => t('Classic/List'),
        1 => t('Menu'),
        2 => t('Collapsible Accordion')
      ),
    );

    return $form;
  }

  /**
   * Check whether an empty Solr search is needed.
   *
   * @return $browse_page
   *   Solr search page id in case empty search is needed, FALSE otherwise.
   */
  public function is_empty_search_needed() {
    if (!$bundle = $this->options['bundle']) {
      return;
    }

    $page_id = 'browse_' . $bundle;
    if (!$search_page = apachesolr_search_page_load($page_id)) {
      return;
    }
    $env_id = $search_page['env_id'];

    if (apachesolr_has_searched($env_id)) {
      // Search was executed already.
      return;
    }
    return $page_id;
  }

  /**
   * Facet widgets that in a non-solr search page must fake a search page in
   * order to show the facet. To do so we execute a "silent" search on solr
   * without showing the results.
   */
  public function call_empty_search_if_needed() {
    if (!$page_id = $this->is_empty_search_needed()) {
      return;
    }

    // Execute the query.
    try {
      if (!$search_page = apachesolr_search_page_load($page_id)) {
        throw new Exception("Failed to load search page information $search_page");
      }
      $env_id = $search_page['env_id'];

      if (apachesolr_has_searched($env_id)) {
        // Search was executed already.
        return;
      }
      $solr = apachesolr_get_solr($env_id);

      $conditions = apachesolr_search_conditions_default($search_page);

      // Initializes params for empty query.
      $params = array(
        'spellcheck' => 'false',
        'fq' => isset($conditions['fq']) ? $conditions['fq'] : array(),
        'rows' => 1,
      );
      $context['page_id'] = $page_id;
      $context['search_type'] = 'apachesolr_search_show_facets';
      apachesolr_search_run_empty('apachesolr', $params, $search_page['search_path'], $solr, $context);

      // Returning page id means there was an empty search, and we will cache the
      // block.
      return $page_id;
    }
    catch (Exception $e) {
      watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    }
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    $block = parent::render();
    $searcher_name = os_search_solr_get_searcher_name();
    // Gets delta map
    if (!$map = facetapi_get_delta_map()) {
      return $block;
    }

    $delta = $block['delta'];

    // Filter by OG vocab.
    $string_og_vocab = facetapi_build_delta($searcher_name, 'block', 'sm_og_vocabulary');
    $delta_og_vocab = array_search($string_og_vocab, $map);

    if ($this->options['bundle'] && is_string($this->options['bundle'])) {
      $bundle = 'node:' . $this->options['bundle'];
    }
    else {
      $bundle = NULL;
    }
    $cid = $this->set_cache_id($bundle);

    if ($page_id = $this->is_empty_search_needed()) {
      // Block delta is needed since $delta_og_vocab is not unique between sites
      $cid = $this->cid = $cid .= ':' . $page_id;

    }
    // Verify the cache is still valid, based on time since created.
    // Verify the facet block has something to work on (means - it is in a solr
    // search page). If not - execute the empty solr search without showing
    // the results.
    // If empty search happened, we will cache the block.
    $use_cache = $this->call_empty_search_if_needed();
    if (!$use_cache || (($block = $this->get_cache()) === FALSE)) {
      $block = module_invoke('facetapi', 'block_view', $delta_og_vocab);
      $block['delta'] = $delta;

      // Set block subject/title from box configuration.
      $block['subject'] = check_plain($this->title);
      $block['title'] = check_plain($this->title);

      if (empty($block['content'])) {
        // Block content must be defined, even if it is empty, to avoid errors.
        // os_boxes_widget_submit() renders blocks into a 'dummy' region, but
        // gives errors in case the content is empty, hence the space.
        $block['content'] = ' ';

        // While block is empty, caching NULL will provide the needed caching.
        if ($use_cache) {
          $this->set_cache(NULL);
        }
      }
      else {
        if (!empty($this->options['vocabularies'])) {
          // Get the list of selected vocabularies
          $selected = array();
          foreach ($this->options['vocabularies'] as $vocabulary_name => $value) {
            if ($value == "all") {
              $selected = array();
              break;
            }
            if ($value) {
              $selected[$vocabulary_name] = $value;
            }
          }
          // If no selection - show all. If there is selection - show only them.
          if (count($selected)) {
            foreach (array_keys($block['content']['sm_og_vocabulary']) as $vocabulary_name) {
              if (empty($selected[$vocabulary_name])) {
                unset($block['content']['sm_og_vocabulary'][$vocabulary_name]);
              }
            }
          }
        }

        if (!empty($block['content']['sm_og_vocabulary'])) {
          // Sort by the Vocabulary weight if that option is selected.
          if ($this->options['sort_by'] == 'weight') {
            $query = new EntityFieldQuery();
            $results = $query->entityCondition('entity_type', 'taxonomy_vocabulary')
              ->propertyCondition('machine_name', array_keys($block['content']['sm_og_vocabulary']), 'IN')
              ->propertyOrderBy('weight')
              ->execute();

            $ordered_vocab = array();
            foreach($results['taxonomy_vocabulary'] as $vid) {
              $vocab = taxonomy_vocabulary_load($vid->vid);
              // Take the existing content and reorder it based on the query.
              $ordered_vocab[$vocab->machine_name] = $block['content']['sm_og_vocabulary'][$vocab->machine_name];
            }
            $block['content']['sm_og_vocabulary'] = $ordered_vocab;
          }
          else {             
            // Default is to sort by term relavance.
            ksort($block['content']['sm_og_vocabulary']);
          }

          // Checking term render option for relevance/alphabetical/weighted sorting of links.
          // Default term sorting order would be ordered by relevancy.
          if ($this->options['sort_by_term'] == 'alpha') {
            // Term sorting by alphabetical order.
            foreach (array_keys($block['content']['sm_og_vocabulary']) as $vocabulary_name) {
              array_sort_by_column($block['content']['sm_og_vocabulary'][$vocabulary_name]['#items'], 'data');
            }
          } elseif ($this->options['sort_by_term'] == 'weight') {
            // Term sorting by weight.
            foreach (array_keys($block['content']['sm_og_vocabulary']) as $vocabulary_name) {
              array_sort_by_term_weight($block['content']['sm_og_vocabulary'][$vocabulary_name]['#items'], $vocabulary_name);
            }
          }
        }        
        // Initializing jQuery UI Accordion for this widget instance if display style is set as "Collapsible"
        if ($this->options['as_nav'] == 2) {
          $block['content']['#prefix'] = '<div class="accordion">';
          $block['content']['#suffix'] = '</div>';
          $block['content']['#attached'] = array(
            'library' => array(
              array('system', 'ui.accordion'),
            ),
            'js' => array(
              drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_facetapi_vocabulary/os_boxes_facetapi_vocabulary_accordion.render.js',
              array(
                'type' => 'setting',
                'data' => array(
                  'os_boxes' => array(
                    'faceted_taxonomy' => array(
                      $this->delta => array(
                        'delta' => $this->delta,
                      )
                    )
                  )
                )
              ),
            ),
          );
        }

        $block['content'] = render($block['content']);

        $additional_classes = array();
        if ($this->options['show_count'] == FALSE) {
          $additional_classes[] = 'hide-count';
        }
        if ($this->options['as_nav'] == 1) {
          $additional_classes[] = 'navigation';
        } elseif ($this->options['as_nav'] == 2) {
          $additional_classes[] = 'collapsible';
        }

        if (!empty($additional_classes)) {
          $this->options['additional_classes'] = implode(" ", $additional_classes);
        }

        if ($use_cache) {
          $this->set_cache($block);
        }
      }
    }

    return $block;
  }

  /**
   * When submitting a box we need to invalidate here.
   */
  public function options_submit($form, $form_state) {
    if (module_exists('vsite') && !$vsite = vsite_get_vsite()) {
      return;
    }

    // We try to delete the cache of a newly selected bundle, leaving the old cached
    // info unused. If we will re-select the old bundle its cache will be cleared
    // next time.
    if ($this->options['bundle'] && is_string($this->options['bundle'])) {
      $bundle = 'node:' . $this->options['bundle'];
    }
    else {
      $bundle = NULL;
    }
    $cid = $this->set_cache_id($bundle);

    cache_clear_all($cid, 'cache_os_boxes', TRUE);
    cache_clear_all('assets:' . $cid, 'cache_os_boxes', TRUE);
  }
}

/**
 * AJAX callback to attach the vocab to the form.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 */
function os_boxes_facetapi_vocabulary_ajax_callback($form, &$form_state) {
  return $form['options']['vocab_wrapper'];
}

/**
 * Function to sort vocabulary terms alphabetically.
 *
 * @param $arr
 *   The reference to the actual array element that needs to be sorted.
 * @param $col
 *   To define target column name of multi-dimentional array for sorting.
 * @param $dir
 *   To define sorting order.
 */
function array_sort_by_column(&$arr, $col, $dir = SORT_ASC) {
  $sort_col = array();
  foreach ($arr as $key => $row) {
    $sort_col[$key] = strip_tags($row[$col]);
  }
  array_multisort($sort_col, $dir, $arr);
}

/**
 * Function to sort vocabulary terms by weight.
 *
 * @param $arr
 *   The reference to the actual array element that needs to be sorted.
 * @param $vocabulary
 *   The name of the vocabulary.
 * @param $dir
 *   To define sorting order.
 */
function array_sort_by_term_weight(&$arr, $vocabulary, $dir = SORT_ASC) {
  $vocab = taxonomy_vocabulary_machine_name_load($vocabulary);
  $term_tree = taxonomy_get_tree($vocab->vid);
  $ordered_term = array();
  
  // Preparing ordered array for taxonomy tree.
  foreach ($term_tree as $key => $term) {
    $ordered_term[$term->name] = $key;
  }

  // Iterating through target taxonomy term array.
  foreach ($arr as $key => $row) {
    $matches = array();
    // Filtering the term name from string coming from ApacheSolr and FacetAPI.
    preg_match("/Apply(.*?)filter/s", $row['data'], $matches);
    $term = taxonomy_get_term_by_name($matches[1], $vocabulary);
    $keys = array_keys($term);
    // Obtaining defined weight of the taxonomy term.
    $arr[$key]['weight'] = $ordered_term[$term[$keys[0]]->name];
  }

  // Sorting array by taxonomy weight.
  array_sort_by_column($arr, 'weight');
}