<?php 

/**
 * @file node_presentation.inc
 * 
 * presentation plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_presentation'),
  'entity_type' => array('node'),
  'bundle' => array('presentation'),
);

class sv_list_node_presentation extends sv_list_plugin  {
  /**
   * @function register_sorts()
   */
  public function register_sorts() {
    return array('sort_presentation' => t('Recently Presented'));
  }
  
  /**
   * @function sort_presentation
   */
  function sort_presentation($options, &$efq) {
    $efq->fieldOrderBy('field_presentation_date', 'value', 'DESC');
    $efq->propertyOrderBy('created', 'DESC');
  }
}
