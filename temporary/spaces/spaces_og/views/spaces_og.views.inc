<?php

/**
 * Implements hook_views_plugins().
 */
function spaces_og_views_plugins(){
  return array(
      'argument validator' => array(
          'useringroups' => array(
              'title' => t("User in current user's groups"),
              'handler' => 'spaces_og_plugin_argument_validate_useringroups',
              'path' => drupal_get_path('module', 'spaces_og') . '/views'
          )
      )
  );
}

/**
 * Implements hook_views_handlers().
 */
function spaces_og_views_handlers(){
  return array(
      'info' => array(
          'path' => drupal_get_path('module', 'spaces_og') . '/views'
      ),
      'handlers' => array(
          'spaces_og_handler_field_og_group_nids' => array(
              'parent' => 'og_views_handler_field_og_group_nids'
          )
      )
  );
}

/**
 * Implements hook_views_data_alter().
 */
function spaces_og_views_data_alter(&$data){
  if (isset($data['og_membership']['gid']['field']['handler'])) {
    $data['og_membership']['gid']['field']['handler'] = 'spaces_og_handler_field_og_group_nids';
  }
}

/**
 * Implements hook_views_pre_view().
 */
function spaces_og_views_pre_view(&$view, &$display_id, &$args){

  // The Spaces Current Space filter is bound to the space data stub
  // if we are in an og space we want to use the og membership relation
  // to filter the posts
  $space = spaces_get_space();

  //Is this space of type OG and we are acting on users or nodes
  if (is_object($space) && $space->type == 'og') {
    //After we have determined that we are in an OG space lets make
    //sure this display makes use of the spaces filter
    $filter = $view->get_item($display_id, 'filter', 'current');

    if ($filter) {

      $table = false;
      if (!isset($filter['relationship']) || $filter['relationship'] == 'none') {
        $table = $view->base_table;
      }
      elseif (($rel = $view->get_item($display_id, 'relationship', $filter['relationship']))) {
        //Get Relationship data
        $rel_data = views_get_handler($rel['table'], $rel['field'], 'relationship');

        //Use the base table for the relationship the current space filter is attached to
        if (isset($rel_data->definition['base'])) {
          $table = $rel_data->definition['base'];
        }
      }

      if (in_array($table, array(
        'file_managed',
        'node',
        'user'
      ))) {

        //Check to see if the relationship has already been added via the UI
        if (! $view->get_item($display_id, 'relationship', 'og_membership_rel')) {
          //If so lets add the Relationship
          $view->add_item($display_id, 'relationship', $table, 'og_membership_rel');
        }

        //Set the 'current' space filter up to use the new og relationshipt
        $view->set_item_option($display_id, 'filter', 'current', 'relationship', 'og_membership_rel');
      }

      $i = 1;
      //For views that use multiple spaces filters tie the multiple spaces filters to thier respective og_membership tables
      while($view->get_item($display_id, 'filter', 'current_'.$i) && $view->get_item($display_id, 'relationship', 'og_membership_rel_'.$i)){
        //Set the 'current_n' space filter up to use a og relationship
        $view->set_item_option($display_id, 'filter', 'current_'.$i, 'relationship', 'og_membership_rel_'.$i);
        $i++;
      }
    }
  }
}
