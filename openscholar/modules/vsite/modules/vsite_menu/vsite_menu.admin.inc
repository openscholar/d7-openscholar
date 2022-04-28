<?php

/**
 * Form for editing an entire menu tree at once.
 *
 * Shows for one menu the menu links accessible to the current user and
 * relevant operations.
 */
function vsite_menu_menu_overview_form($form, &$form_state, $menu) {
  global $menu_admin;
  $form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css');
  include_once drupal_get_path('module', 'menu')."/menu.admin.inc";
  
  $vsite = spaces_get_space();
  if(!$vsite || ! isset($vsite->controllers->menus)) return false;
  
  $tree = $vsite->controllers->menus->get_menu_tree($menu['menu_name']);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
  // We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;

  $form = array_merge($form, _menu_overview_tree_form($tree));
  $form['#menu'] =  $menu;

  if (element_children($form)) {
    //Remove drupal core edit links
    foreach (element_children($form) as $link) {
    	unset($form[$link]['operations']);
    }
    
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = t('There are no menu links yet. <a href="@link">Add link</a>.', array('@link' => url('admin/structure/menu/manage/'. $form['#menu']['menu_name'] .'/add')));
  }
  return $form;
}

/**
 * Submit handler for the menu overview form.
 * Replaces 'menu_overview_form_submit' inside a vsite
 *
 * @see menu_overview_form_submit()
 * @see menu_overview_form()
 */
function vsite_menu_menu_overview_form_submit($form, &$form_state){
  ctools_include('menu', 'os');
  $vsite = spaces_get_space();
  $os_menus = os_get_menus();
  $menu_name = $form['#menu']['menu_name'];
  
  //Only execute this function if we are in a space and this is an os_menu
  if (! $vsite || ! isset($vsite->controllers->menus) || ! array_key_exists($menu_name, $os_menus))
    return menu_overview_form_submit($form, $form_state);
    
  // When dealing with saving menu items, the order in which these items are
  // saved is critical. If a changed child item is saved before its parent,
  // the child item could be saved with an invalid path past its immediate
  // parent. To prevent this, save items in the form in the same order they
  // are sent by $_POST, ensuring parents are saved first, then their children.
  // See http://drupal.org/node/181126#comment-632270
  $order = array_flip(array_keys($form_state['input'])); // Get the $_POST order.
  $form = array_merge($order, $form); // Update our original form with the new order.
   
  $items = array();
  $fields = array(
      'weight',
      'plid'
  );
  
  //Build the full menu_item list and mark the customized menu entries as such
  foreach ( element_children($form) as $mlid ) {
    if (isset($form[$mlid]['#item'])) {
      $element = $form[$mlid];
      // Update any fields that have changed in this menu item.
      foreach ( $fields as $field ) {
        if ($element[$field]['#value'] != $element[$field]['#default_value']) {
          $element['#item'][$field] = $element[$field]['#value'];
          $items[$mlid] = $element['#item'];
          $element['#item']['customized'] = 1;
        }
      }
      // Hidden is a special case, the value needs to be reversed.
      if ($element['hidden']['#value'] != $element['hidden']['#default_value']) {
        // Convert to integer rather than boolean due to PDO cast to string.
        $element['#item']['hidden'] = $element['hidden']['#value'] ? 0 : 1;
        $element['#item']['customized'] = 1;
        $items[$mlid] = $element['#item'];
      }
    }
  }
  
  //Save the changed links and update thier parent items and depth
  foreach ($items as $item){
  	vsite_menu_menu_link_save($item);
  }
  
  drupal_set_message(t('Your configuration has been saved.'));
}


