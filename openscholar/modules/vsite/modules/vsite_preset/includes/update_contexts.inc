<?php

/**
 * @file
 * Page Contexts update task functions.
 */

/**
 * A generic task which attempts to update page contexts.
 */
function vsite_preset_update_page_contexts_task($entity, $task, &$context) {
  $vsite = vsite_get_vsite($entity->nid, TRUE);
  if (!$vsite) {
    return FALSE;
  }
  // Save the current vsite.
  $current_vsite = vsite_get_vsite();
  // Temporary switch from the current vsite to created vsite.
  spaces_set_space($vsite);

  $preset = spaces_preset_load($entity->spaces_preset_og, 'og');
  foreach ($preset->value['context'] as $context => $value) {
    foreach (array('os_pages-page-', 'os_pages-section-') as $id) {
      if (strpos($context, $id) !== FALSE) {
        $alias = str_replace([$id, ':reaction:block'], '', $context);
        $nid = vsite_preset_get_nid_from_alias($alias);
        if ($nid) {
          $new_context = $id . $nid . ':reaction:block';
          $vsite->controllers->context->set($new_context, $value);
        }
      }
    }
  }

  // Switch back the current vsite.
  spaces_set_space($current_vsite);

  return TRUE;
}
