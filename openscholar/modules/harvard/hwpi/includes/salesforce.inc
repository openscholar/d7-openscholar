<?php


/**
 * Creates implementation project in Salesforce.
 */
function hwpi_salesforce_impl_project_create($vsite, $author) {
  global $base_url;

  // If uid is passed
  if (is_int($author)) {
    $author = user_load($author);
  }

  $sfapi = salesforce_get_api();

  // Not authorized, we need to bail this time around.
  if (!$sfapi->isAuthorized()) {
    return;
  }

  $params = array (
    'Name' => url($base_url . '/' . $vsite->group->purl, array('absolute' => TRUE)),
    'Start_Date__c' => date("Y-m-d"),
    'Dev_URL__c' => url($base_url . '/' . $vsite->group->purl, array('absolute' => TRUE)),
    'Service__c' => 'Implementation',
    'Service_level__c' => 'Self-Service Impl',
  );

  try {
    $data = $sfapi->objectCreate("Project__c", $params);
  }
  catch(SalesforceException $e) {
    watchdog_exception('hwpi', $e);
    salesforce_set_message($e->getMessage(), 'error');
    return;
  }

  if (!empty($data['errorCode'])) {
    $message = t('Failed to sync %label with Salesforce. @code: @message',
      array(
        '%label' => 'whatever',
        '@code' => $data['errorCode'],
        '@message' => $data['message'],
      )
    );
    salesforce_set_message($message, 'error');
    watchdog('hwpi', $message);
    return;
  }

  return;
}

/**
 * Creates support project in Salesforce.
 */
function hwpi_salesforce_support_project_create($vsite, $author) {
  global $base_url;

  // If uid is passed
  if (is_int($author)) {
    $author = user_load($author);
  }

  $sfapi = salesforce_get_api();

  // Not authorized, we need to bail this time around.
  if (!$sfapi->isAuthorized()) {
    return;
  }

  // Prepares the new vsite owner (user)'s full name.
  $name_values = array();
  $name_fields = array('field_first_name', 'field_last_name');
  foreach ($name_fields as $field) {
    $items = field_get_items('user', $author, $field);
    if (isset($items[0]['value']) && !empty($items[0]['value'])) {
      $name_values[] = $items[0]['value'];
    }
  }
  $full_name = sizeof($name_values) > 1 ? implode(' ', $name_values) : t('(No entry)');

  // Prepares the site visibility value and fetches human-readable name.
  $items = field_get_items('node', $vsite->group, VSITE_ACCESS_FIELD);
  $visibility = (isset($items[0]['value'])) ? $items[0]['value'] : FALSE;
  $visibility_text = '';
  if ($visibility !== FALSE) {
    switch ($visibility) {
      case 0:
        $visibility_text = 'Public on the web';
        break;
      case 1:
        $visibility_text = 'Invite only';
        break;
      case 2:
        $visibility_text = 'Anyone with the link';
        break;
      case 3:
        $visibility_text = 'Harvard community';
        break;
    }
  }

  // Prepares a site type like "department (os_department_minimal preset)".
  $site_type = isset($vsite->group->type) ? $vsite->group->type : '';

  $params = array (
    'Name' => url($base_url . '/' . $vsite->group->purl, array('absolute' => TRUE)),
    'Start_Date__c' => date("Y-m-d"),
    'Dev_URL__c' => url($base_url . '/' . $vsite->group->purl, array('absolute' => TRUE)),
    'Service__c' => 'Support & Hosting',
    'Service_level__c' => 'Starter Support',
    'Site_Type__c' => $site_type,
    'Visibility__c' => $visibility_text,
    'Owner_Username__c' => $author->name,
    'Owner_Full_Name__c' => $full_name,
    'Owner_Email__c' => $author->mail,
  );

  try {
    $data = $sfapi->objectCreate("Project__c", $params);
  }
  catch(SalesforceException $e) {
    watchdog_exception('hwpi', $e);
    salesforce_set_message($e->getMessage(), 'error');
    return;
  }

  if (!empty($data['errorCode'])) {
    $message = t('Failed to sync %label with Salesforce. @code: @message',
      array(
        '%label' => 'whatever',
        '@code' => $data['errorCode'],
        '@message' => $data['message'],
      )
    );
    salesforce_set_message($message, 'error');
    watchdog('hwpi', $message);
    return;
  }

  return;
}
