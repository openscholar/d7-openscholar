<?php

/**
 * @file
 * Views handler for displaying link that creates news node based on feed item.
 */

class os_reader_handler_copy_feed_to_news extends views_handler_field {

  function render($values) {
    global $user;
    if (!$user->uid) {
      return;
    }

    $current_vsite = vsite_get_vsite();
    $feed = os_reader_feed_item_load($values->id);

    if (!empty($current_vsite)) {
      // Show button only for current VSite.
      $vsites = node_load_multiple(array($current_vsite->id));
    }
    else {
      // No current VSite, show button for all VSites.
      $groups = og_get_entity_groups();
      if (empty($groups['node'])) {
        return;
      }

      $vsites = node_load_multiple($groups['node']);
    }

    $items = array();
    foreach ($vsites as $vsite) {
      if (!og_user_access('node', $vsite->nid, 'create news content')) {
        // User does not have access to create "News" in VSite.
        continue;
      }

      if ($news_node = os_reader_get_news_by_feed_item($values->id, $vsite->nid)) {
        $generic_message = t('Imported to @vsite. !edit', array('@vsite' => $vsite->title, '!edit' => l(t('Edit'), 'node/' . $news_node->nid)));
        $vsite_message = t('Imported. !edit', array('!edit' => l(t('Edit'), 'node/' . $news_node->nid)));
        $items[] = empty($current_vsite) ? $generic_message : $vsite_message;
      }
      else {
        $token = drupal_get_token($values->id . ':' . $vsite->nid);
        $options = array(
          'attributes' => array(
            'class' => array('use-ajax'),
          ),
        );
        $copy = !empty($current_vsite) ? t('Import') : t('Import to @vsite', array('@vsite' => $vsite->title));

        $url = l($copy, 'os-reader/copy-feed-to-news/' . $values->id . '/' . $vsite->nid . '/nojs/' . $token, $options);
        $items[] = "<span id='feed-{$values->id}-{$vsite->nid}'>$url</span>";
      }
    }

    drupal_add_js(drupal_get_path('module', 'os_reader') . '/js/os_reader.js');

    return theme('item_list', array('items' => $items));
  }
}
