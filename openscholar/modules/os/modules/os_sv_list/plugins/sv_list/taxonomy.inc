<?php

/**
 * @file taxonomy.inc
 *
 * taxonomy plugin for sv_list box.  let's try and keep this applicable to files AND nodes
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_taxonomy'),
  'entity_type' => array(),
  'bundle' => array(),
);

class sv_list_taxonomy extends sv_list_plugin {
  public function options_defaults() {
    //@todo document these.  I don't know what they all mean, especially the only_ vars
    return array(
      'vocabs' => array(),
      'terms' => array(),
      'only_vocabs' => TRUE,
      'only_vocabs_values' => array(),
    );
  }

  public function options_form($options, &$form) {
     // Get all relevant vocabularies.
    //$vocabs = os_sv_list_get_content_type_vocabs($options['content_type'], $this->entity_type); //@TODO does this func work with files terms too?
    $vocabs = $this->_os_sv_list_vid_by_bundle();

    if (empty($vocabs)) {
      return;
    }

    // The 'vocabs' tree will contain select fields - one for each vocabulary.
    $vocab_form = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => 'Filter by Vocabulary',
      '#description' => t('Only include posts tagged with terms from the following vocabularies.'),
      '#tree' => TRUE,
      '#attached' => array(
        'css' => array(libraries_get_path('select2') . '/select2.css'),
        'js' => array(
          array('type' => 'setting', 'data' => array('sv_list_vocab_bundles' => $vocabs)),
          libraries_get_path('select2') . '/select2.js',
        )
      ),
    );

    // Create a select field for each vocabulary, with the vocabulary's terms as the field's options.
    foreach (array_keys($vocabs) as $vid) {
      $vocabulary = taxonomy_vocabulary_load($vid);

      // Get the vocabulary's terms.
      $terms = array();
      foreach (taxonomy_get_tree($vid) as $item) {
        $term = taxonomy_term_load($item->tid);
        $terms[$term->tid] = $term->name;
      }

      $element = 'vocab_' . $vid;
      $vocab_default_value = (is_array($options['vocabs']) && isset($options['vocabs'][$element])) ? $options['vocabs'][$element] : array();

      // Backwards compatibility, some widgets dont have $options['vocabs'] populated.
      if(!is_array($options['vocabs']) || !$options['vocabs']) {
        $vocab_default_value = array_intersect(array_keys($terms), $options['terms']);
      }

      // Create select field with the terms as options.
      $vocab_form[$element] = array(
        '#type' => 'select',
        '#options' => $terms,
        '#title' => $vocabulary->name,
        '#multiple' => TRUE,
        '#default_value' => $vocab_default_value,
        '#element_validate' => array('os_sv_list_terms_element_validate'), //group selected terms from all vocab fields
      );
    }

    $form['vocabs'] = $vocab_form;
  }

  /**
   * @function query_alter
   *
   * Limit the query's scope to items in this subsite.
   */
  public function query_alter($options, $efq) {
    // For all Queries but image, filter out based on vocab.
    if ($options['terms'] && $options['terms'] != -1) {
      $ids = $this->tagged_entities($options);
      if (is_array($ids)) {
        $entity_info = entity_get_info($this->entity_type);
        $table = $entity_info['base table'];
        $id = $entity_info['entity keys']['id'];

        $efq->propertyCondition($id, $ids, 'IN');
      }
    }
  }

  /**
   * @function tagged_nodes
   *
   * Return the nodes attached to terms of the selected vocabulary.
   */
  private function tagged_entities($options) {
    // Get the  terms the user selected.
    $tids = (is_array($options['terms'])) ? $options['terms'] : drupal_map_assoc($options['terms']);

    // No terms were found.
    if (empty($tids)) {
      return;
    }

    $bundles = os_get_bundles(array(1)) + os_files_get_bundles();
    $content_types = ($options['content_type'] == 'all') ? array_keys($bundles) : array($options['content_type']);

    $query = db_select('field_data_og_vocabulary', 'v')
      ->fields('v', array('entity_id'))
      ->condition('v.entity_type', $this->entity_type)
      ->condition('v.bundle', $content_types, 'IN');

    // Get vocabs by bundle.
    $vocabs_by_bundle = $this->_os_sv_list_vid_by_bundle();

    //group tids by vocab.
    $vocabs = array_reduce(taxonomy_term_load_multiple($tids), function ($ret, $term) {
      $ret[$term->vid][] = $term->tid;
      return $ret;
    }, array());

    //each entity must match a term from each specified vocab.  getting a `tid IN $tids` could potentially ignore a vocab.
    foreach ($vocabs as $vid => $terms) {
      // Only include terms from vocabs that have selected entity types.
      if (!array_intersect($vocabs_by_bundle[$vid], $content_types)) {
        continue;
      }

      $alias = 'vid' . $vid;
      $query->innerJoin('field_data_og_vocabulary', $alias, "$alias.entity_id = v.entity_id");
      $query->condition("$alias.og_vocabulary_target_id", $terms, 'IN');
    }

    $result = $query->execute();
    return ($result->rowCount()) ? $result->fetchCol() : array(0); //needs a nonempty array or we get db errors
  }

  /*
   * @function _os_sv_list_vid_by_bundle()
   *
   * Gets all vocab vids in this vsite and the bundles that use them
   */
  private function _os_sv_list_vid_by_bundle() {
    $query = db_select('og_vocab_relation', 'ogr')
      ->fields('ogr', array('vid'));

    if (module_exists('spaces') && $space = spaces_get_space()) {
      $query->condition('gid', $space->id);
    }

    $query->join('og_vocab', 'ogv', 'ogr.vid = ogv.vid');
    $query->condition('ogv.entity_type', $this->entity_type);

    $query->groupBy('ogv.vid');
    $query->addExpression('GROUP_CONCAT(ogv.bundle)', 'bundles');
    $result = $query->execute();

    $ret = array();
    while ($row = $result->fetchObject()) {
      $ret[$row->vid] = explode(',', $row->bundles);
    }

    return $ret;
  }
}
