<?php 

/**
 * @file node_person.inc
 * 
 * person plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_person'),
  'entity_type' => array('node'),
  'bundle' => array('person'),
);

class sv_list_node_person  extends sv_list_plugin {
  
  /**
   * @function sort_alter
   * 
   * People should use last name, not first.
   */
  public function sort_alter($options, $efq) {  
    if ($options['content_type'] != 'person') {
      return;
    }
    
    if ($options['sort_by'] == 'sort_alpha') {
      // store the existing sorts in temporary vars
      $temp = $efq->order;
      $efq->order = array();
      // the field sort is complicated, so it's best to let Drupal worry about constructing it
      $efq->fieldOrderBy('field_last_name', 'value', 'ASC');
      $fsort = $efq->order;

      // find the title sort, drop the field sort in front of it, title is (first + last) so this will
      // enforce sort by last name then sort by title.
      foreach ($temp as $k => $sort) {
        if ($sort['specifier'] == 'title') {
          array_splice($temp, $k, 0, $fsort);
          break;
        }
      }

      $efq->order = $temp;
    }
  }
}
