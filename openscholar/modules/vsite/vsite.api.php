<?php

/**
 * @file
 * Describes API functions for Vsite module.
 */

/**
 * Implements hook_vsite_og_node_type_info().
 *
 * Tells vsite module if node types are OG "group" or "group content".
 */
function hook_vsite_og_node_type_info() {
  return array(
    'my_site_type' => 'group',
    'my_post_type' => 'group content',
    'my_other_post_type' => 'group content',
  );
}