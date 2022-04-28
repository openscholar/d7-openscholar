<?php 

/**
 * @file plugin.inc
 * 
 * Parent class for sv list plugins
 */

/**
 * Plugin declaration
 * 
 * entity_type and bundle can be used to restrict this plugin to a specific entity/bundle type
 */
$plugin = array(
  'handler' => array('class' => 'sv_list_plugin'),
  'entity_type' => array(), 
  'bundle' => array(),
);

class sv_list_plugin {
  function __construct($entity_type, $bundle) {
    $this->entity_type = $entity_type;
    $this->bundle = $bundle;
  }

  /**
   * @function option_defaults()
   * 
   * Sets values of options in new boxes
   */
  function option_defaults() {  
  }
  
  /**
   * @function options_form
   * 
   * Alter the box's options form.  
   */
  public function options_form($box_options, &$form) {
  }
  
  /**
   * @function register_sorts
   * 
   * Adds a sort option to sv list.  Return an array keyed by sort function names, each pointing to
   * the label for that sort function.  Your plugin is required to implement its own sort function.
   */
  public function register_sorts() {
  }
  
  /**
   * @function query_alter
   * 
   * Alters the entity field query
   */
  public function query_alter($options, $efq) {
  }
  
  /**
   * @function sort_alter
   * 
   * As query_alter, but taking place a little bit later.  This lets you alter the query after
   * it's been sorted.
   */
  public function sort_alter($options, $efq) {
  }
  
  /**
   * @function entities_alter
   * 
   * After the query has executed, make changes to the entities it loaded.
   */
  public function entities_alter($options, &$entities) {
  }
  
  /**
   * @function render
   * 
   * Take over the rendering of a set of entities.  If this is used, the default render function will 
   * be skipped.
   */
  public function render($options, &$entities) {
  }
  
}