<?php

/**
 * Implements hook_cp_settings().
 */
function grouper_cp_settings() {
  $forms = array();

  $user = $GLOBALS['user'];

  $forms['grouper'] = array(
    'form' => array(
      //'#access' => spaces_access_admin() && !!pinserver_authenticate_get_user_huid($user->uid),
      '#custom_directive' => 'grouper',
      '#custom_directive_parameters' => array(
        'ng-show' => "\$parent.formData.vsite_private == 6"
      ),
      '#default_value' => grouper_site_groups(),
      '#weight' => 2
    ),
    'group' => array(
      '#id' => 'privacy',
      '#title' => t('Site Visibility'),
      '#weight' => 10,
      '#help_link' => l(t('Learn more about site visibility'), 'https://help.theopenscholar.com/global-site-settings#site-visibility', array('attributes' => array('target' => '_blank'))),
      '#group_validate' => 'grouper_group_validate',
    ),
    'rest_submit' => 'grouper_save_groups',
  );

  return $forms;
}

/**
 *
 */
function grouper_save_groups($vals) {
  $space = spaces_get_space();

  $node = node_load($space->id);
  $data = array();
  foreach ($vals as $v) {
    $data[] = array(
      'value' => $v,
      'format' => null,
    );
  }
  $node->field_grouper_path[LANGUAGE_NONE] = $data;
  node_save($node);
  $cid = "field:node:$id";
  cache_clear_all($cid, 'cache_field');
  // Restful by default doesn't allow sessions to be saved
  drupal_save_session(true);
  $_SESSION['os_restful']['cache_clear_all'][] = array('field:node:'.$space->group->nid, 'cache_field');

  // Clear the cache when we switch the privacy in ALL cases except toggling between invite only (value = 1) and Harvard (value = 4).
  // For toggling, the sum of previous and selected value will be always '5' for such case; other two radio buttons values are 0 and 2.
  if (($current_vsite_access + $val) != 5 && $current_vsite_access != $val) {
    cp_clear_site_cache();
  }
}

/**
 * Validate grouper values
 */
function grouper_group_validate($values) {
  if ($values['vsite_private'] == 6 && !$values['grouper']) {
    drupal_set_message(t('You did not select any Harvard community groups.'), 'error');
    return FALSE;
  }
  return TRUE;
}