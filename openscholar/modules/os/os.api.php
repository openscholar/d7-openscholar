<?php

/**
 * @file
 * Hooks provided by OpenScholar.
 */

/**
 * Implements hook_os_widget().
 *
 * @returns array
 * Blocks that this module would like to be avalible in the openscholar layout
 */
function hook_os_widget() {

  // Block can make use of any field in the block schema
  return array(
    array(
      'title' => "My Modules block",
      'theme' => 'block_theme',
      'region' => 'sidebar_first',
      'weight' => 100,
      'hidden' => 0,
    )
  );
}

/**
 * Implements hook_os_widget_alter().
 *
 * This function should add any parameters to the passed block that
 * will be needed in the admin user interfaces.  Including access params
 * config paths etc...
 *
 * @param $widget
 */
function hook_os_widget_alter(&$widget) {

  // Does this widget belong to my module.
  if ($widget->module == 'mymodule') {
    // Provides a path to configure the widget that my module needs.
    $widget['config_path'] = '/my/conf/path';
  }
}

/**
 * Implements hook_os_menus_alter().
 *
 * Alters the list of menu's that avalible in the openscholar UI
 *
 * @param $menus
 *   An associative array like:
 *     'menu-id' => 'Menu Item Title'
 */
function hook_os_menus_alter(&$menus) {

  // Remove this menu.
  if (isset($menus['primary-menu'])) {
    unset($menus['primary-menu']);
  }
}

/**
 * Implements hook_os_menu_tree_alter().
 *
 * Alters OpenScholar's menu_tree page data before it is passed to theme
 * functions.
 */
function hook_os_menu_tree_alter($menu_name, &$tree) {

  foreach ($tree as $id => $menu_link) {
    if (count($menu_link['below'])) {
      // This menu link has children add somthing
      $tree[$id]['link']['options'] = 'xx';
    }
  }
}

/**
 * Implements hook_os_layout_contexts().
 *
 * Return Contexts along with thier label that your module creates
 * and would like to allow users to edit
 *
 * @param $privacy array
 * Optional privacy conditions to honor
 *
 * @return array
 * 	Avalible contexts with thier descriptions
 */
function hook_os_layout_contexts($privacy = false) {

  // Contexts provided by this module
  $provided_contexts = array(
    'my_context'=> 'Calendar Section',
    'another_context' => 'Twitter Page',
  );

  return $provided_contexts;
}

/**
 * Implements hook_os_layout_contexts_alter().
 * Modify the contexts that are avalible for a user to edit
 */
function hook_os_layout_contexts_alter(&$all_contexts) {

  // Removes a context when a given module is disabled.
  if(isset($all_contexts['special_context']) && !module_exists('special_module')) {
    unset($all_contexts['special_context']);
  }
}

/**
 * Alter the blocks in a layout pre-save.
 *
 * This is your last chance to alter layout information before save, only applicable
 * for performing high level re-arranging.  Or for performing actions associated with
 * layout changes like re-indexing.  Contexts hooks do not fire in all cases so this
 * is needed.
 *
 * @param $blocks
 *   An array of blocks to be saved
 *
 * @paramm $context_name
 *   Name of the context / layout getting saved
 *
 */
function hook_os_layout_set_alter(&$blocks, $context_name) {

}

/**
 * Implements hook_os_add_new_links_alter().
 *
 * Modify items in the 'Add New' dropdown that appears on every public facing
 * page.
 * Items are passed through l(), so should follow the same structure.
 */
function hook_os_add_new_links_alter(&$links) {
  $type = 'bundle_name';
  if (isset ($links[$type])) {
    $links['something-else'] = array(
      'title' => 'Something',
      'href' => 'some/thing',
    );
  }
}

/**
 * Implements hook_os_app_info().
 *
 * Describes a feature.
 *
 * Keys:
 *   path => Path to page listing feature content.
 *   views tabs => List of views that display this feature's content.  This is used by
 *                 os_taxonomy so that terms can be applie to filter a view.
 *   nodetypes => List of content types provided by this feature.
 */
function hook_os_app_info() {
  $apps = array();

  // The array key should be the same as the module machine name.
  $apps['os_software'] = array(
    'path' => 'software',
  	'nodetypes' => array(
  	  'software_project',
  	  'software_release',
    ),
    'views tabs' => array(
      'os_software_projects' => array('page'),
    ),
  );

  return $apps;
}

/**
 * Implements hook_os_app_info_alter().
 *
 * Makes changes to os_app_info.
 */
function hook_os_app_info_alter(&$info) {
  // Removes "blogpost" from appearing in the nodetypes array from my_app.
  if (isset($info['my_app']['nodetypes']['blogpost'])) {
    unset($info['my_app']['nodetypes']['blogpost']);
  }
}
