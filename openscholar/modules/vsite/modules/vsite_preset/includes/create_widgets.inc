<?php

/**
 * @file
 * Widgets creation task functions.
 */

/**
 * A generic task which attempts to create vsite widgets.
 */
function vsite_preset_create_widgets_task($entity, $task, &$context) {
  $vsite = vsite_get_vsite($entity->nid, TRUE);
  if (!$vsite) {
    return FALSE;
  }
  // Save the current vsite.
  $current_vsite = vsite_get_vsite();
  // Temporary switch from the current vsite to created vsite.
  spaces_set_space($vsite);

  $preset = spaces_preset_load($entity->spaces_preset_og, 'og');
  $created = vsite_preset_create_widgets($preset, $vsite);

  // Switch back the current vsite.
  spaces_set_space($current_vsite);

  return $created;
}

/**
 * Attempts to create widgets for given preset.
 *
 * @param object $preset
 *   Preset object.
 *
 * @param object $vsite
 *   VSite object.
 *
 * @return bool $created
 *   Whether or not the vsite's widgets were created.
 */
function vsite_preset_create_widgets($preset, $vsite) {
  $created = FALSE;
  $widgets = array();

  $path = DRUPAL_ROOT . '/' . drupal_get_path('module', $preset->export_module);
  $path .= '/preset_assets/' . $preset->name . '/widgets/widgets.inc';
  if (is_file($path) && is_readable($path)) {
    require_once $path;
  }
  
  foreach ($widgets as $delta => $options) {
    $options['delta'] = $delta;
    $plugin = $options['plugin'];
    unset($options['plugin']);
    if ($plugin == 'os_boxes_site_info') {
      if (isset($options['options']['site_title']) || isset($options['options']['descrip'])) {
        $wrapper = entity_metadata_wrapper('node', $vsite->group);
        if (isset($options['options']['site_title'])) {
          $wrapper->title->set($options['options']['site_title']);
        }
        if (isset($options['options']['descrip'])) {
          $wrapper->field_site_description->set($options['options']['descrip']);
        }
        $wrapper->save();
      }
      continue;
    }
    if ($plugin == 'os_boxes_manual_list') {
      $nodes = array();
      foreach ($options['options']['nodes'] as $i => $node) {
        if (!$node['nid'] && $node['alias']) {
          $nid = vsite_preset_get_nid_from_alias($node['alias']);
          if ($nid) {
            $node['nid'] = $nid;
            $nodes[] = $node;
          }
        }
      }
      if (!empty($nodes)) {
        $options['options']['nodes'] = $nodes;
      }
    }

    if ($plugin == 'os_slideshow_box') {
      $nodes = array();
      foreach ($options['options']['nodes'] as $i => $node) {
        if (!$node['nid'] && $node['image']) {
          $nid = vsite_preset_create_slide($node);
          if ($nid) {
            $nodes[$nid] = array(
              'nid' => $nid,
              'weight' => $node['weight'],
              '#weight' => $node['weight'],
            );

          }
        }
      }
      if (!empty($nodes)) {
        $options['options']['nodes'] = $nodes;
      }
    }

    if (!$box = boxes_box::factory($plugin, $options)) {
      throw new Exception(sprintf('The box %s failed to saved', $options['title']));
    }
    $vsite->controllers->boxes->set($box->delta, $box);
    $created = TRUE;
  }
  return $created;
}
