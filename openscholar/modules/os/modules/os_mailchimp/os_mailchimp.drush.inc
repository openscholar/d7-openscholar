<?php 

/**
 * @file os_mailchimp.drush.inc
 * 
 * Drush command for getting a list of users, suitable for bulk subscriptions
 */

function os_mailchimp_drush_command() {
  return array(
    'os-mailchimp-list' => array(
      'description' => dt('Get a list of names and email addresses'),
      'options' => array(),
      'arguments' => array(),
      'examples' => array('drush @prod os-ml' => dt('Get a comma separated list of user accounts and email addresses')),
      'aliases' => array('os-ml'),
    ),
  );
}

function drush_os_mailchimp_list() {
  $purl = variable_get('purl_base_domain');
  
  //get user and names
  $query = db_select('users', 'u')
    ->fields('u', array('mail', 'uid'));
  $query->leftJoin('field_data_field_first_name', 'fn', 'fn.entity_id = u.uid');
  $query->condition('fn.entity_type', 'user' );
  $query->fields('fn', array('field_first_name_value'));
  
  $query->leftJoin('field_data_field_last_name', 'ln', 'ln.entity_id = u.uid');
  $query->condition('ln.entity_type', 'user' );
  $query->fields('ln', array('field_last_name_value'));
  
   
  $result = $query->execute();
  while ($row = $result->fetchAssoc()) {
    $row[] = $purl;
    $row['purl'] = _os_mailchimp_user_vsites($row['uid']);
   
    unset($row['uid']);
    print(implode(',', $row) . "\n");
    unset($row);
  }
}

function _os_mailchimp_user_vsites($uid) {
  //their vsites
  $query = db_select('node', 'n')
    ->condition('n.uid', $uid)
    ->condition('n.type', array('personal', 'project', 'department'), 'IN');
  
  //each vsite's purl(s).
  $query->leftJoin('purl', 'p1', "n.nid = p1.id AND p1.provider = 'vsite_domain'");
  $query->fields('p1', array('value'));
  $query->join('purl', 'p2', "n.nid = p2.id AND p2.provider = 'spaces_og'");
  $query->fields('p2', array('value'));
  $result = $query->execute();
  
  $purls = array();
  while ($row = $result->fetch()) {
    $purls[] = ($row->value) ? $row->value : $row->p2_value; 
  }
  
  return implode(' ', $purls);
}