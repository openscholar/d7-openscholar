<?php
/**
 * @file menu.inc
 * Provides menu management and CRUD API functions.
 */

/**
 * Returns the list of menu names used on all OpenScholar sites.
 */
function os_get_menus() {

  $menus = variable_get('os_menus', array(
      'primary-menu' => 'Primary Menu',
      'secondary-menu' => 'Secondary Menu',
  ));

  drupal_alter('os_menus', $menus);

  return $menus;
}

/**
 * Creates (inserts) a new menu to be used in the OS Menu system.
 *
 * Note: this function does NOT implement hook_menu_insert().
 *
 * @param $title
 *  The human readable menu title.
 *
 * @param $menu_name (optional)
 *  The unique name of the custom menu (composed of lowercase letters,
 *  numbers, and hyphens).
 *
 * @param $description (optional)
 *  The custom menu description.
 */
function os_menu_create($title, $menu_name = false, $description = '') {

  if (!$menu_name) {
    // Strip non alphanumeric char's.
    $menu_name = pereg_replace("[^a-z0-9-]", "",str_replace(" ","-",strtolower($title)));
  }

  if (module_exists('vsite_menu')) {
    //Save a new menu using the vsite controllers.
    vsite_menu_add($title, $menu_name);
  }
  else {

    $current_menus = os_get_menus();
    $current_menus[$menu_name] = $title;
    variable_set('os_menus',$current_menus);

    // Call the standard drupal menu save (this will clear all menu caches).
    menu_save(array(
      'title' => $title,
      'menu_name' => $menu_name,
      'description' => $description,
    ));
  }
}

/**
 * Removes (deletes) a menu from the OpenScholar menu system.
 *
 * Note: this function does NOT implement hook_menu_delete().
 *
 * @param string $menu_name
 *   The machine name of the menu to remove.
 */
function os_menu_remove($menu_name) {

  $current_menus = os_get_menus();
  //Is this a valid menu to remove?
  if (!isset($current_menus[$menu_name])) {
    return FALSE;
  }

  if (module_exists('vsite_menu')) {
    // Delete a menu using the vsite controllers.
    vsite_menu_del($menu_name);
  }
  elseif (!module_exists('vsite')) {

    // Remove the menu from the variable.
    unset($current_menus[$menu_name]);
    variable_set('os_menus',$current_menus);

    // Call the standard drupal menu delete (this will clear all menu caches and
    // delete all the menu links).
    $menu = menu_load($menu_name);
    menu_delete($menu);
  }

  return TRUE;
}

/**
 * Reverts the OpenScholar menu(s) back to the default configuration.
 *
 * @param $menu_name
 *   (Optional) The name of the menu to revert.
 *   If empty, reverts all menus instead.
 *
 * @return bool
 *   TRUE if vsite_menu_revert() was invoked.
 */
function os_menu_revert($menu_name = FALSE) {

  $current_menus = os_get_menus();
  // Checm if this is a valid menu to remove.
  if ($menu_name && !isset($current_menus[$menu_name])) {
    return FALSE;
  }

  if (module_exists('vsite_menu')) {
    //Delete a menu using the vsite controllers
    vsite_menu_revert($menu_name);
    return TRUE;
  }
  else {
    // Nothing to do in single-tenant no overrides.
    // @TODO possibly delete custom menu's that were not present on install
    return FALSE;
  }
}

/**
 * Returns the menu_tree page data ready for theming by menu_tree_output().
 *
 * @param $menu_name
 *
 * @return
 *  Menu Tree Page Data
 */
function os_menu_tree_data($menu_name) {
  $menu_data = &drupal_static(__FUNCTION__, array());
  $menus = os_get_menus();
  if (!isset($menus[$menu_name])) {
    return array();
  }

  if (!isset($menu_data[$menu_name])) {
    if (module_exists('vsite_menu')) {
      $tree = vsite_menu_os_menu_tree($menu_name);
    }
    if (!isset($tree) || $tree === FALSE) {
      $tree = menu_tree_page_data($menu_name);
    }

    // Allow other modules to make changes to the tree.
    drupal_alter('os_menu_tree', $menu_name, $tree);
    $menu_data[$menu_name] = $tree;
  }

  return $menu_data[$menu_name];
}

/**
 * Returns an individual rendered OpenScholar menu.
 *
 * Replaces menu_tree(),
 */
function os_menu_tree($menu_name) {

  $menus = os_get_menus();
  if (!isset($menus[$menu_name])) {
    return;
  }

  $menu_output = menu_tree_output(os_menu_tree_data($menu_name));
  return $menu_output;
}

/**
 * Creates (inserts) a new menu link or updates an existing menu link.
 *
 * @param array $item
 *  An associative array representing a menu link item, with elements:
 *  - link_path: (required) The path of the menu item, which should be
 *    normalized first by calling drupal_get_normal_path() on it.
 *  - link_title: (required) Title to appear in menu for the link.
 *  - menu_name: (optional) The machine name of the menu for the link.
 *    Defaults to 'navigation'.
 *  - weight: (optional) Integer to determine position in menu. Default is 0.
 *  - expanded: (optional) Boolean that determines if the item is expanded.
 *  - options: (optional) An array of options, see l() for more.
 *  - mlid: (optional) Menu link identifier, the primary integer key for each
 *    menu link. Can be set to an existing value, or to 0 or NULL.
 *
 * to insert a new link.
 * - plid: (optional) The mlid of the parent.
 * - router_path: (optional) The path of the relevant router item.
 *
 * @return
 *  The mlid of the saved menu link, or FALSE if the menu link could not be
 *  saved.
 */
function os_menu_link_save(&$item) {
  $mlid = FALSE;
  if (module_exists('vsite_menu')) {
    // Save a menu item using the vsite controllers.
    $mlid = vsite_menu_menu_link_save($item);
  }
  else {
    // Call the standard drupal menu save (this will clear all menu caches).
    $mlid = menu_link_save($item);
  }

  return $mlid;
}

/**
 * Removes (deletes) one menu link item.
 *
 * Note: this function does NOT implement hook_menu_link_delete().
 *
 * @param $menu
 *   A valid menu identifier
 *
 * @param $mlid
 *   A valid menu link mlid
 */
function os_menu_link_remove($menu, $mlid) {

  if (module_exists('vsite_menu')) {
    // Delete a menu item using the vsite controllers.
    $mlid = vsite_menu_delete_menu_link($menu, $mlid);
  }
  else {
    // Call the standard drupal menu delete (this will clear all menu caches).
    _menu_delete_item(db_query("SELECT * FROM {menu_links} WHERE mlid = :mlid", array(
        ':mlid' => $mlid
    ))->fetchAssoc());
  }
}

/**
 * Loads and returns an individual menu link object.
 *
 * @param $menu
 *  A valid menu identifier
 *
 * @param $mlid
 *  A valid menu link mlid
 *
 * @return
 *  A menu link, with $item['access'] filled and link translated for
 *  rendering.
 */
function os_menu_link_load($mlid, $menu = '') {
  if (module_exists('vsite_menu')) {
    // Return a menu item loaded from vsite menu controller.
    return vsite_menu_menu_link_load($mlid, $menu);
  }
  else {
    // Call the standard drupal menu_link_load, _menu_translate is run to set
    // access.
    return menu_link_load($mlid);
  }
}

/**
 * Loads and returns a menu link by router path.
 *
 * @param $path
 *  A valid drupal path (not alias), i.e. "node/123".
 *
 * @return
 *  A menu link, with $item['access'] filled and link translated for
 *  rendering or false if no link exists
 */
function os_menu_load_link_path($path) {

  $os_menus = os_get_menus();
  if (module_exists('vsite_menu')) {
    // Return a menu item loaded from vsite menu controller.
    foreach ( $os_menus as $menu => $name ) {
      $mlid = vsite_menu_get_link_path($menu, $path);
      if ($mlid) {
        return os_menu_link_load($mlid, $menu);
      }
    }
  }
  else {
    $mlid = db_query_range("SELECT mlid FROM {menu_links} WHERE link_path = :path AND module = 'menu' AND menu_name IN (:type_menus) ORDER BY mlid ASC", 0, 1, array(
        ':path' => $path,
        ':type_menus' => array_keys($os_menus)
    ))->fetchField();
    if ($mlid) {
      // Call the standard drupal menu_link_load, _menu_translate is run to set
      // access
      return menu_link_load($mlid);
    }
  }

  return FALSE;
}

/**
 * Form builder; presents an entire menu tree at once for editting.
 *
 * Shows for one menu the menu links accessible to the current user and relevant
 * operations.
 *
 * An override of the standard drupal overview form.
 */
function os_menu_overview_form($form, &$form_state, $menu) {

  // If the menu string was passed load the full menu.
  if (is_string($menu))
    $menu = menu_load($menu);

  if (module_exists('vsite_menu')) {
    include_once drupal_get_path('module', 'vsite_menu') . '/vsite_menu.admin.inc';

    // Retrieve a menu item using the vsite controllers.
    $form = vsite_menu_menu_overview_form($form, $form_state, $menu);
  }
  else {
    // Call the standard drupal menu retrieve.
    $form = menu_overview_form($form, $form_state, $menu);
  }

  // Setup default submit and theme handlers.
  $form['#submit'][] = 'menu_overview_form_submit';
  $form['#theme'][] = 'menu_overview_form';

  return $form;
}
