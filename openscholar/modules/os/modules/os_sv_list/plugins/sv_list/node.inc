<?php 

/**
 * @file node.inc
 * 
 * person plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node'),
  'entity_type' => array('node'),
  'bundle' => array('person'),
);

class sv_list_node  extends sv_list_plugin {
  
  /**
   * @function sort_alter
   * 
   * People should use last name, not first.
   */
  public function sort_alter($options, $efq) {
    if ($options['sort_by'] != 'sort_random') {
      $order = $efq->order;
      $efq->order = array();
      $efq->propertyOrderBy('sticky', 'DESC');
      $efq->order = array_merge($efq->order, $order);
    }
  }
}
