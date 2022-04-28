<?php

/**
 * @file
 * Provide API function to add and remove entities from virtual-sites.
 */

/**
 * Add the passed entity to a vsite.
 *
 * The entity that you wish to add to the vsite.
 * @param $entity
 *   The entity object.
 * @param $entity_type
 *   Optional; The type of entity you are passing. Defaults to "node".
 * @param space $vsite
 *   Optional; The vsite object to add the entity to. Defaults to the
 *   active space.
 *
 * @return
 *   The newly created OG-membership or FALSE if not valid.
 */
function vsite_add_entity($entity, $entity_type = 'node', $vsite = FALSE){
  // Make sure the entity is valid.
  list($id) = entity_extract_ids($entity_type, $entity);

  // Get the active space.
  if(!$vsite){
    $vsite = spaces_get_space();
  }

  if ($id && $vsite){
    $values = array(
        'entity_type' => $entity_type,
        'entity' => $entity,
    );

    // Add this entity to the active group.
    $og_membership = og_group($vsite->group_type, $vsite->id, $values);

    // Tell OG to ignore the existing field settings on this entity.
    $entity->skip_og_membership = TRUE;

    return $og_membership;
  }

  return FALSE;
}

/**
 * Add the passed $node to the specified $vsite.
 */
function vsite_add_node($node, $gid = FALSE, $update_alias = TRUE) {
  // Loads the vsite to add the node as group content.
  // Only sets reset = TRUE when we need to update the alias; this is because
  // the purl value will be cached as NULL on newly created vsites, but we'll
  // need the purl value in order to update the alias properly.
  // @see vsite_pathauto_alias_alter()
  $vsite = vsite_get_vsite($gid, $update_alias);
  $og_membership = vsite_add_entity($node, 'node', $vsite);
  // Optionally update the pathauto alias to reflect the new vsite.
  if ($update_alias) {
    if (!isset($node->path)) {
      $node->path = array();
    }
    // Tells pathauto to update this node's alias.
    $node->path['pathauto'] = TRUE;
    $node->vsite_pathauto_alias_alter = TRUE;

    // Saves the node to trigger
    node_save($node);
  }

  return $og_membership;
}

/**
 * Remove the passed entity from a vsite
 *
 * The entity that you wish to add to the vsite.
 * @param $entity
 *   The entity object.
 * @param $entity_type
 *   Optional; The type of entity you are passing. Defaults to "node".
 * @param space $vsite
 *   Optional; The vsite object to add the entity to. Defaults to the
 *   active space.
 */
function vsite_remove_entity($entity, $entity_type = 'node', $vsite = FALSE){
  // Make sure the entity is valid.
  list($id) = entity_extract_ids($entity_type, $entity);
  if (!$id) {
    return;
  }

  // Get the active space.
  $vsite = $vsite ? $vsite : spaces_get_space();
  if (!$vsite) {
    return;
  }

  // Remove this entity from the group.
  og_ungroup($vsite->group_type, $vsite->id, $entity_type, $id);
}

/**
 * Creates a new vsite
 *
 * @param $name
 *   Name of site.
 * @param $domain
 *   Path for new site.
 * @param $uid
 *  The user ID to assocaite with the new VSite.
 * @param string $bundle
 *  The type(bundle) of the VSite to be created.
 * @param bool $preset
 *  Type of vsite to create.
 * @param null $parent
 *  The parent VSite of the new VSite.
 * @param bool $visibility
 *  Default visibility of the new VSite.
 * @param array $additional_settings
 *  Array of additional settings for the node object.
 *
 * @return bool
 *  The SPACE object if created or FALSE.
 */
function vsite_create_vsite($name, $domain, $uid, $bundle = 'personal', $preset = FALSE, $parent = NULL, $visibility = FALSE, $additional_settings = array()) {
  $node = new stdClass();
  $node->type = $bundle;
  node_object_prepare($node);

  $node->title = $name;
  $node->body = array(LANGUAGE_NONE => array(array('value' => '')));
  $node->purl = array(
    'value' => $domain,
    'provider'=>'vsite_og',
    'id'=>NULL,
  );
  $node->uid = $uid;
  $node->spaces_preset_og = $preset;
  $node->language = LANGUAGE_NONE;
  // Sets the parent relationship if this is a subsite with a parent specified.
  if (isset($parent) && $parent_node = node_load($parent)) {
    $node->field_group_parent = array(
      LANGUAGE_NONE => array(
        array(
          'target_id' => $parent,
          'entity' => $parent_node,
          'access' => TRUE,
        )
      )
    );
  }
  // Sets the site visibility value if the module is available and specified.
  if (module_exists('vsite_access')) {
    if (isset($visibility)) {
      $node->{VSITE_ACCESS_FIELD} = array(
        LANGUAGE_NONE => array(
          array(
            'value' => $visibility,
          )
        )
      );
    }
  }

  // Allow other modules to alter the node object before saving it to the DB.
  $node = (object) array_merge((array) $node, $additional_settings);

  node_save($node);

  if ($vsite = spaces_load('og', $node->nid)) {
    return $vsite;
  }
  else {
    error_log(t('Error creating '.$bundle.': @nid', array('@nid'=>$node->nid)), LOG_WARNING);
    return FALSE;
  }
}

/**
 * Gets all vsites a user "owns".
 *
 * @param $uid
 *   The user ID of the user to lookup. Defaults to current user.
 *
 * @return
 *   Array with VSite objects if found, or empty array.
 */
function vsite_get_vsite_by_owner($uid = NULL) {
  if (!$uid) {
    global $user;
    $uid = $user->uid;
  }

  if (empty($uid)) {
    return array();
  }

  $group_bundles = og_get_all_group_bundle('node');
  if (empty($group_bundles)){
    //can't determine group content types
    return array();
  }
  $bundles = array_keys($group_bundles);

  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->propertyCondition('uid', $uid)
    ->entityCondition('bundle', $bundles, 'IN')
    ->execute();

  if (empty($result['node'])) {
    return array();
  }

  foreach(array_keys($result['node']) as $id) {
    $vsites[] = spaces_load('og', $id);
  }

  // Filter out any spaces that were not loaded.
  return array_filter($vsites);
}

/**
 * Gets all vsites a user is a member of
 *
 * @param $uid
 *   The user ID of the user to lookup. Defaults to current user.
 *
 * @return
 *   Array with VSite objects if found, or NULL.
 */
function vsite_get_user_vsites($uid = NULL) {

  //Retrieve all group memberships for the user
  $groups = og_get_entity_groups('user', $uid);

  if (empty($groups['node'])) {
    return array();
  }

  $vsites = array();
  foreach($groups['node'] as $mid => $gid) {
    $vsites[]  = spaces_load('og', $gid);
  }

  // Filter out any spaces that were not loaded.
  return array_filter($vsites);
}
