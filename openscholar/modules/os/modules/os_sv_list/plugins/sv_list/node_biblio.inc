<?php 

/**
 * @file node_biblio.inc
 * 
 * Biblio plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_biblio'),
  'entity_type' => array('node'),
  'bundle' => array('biblio'),
);

class sv_list_node_biblio extends sv_list_plugin {
  public function __construct($entity_type, $bundle) {
    parent::__construct($entity_type, $bundle);
    $this->biblio_types = $this->_biblio_types();
  }
  
  public function options_defaults() {
    return array(
      'biblio_whitelist' => drupal_map_assoc(array_keys($this->biblio_types)), 
    );
  }
  
  /**
   * @function register_sorts()
   * 
   * Adds a custom sort for this entity/bundle
   */
  public function register_sorts() {
    return array('sort_biblio' => t('Year of Publication'));
  }
  
  public function options_form($box_options, &$form) {
    $form['content']['biblio_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Publication Types'),
      '#collapsed' => TRUE, 
      '#collapsible' => TRUE,
      'biblio_whitelist' => array(
        '#type' => 'checkboxes',
        '#title' => t('Select publication types to display'),
        '#default_value' => $box_options['biblio_whitelist'], //isset($this->options['biblio_whitelist']) ? $this->options['biblio_whitelist'] : $defaults['biblio_whitelist'],
        '#options' => $this->biblio_types,
        '#prefix' => '<div id="os_sv_list_biblio_whitelist">',
        '#suffix' => '</div>',
        '#attached' => array(
          'js' => array(
            drupal_get_path('module', 'os_boxes').'/misc/selectAll.js',
          ),
        ),
      ),
      '#states' => array(
        'visible' => array(
          '#os_sv_list_content_type' => array('value' => 'biblio'),
        ),
        'invisible' => array(
          '#os_sv_list_content_type' => array('!value' => 'biblio'),
        ),
      )
    );
  }
  
  
  private function _biblio_types() {
    $rows = db_select('biblio_types', 'bt')
      ->fields('bt', array('tid', 'name'))
      ->condition('tid', 0, '>')
      ->condition('visible', 1)
      ->execute();
    $options = array('all' => 'Select All');
    foreach ($rows as $r) {
      $options[$r->tid] = $r->name;
    }
  
    return $options;
  }
  
  /**
   * @function query_alter 
   * 
   * Adds a filter to only use the selected biblio types.
   */
  public function query_alter($options, $efq) {
    $types = $this->_biblio_types();
    $whitelist = array_filter($options['biblio_whitelist']);
    
    if (empty($whitelist) || $options['content_type'] != 'biblio') {
      return;
    }
    
    if (count($types) > count($whitelist) && !in_array('all', $whitelist)) {
      $query = db_select('biblio', 'b')
        ->condition('biblio_type', array_values($whitelist), 'IN')
        ->fields('b', array('nid'));
      $query->innerJoin ('node', 'n', 'b.nid = n.nid AND n.vid = b.vid'); // filter out
      if ($vsite = vsite_get_vsite()) {
        $query->innerJoin('og_membership', 'ogm', "ogm.etid = b.nid AND ogm.entity_type = 'node'");
        $query->condition('ogm.gid', $vsite->group->nid);
      }
      $nids = $query->execute()
        ->fetchCol();

      if (!empty($nids)) {
        $efq->entityCondition ('nid', $nids, 'IN');
      }
    }
  }
  
  public function sort_biblio($options, $efq) {
    $efq->addTag('sv_list_biblio');
  }
}
