<?php
/**
 * Provides and CRUD functions needed for adding or removing users from a vsite
 *
 * @file user.inc
 *
 * Provides user management and crud functions
 *
 * vsite_user_add
 *  Add users to a OpenScholar vsite
 *
 * vsite_user_remove
 *  Remove a user from a OpenScholar vsite
 *
 * vsite_role_grant
 *   Grant a user a role
 *
 * vsite_role_revoke
 *   Revoke a user a role
 */


/**
 * Add users to a OpenScholar vsite
 *
 * @param $account
 * The user ID or object
 *
 * @param $site_id
 * Site ID
 *
 * @return bool
 * Success
 */
function vsite_user_add($account, $site_id = false){

  $vsite = false;
  if($site_id){
    $vsite = spaces_load('og', $site_id);
  }

  if(is_int($account)){
    $account = user_load($uid);
  }
  //Do some basic validation
  if (!is_object($account)) {
    //An invalid user has been passed
    return false;
  }

  module_load_include('inc', 'vsite', 'includes/vsite');

  //Add the user to this site
  return vsite_add_entity($account, 'user', $vsite);
}

/**
 * Remove a user from a OpenScholar vsite
 *
 * @param $account
 * The user ID or object
 *
 * @param $site_id
 * Site ID
 *
 * @return bool
 * Success
 */
function vsite_user_remove($account, $site_id = false){

  $vsite = false;
  if($site_id){
    $vsite = spaces_load('og', $site_id);
  }

  if(is_int($account)){
    $account = user_load($uid);
  }
  //Do some basic validation
  if (!is_object($account)) {
    //An invalid user has been passed
    return false;
  }

  module_load_include('inc', 'vsite', 'includes/vsite');

  //Remove the user from this site
  return vsite_remove_entity($account, 'user', $vsite);
}

/**
 * Grant a role to a user within a vsite.
 *
 * @param $uid
 *   The user ID.
 * @param $rid
 *   The role ID.
 * @param $site_id
 *   The Group ID
 * @param $group_type
 *   The Group Type
 */
function vsite_role_grant($uid, $role_name, $site_id = false, $group_type = 'node') {

  if(!$site_id){
    $vsite = spaces_get_space();
    if($vsite){
      $site_id = $vsite->id;
      $group_type = $vsite->group_type;
    }
  }

  if($site_id){
    $wrapper = entity_metadata_wrapper($group_type, $site_id);
    $bundle = $wrapper->getBundle();
    //Add role via OG
    $roles = og_roles($group_type, $bundle, $site_id);
    foreach ($roles as $rid => $role) {
      if($role == $role_name){
        og_role_grant($group_type, $site_id, $uid, $rid);
        return true;
      }
    }
  }

  //Unable to determine group
  return false;
}

/**
 * Revoke a role to a user within a vsite.
 *
 * @param $uid
 *   The user ID.
 * @param $rid
 *   The role ID.
 * @param $site_id
 *   The Group ID
 * @param $group_type
 *   The Group Type
 */
function vsite_role_revoke($uid, $role_name, $site_id = false, $group_type = 'node') {

  if(!$site_id){
    $vsite = spaces_get_space();
    if($vsite){
      $site_id = $vsite->id;
      $group_type = $vsite->group_type;
    }
  }

  if($site_id){
    $wrapper = entity_metadata_wrapper($group_type, $site_id);
    $bundle = $wrapper->getBundle();
    //Add role via OG
    $roles = og_roles($group_type, $bundle, $site_id);
    foreach ($roles as $rid => $role) {
      if($role == $role_name){
        og_role_revoke($group_type, $site_id, $uid, $rid);
        return true;
      }
    }
  }

  //Unable to determine group
  return false;
}