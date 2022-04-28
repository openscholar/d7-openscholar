<?php 

/**
 * @file node_news.inc
 * 
 * news plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_news'),
  'entity_type' => array('node'),
  'bundle' => array('news'),
);

class sv_list_node_news extends sv_list_plugin  {
  /**
   * @function register_sorts()
   */
  public function register_sorts() {
    return array('sort_news' => t('News Date'));
  }
  
  /**
   * @function sort_news
   */
  function sort_news($options, &$efq) {
    $efq->fieldOrderBy('field_news_date', 'value', 'DESC');
  }
}
