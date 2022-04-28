<?php 

/**
 * @file subsites.inc
 * 
 * subsites plugin for sv_list box.  let's try and keep this applicable to files AND nodes
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_subsites'),
  'entity_type' => array(),
  'bundle' => array(),
);

class sv_list_subsites extends sv_list_plugin  {
  
  public function options_defaults() {
    if (module_exists('vsite') && $space = spaces_get_space()) {
      return array('query_scope' => array($space->id => $space->id));
    }
  }
  
  public function options_form($options, &$form) {
    if (!module_exists('vsite') || ($vsite = spaces_get_space()) == FALSE) {
      return;
    }
    
    if (!in_array($vsite->og->type, variable_get('subsite_parents', array()))) {
      return;
    }
    
    ctools_include('subsite','vsite');
    
    $subsites = array_reduce(node_load_multiple(array($vsite->id) + vsite_get_subsites($vsite)), function ($ret, $n) {
      $ret[$n->nid] = $n->title; return $ret;
    }, array());
    
    $form['filters']['query_scope'] = array(
      '#title' => 'Include results from',
      '#type' => 'checkboxes',
      //'#options' => array('current' => t('Current Site'), 'subsites' => t('Subsites')),
      '#options' => $subsites,
      '#default_value' => $options['query_scope'],
      '#access' => vsite_has_subsites(),
      '#required' => TRUE,
    );
    //how to alter the display style?  do we need a separate alter hook?
  }
  
  /**
   * @function query_alter
   * 
   * Limit the query's scope to items in this subsite.
   */
  public function query_alter($options, $efq) {
    if (!module_exists('vsite') || (($vsite = spaces_get_space()) == FALSE)) {
      return;
    }

    //skip this if subsites aren't an option
    if (!in_array($vsite->og->type, variable_get('subsite_parents', array()))) {
      return;
    }
    
    //remove default og group ref condition
    $efq->fieldConditions = array_filter($efq->fieldConditions, function($a) {
      return ($a['field']['field_name'] != OG_AUDIENCE_FIELD);
    });

    $efq->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', array_filter($options['query_scope']), 'IN');
  }
}
