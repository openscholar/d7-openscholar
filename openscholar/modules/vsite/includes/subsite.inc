<?php

/**
 * Returns if a user has permission to add/delete a subsite
 *
 * @param $op
 * Operation (create, delete)
 *
 * @param $vsite
 *
 * @return bool
 * Access
 */
function vsite_subsite_access($op = 'create', $vsite = false){

  if (!$vsite) {
    $vsite = vsite_get_vsite();
  }

  $subsite_parents = variable_get('subsite_parents',array('department', 'project'));
  // Account for Space and Node
  $type = isset($vsite->group)? $vsite->group->type : $vsite->type;

  global $user;

  return ($vsite && in_array($type,$subsite_parents) && spaces_access_admin($user, vsite_get_vsite($vsite->nid)) && og_user_access("node", $vsite->nid, $user, "{$op} subsites"));
}

/**
 * Returns the subsites for a site
 *
 * @param $vsite
 * Parent Site (Vsite Object or Int)
 *
 * @param $reset
 *
 * @return array
 * Subsite ID's
 */
function vsite_get_subsites($vsite = false, $reset = false){
  $subsite_map = drupal_static('vsite_get_subsites',array());

  if ($reset || (!$subsite_map && !($subsite_map = cache_get('vsite_subsite_map')))) {
    $subsite_map = array();

    if ($info = field_info_field('field_group_parent')) {
      $result = db_select('field_data_field_group_parent', 'fgp')
        ->fields('fgp',array('bundle','entity_id','field_group_parent_target_id'))
        ->condition('deleted', 0)
        ->execute();

      while ($item = $result->fetch()) {
        $subsite_map[$item->field_group_parent_target_id][$item->entity_id] = $item->entity_id;
      }
    }

    cache_set('vsite_subsite_map',$subsite_map);
  }

  if (is_object($subsite_map)) {
    $subsite_map = $subsite_map->data;
  }

  if (!$vsite && !($vsite = spaces_get_space())) {
    return array();
  }

  $sid = is_object($vsite) ? $vsite->group->nid : $vsite;

  return isset($subsite_map[$sid]) ? $subsite_map[$sid] : array();
}

/**
 * Returns if the current site has subsites
 *
 * @param $vsite
 * Parent Site (Vsite Object or Int)
 *
 *
 * @return boolean
 *
 */
function vsite_has_subsites($vsite = false) {

  //If this site has subsites return True.
  return count(vsite_get_subsites($vsite)) ? TRUE : FALSE;
}

/**
 * Returns the url options that should be used for a cross-site link to a node
 *
 * ie. A link to a subsite
 *
 * @param $vsite
 * Active Site (Vsite Object)
 *
 *
 * @return boolean
 *
 */
function vsite_subsite_node_url_options($node, &$options){
  $vsite = spaces_get_space();

  if (!$vsite || !isset($node->{OG_AUDIENCE_FIELD})) {
    //no group information
    return;
  }

  //Check and see if this node is part of a subsite "not this site"
  $groups = field_get_items('node', $node, OG_AUDIENCE_FIELD);
  if (isset($groups[0]) && $vsite->id != $groups[0]['target_id']) {
    //Absolute will force the use of "viste_domain_name"
    $options['absolute'] = TRUE;
    //This will rewrite the URL based on this nodes group
    $options['purl'] = array('id' => $groups[0], 'provider' => 'spaces_og');
  }
}

/**
 * Inherits site settings specified in the parent site.
 *
 * @param object $node
 *   The subsite node object, to be passed to entity_metadata_wrapper().
 *
 * @see vsite_node_insert()
 */
function vsite_inherit($node) {
  $wrapper = entity_metadata_wrapper('node', $node);
  $parent_sid = $wrapper->field_group_parent->value(array('identifier' => TRUE));
  if (empty($parent_sid) || !$parent_sid) {
    return;
  }
  $parent_vsite = vsite_get_vsite($parent_sid);

  // Checks if the parent site has specified a theme to inherit.
  $name = 'vsite_subsite_inherit_theme';
  $inherit_theme = $parent_vsite->controllers->variable->get($name);
  if (!$inherit_theme) {
    return;
  }

  // Sets the new subsite's theme to match it's parent.
  $name = 'theme_default';
  $value = $parent_vsite->controllers->variable->get($name);
  $vsite = vsite_get_vsite($node->nid);
  $vsite->controllers->variable->set($name, $value);

  // Sets the new subsite's theme flavor to match it's parent.
  $name = 'os_appearance_' . $value . '_flavor';
  $value = $parent_vsite->controllers->variable->get($name);
  if (!empty($value)) {
    $vsite->controllers->variable->set($name, $value);
  }
}
