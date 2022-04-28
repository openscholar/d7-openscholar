<?php
// $Id$


/**
 * Exposes OpenScholar blocks as context reactions.
 */
class os_context_reaction_block extends context_reaction_block {

  /**
   * An alternative version of block_list() that provides any context enabled
   * blocks.
   */
  function block_list($region) {
    module_load_include('module', 'block', 'block');
    ctools_include('layout', 'os');

    $context_blocks = &drupal_static('context_reaction_block_list');
    $contexts = context_active_contexts();

    if (!isset($context_blocks)) {
      $info           = $this->get_blocks();
      $context_blocks = array();
      $all_blocks     = array();
      $context_order  = array_keys($contexts);
      @usort($context_order, '_os_context_sort_by_hierarchy');

      foreach ($context_order as $key) {
        $context = $contexts[$key];
        $options = $this->fetch_from_context($context);
        if (!empty($options['blocks'])) {
          foreach ($options['blocks'] as $block) {
            $block          = (object) $block;
            $block->context = $context->name;
            $block->bid     = "{$block->module}-{$block->delta}";
            $block->title   = isset($info[$block->bid]->title) ? $info[$block->bid]->title : NULL;
            $block->cache   = isset($info[$block->bid]->cache) ? $info[$block->bid]->cache : DRUPAL_NO_CACHE;
            // Allows child contexts to override parents.
            $all_blocks[$block->bid] = $block;
          }
        }
      }

      // Put blocks into regions.
      foreach ($all_blocks as $block) {
        $context_blocks[$block->region][$block->bid] = $block;
      }

      $this->is_editable_check($context_blocks);
      foreach ($context_blocks as $block_region => $blocks) {
        // Skips blocks that are in no region.
        if (!$block_region) {
          continue;
        }
        $context_blocks[$block_region] = _block_render_blocks($blocks);

        // Makes blocks editable if allowed.
        if ($this->is_editable_region($block_region)) {
          foreach ($context_blocks[$block_region] as $key => $block) {
            $context_blocks[$block_region][$key] = $this->editable_block($block);
          }
        }

        // Sorts blocks.
        uasort($context_blocks[$block_region], array('context_reaction_block', 'block_sort'));
      }
    }
    return isset($context_blocks[$region]) ? $context_blocks[$region] : array();
  }
}

