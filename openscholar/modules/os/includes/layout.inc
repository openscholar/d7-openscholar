<?php

/**
 * @file layout.inc
 * Provides layout management and crud functions.
 *
 * os_layout_get_contexts
 * Return an array of all the contexts avalible (optionally) filtered by privacy.
 *
 * os_layout_get
 * Return an array of block settings for a single context.
 *
 * os_layout_get_multiple
 * Returns an array of block settings for multiple contexts.
 *
 * os_layout_set
 * Saves block settings for a single context.
 *
 * os_layout_set_multiple
 * Saves and array of block settings for multiple contexts.
 *
 * os_layout_put
 * Helper function to add a single block to a layout.
 *
 * os_layout_load_block
 * Given a blocks full meta-data including config paths and access parameters.
 *
 * os_layout_block_delete
 * Given a bid remove all instances of that block from the layout system.
 *
 */

/**
 * Returns the public non-admin contexts that are avalible in a site.
 *
 * @param array $privacy
 *  0: disabled
 *  1: public
 *  2: private
 *
 *  TRUE returns all.
 *
 *  @param boolean $user_created
 *  Should dynamically user created contexts be returned?
 */
function os_layout_get_contexts($privacy = array(1, 2), $user_created = FALSE) {
  if (!is_array($privacy)) {
    if (!$privacy) {
      // If false was passed then return no contexts.
      return array();
    }
  }

  $os_contexts = array();
  foreach (module_implements('os_layout_contexts') as $module) {
    $contexts = module_invoke($module, 'os_layout_contexts', $privacy, $user_created);
    foreach ($contexts as $id => $name) {
      $os_contexts[$id] = $name;
    }
  }

  drupal_alter('os_layout_contexts', $os_contexts);

  return $os_contexts;
}

/**
 * Returns the blocks for a individual context
 *
 * @param $context_name
 *  The identifier string for a context.
 *
 * @param $load_meta
 *  Add meta-data used by admin forms.
 *
 * @param $unused_blocks
 *  Add all unused site blocks.
 *
 * @return
 *  List of blocks.
 */
function os_layout_get($context_name, $load_meta = FALSE, $unused_blocks = FALSE, $space = NULL) {
   // context load alters the context (for vsites) so no need to do anything
   // extra.
  $context = context_load($context_name);

  if ($space) {
    // Some values we need to set to prevent later overwrites are protected.
    $space->controllers->context->get(null, 'original');
    foreach (array_keys(context_reactions()) as $plugin) {
      // overwrite with true original values
      if (isset($context->reactions[$plugin])) {
        $space->controllers->context->values['original']["{$context->name}:reaction:{$plugin}"] = $context->reactions[$plugin];
      }
      // Now get from space.
      $override = $space->controllers->context->get("{$context->name}:reaction:{$plugin}");
      if (!is_null($override)) {
        $context->reactions[$plugin] = $override;
      }
    }
  }

  if (!is_object($context)) {
    return FALSE;
  }
  elseif (!isset($context->reactions['block'])) {
    return array();
  }

  if (array_key_exists('blocks', $context->reactions['block'])) {
    $block_settings = $context->reactions['block']['blocks'];
  }
  else {
    // No blocks set yet for this context.
    $block_settings = array();
  }

  if ($unused_blocks) {
    $site_blocks = module_invoke_all('os_widget');
    foreach ( $site_blocks as $bid => $block ) {
      // Add unused block.
      if (! isset($block_settings[$bid])) {
        $block['region'] = FALSE;
        $block_settings[$bid] = $block;
      }
    }
  }

  if ($load_meta) {
    foreach ($block_settings as $bid => $block) {
      $block_settings[$bid] = os_layout_load_block($bid, TRUE, $block);
    }
  }

  // Ensure blocks all have the required fields.
  $required = array(
    'region' => FALSE,
    'status' => 0,
    'title' => '',
    'weight' => 0,
  );
  foreach ($block_settings as $k => &$b) {
    if (os_layout_block_exists($b) && $k == $b['module'].'-'.$b['delta']) {
      $b += $required;
    }
    else {
      unset($block_settings[$k]);
    }
  }

  drupal_alter('os_layout_get', $context_name, $block_settings);

  return $block_settings;
}

/**
 * Returns the blocks for multiple contexts with child contexts overriding the
 * parents.
 *
 * @param $contexts
 *  A list of context identifiers to load and aggregate.
 *
 * @param $load_meta
 *  Add meta-data used by admin forms.
 *
 * @param $unused_blocks
 *  Add all unused site blocks.
 *
 * @return
 *  List of blocks.
 */
function os_layout_get_multiple($contexts, $load_meta = FALSE, $unused_blocks = FALSE) {
  usort($contexts, '_os_context_sort_by_hierarchy');

  $context_blocks = array();

  // Retrieve all of the blocks.
  foreach ($contexts as $context) {
    // Merge the blocks with child blocks replacing parent blocks.
    $blocks = os_layout_get($context, $load_meta, $unused_blocks);

    // If this was true then the unused blocks were all added on the first run,
    // there is no need to keep getting them also this prevents un-intended
    // overrides.
    $unused_blocks = false;

    if (is_array($blocks)) {
      $context_blocks = array_merge($context_blocks, $blocks);
    }
  }

  return $context_blocks;
}

/**
 * Saves an array blocks for a individual context.
 *
 * @param string $context_name
 * @param array $blocks
 * @param space $vsite
 */
function os_layout_set($context_name, $blocks, $vsite = NULL) {
  _os_layout_sanitize_blocks($blocks);

  $clean_up = false;

  if (!$vsite) {
    $vsite = module_exists('vsite') ? vsite_get_vsite() : NULL;
  }

  if ($context_name == 'os_public' && !variable_get('os_layout_has_cleaned_up', false)) {
    // we want to compare to the old version, which is what should be in the children contexts
    $old_blocks = os_layout_get($context_name);
    $q = null;
    if ($vsite) {
      $q = db_select('spaces_overrides', 'so')
           ->condition('type', 'og')
           ->condition('id', $vsite->group->nid)
           ->condition('object_type', 'context');
      $q   ->addExpression('DISTINCT SUBSTRING(object_id, 1, LOCATE(\':\', object_id)-1)', 'name');
      $q = $q->execute();
    }
    else {
      $q = db_select('context', 'c')
           ->fields('c', array('name'))
           ->execute();
    }
    foreach ($q as $r) {
      $ctx = $r->name;

      if ($ctx == $context_name) {
        continue;
      }

      $child_blocks = os_layout_get($ctx);

      _os_layout_adjust_relative_weight($child_blocks, $old_blocks);
      $child_blocks = _os_layout_filter_blocks($child_blocks, $old_blocks);

      os_layout_set($ctx, $child_blocks, $vsite);
    }
    $clean_up = true;
  }

  drupal_alter(array('os_layout_set'), $blocks, $context_name);

  if ($context_name == 'os_public') {
    // Unset the widgets with empty region for all contexts have region set in os_public context
    $os_public_blocks = os_layout_get('os_public');

    // Get all the contexts
    $ctxs = $vsite->controllers->context->values['space'];
    foreach ($ctxs as $ctx => $title) {
      if ((strpos($ctx, ':reaction:block') !== FALSE) && (strpos($ctx, 'os_public') === FALSE)) {

        // Get context id
        $ctx_name = str_replace(':reaction:block', '', $ctx);
        $other_context_blocks = os_layout_get($ctx_name);

        // get the common elements between os_public and other contexts
        $blocks_common = array_intersect_key($os_public_blocks, $other_context_blocks);
          if (is_array($blocks_common)) {
          // Get the array keys for those common blocks
          foreach (array_keys($blocks_common) as $blocks_common_key) {
            if (isset($os_public_blocks[$blocks_common_key]['region']) && (!isset($other_context_blocks[$blocks_common_key]['region']) || $other_context_blocks[$blocks_common_key]['region'] == '')) {
              // Blocks have got the set region will get preference
              unset($other_context_blocks[$blocks_common_key]);
              vsite_layout_context_set($ctx_name, $other_context_blocks, $vsite);
            }
          }
        }
      }
    }
  }

  if (module_exists('vsite_layout')) {
    // Writes the context to the overrides table.
    vsite_layout_context_set($context_name, $blocks, $vsite);
    if ($clean_up && $vsite) {
      $vsite->controllers->variable->set('os_layout_has_cleaned_up', true);
    }
  }
  else {
    $context = context_load($context_name);
    $context->reactions['block']['blocks'] = $blocks;
    // write the context to the database.
    context_save($context);
    if ($clean_up) {
      variable_set('os_layout_has_cleaned_up', true);
    }
  }
}

/**
 * Reverts a individual context back to defaults.
 *
 * @param $context
 *
 * @return
 *  TRUE/FALSE
 */
function os_layout_revert($context) {
  if (module_exists('vsite_layout')) {
    // Write the context to the overrides table.
    vsite_layout_context_revert($context);
    return TRUE;
  }
  else {
    // No overrides in single tenant, nothing to do.
    return FALSE;
  }
}

/**
 * Saves an array blocks to multiple contexts, resolving weight issues and
 * deciding what overrides need to be made.
 *
 * @param $contexts
 * @param $blocks
 *  An array of blocks to save to the contexts with the correct block keys.
 */
function os_layout_set_multiple($contexts, $blocks) {
  usort($contexts, '_os_context_sort_by_hierarchy');
  uasort($blocks, 'os_layout_block_sort');
  $parents = $contexts;
  $active_context = array_pop($parents);
  $parent_blocks = os_layout_get_multiple($parents);

  $removed_blocks = array();

  // try to make blocks the same as in parent contexts
  _os_layout_adjust_relative_weight($blocks, $parent_blocks);
  $child_blocks = _os_layout_filter_blocks($blocks, $parent_blocks);

  // we never edit parent contexts.
  // To edit them, load the layout form with your target context as the child.
  os_layout_set($active_context, $child_blocks);
}

/**
 * Adds/Updates an individual block within a named context.
 *
 * @param $block
 * @param $context
 */
function os_layout_put($block, $context) {
  $context_blocks = os_layout_get($context);
  // Valid Context?
  if (is_array($context_blocks)) {
    // Add new block settings
    $context_blocks[$block['module'] . '-' . $block['delta']] = $block;

    os_layout_set($context, $context_blocks);
  }
}

/**
 *
 * @param $bid
 *  Block identifier [os-primary-menu]
 *
 * @param $load_info
 *  Load Block meta-data used by admin forms
 *
 * @return
 *  Block.
 */
function os_layout_load_block($bid, $load_meta = TRUE, $block = array()) {
  list($module, $delta) = explode('-', $bid, 2);

  if ($load_meta) {
    $block_info = &drupal_static(__FUNCTION__,array());

    // Add standard information gleaned from the block_info hook
    $info_hook = "{$module}_block_info";
    if (!isset($block_info[$module]) && function_exists($info_hook)) {
      $block_info[$module] = $info_hook();
    }

    if (isset($block_info[$module][$delta])) {
      $block = array_merge($block_info[$module][$delta], $block);
    }

    // Load block install file.
    module_load_include('install', 'block', 'block');

    if (function_exists('block_schema')) {
      // Populate block with the defaults from the schema
      $block_schema = block_schema();

      foreach ( $block_schema['block']['fields'] as $field => $info ) {
        // If a default exists populate that.
        if (! isset($block[$field]) && isset($info['default'])) {
          $block[$field] = $info['default'];
        }
      }
    }

    // Get the blocks current title by rendering it.
    if (! isset($block['title']) && isset($block['delta'])) {
      $render_hook = "{$module}_block_view";
      if (function_exists($render_hook)) {
        $block_render = $render_hook($block['delta']);
        if (isset($block_render['title'])) {
          $block['title'] = $block_render['title'];
        }
      }
    }

    drupal_alter('os_widget', $block);
  }

  return $block;
}

/**
 * Remove a block from one or more contexts.
 *
 * @param $bid
 *   Block identifier [os-primary-menu].
 * @param $contexts
 *   An array of contexts or individual context to delete a block from,
 *   defaults to all contexts.
 * @param vsite $vsite
 *   The vsite object.
 */
function os_layout_block_delete($bid, $contexts = FALSE, vsite $vsite = NULL) {
  if (!$contexts) {
    $contexts = os_layout_get_contexts(TRUE);
  }
  elseif (!is_array($contexts)) {
    $contexts = array($contexts);
  }

  // Remove from each context.
  foreach ($contexts as $context => $title) {
    $blocks = os_layout_get($context);
    $set = FALSE;
    if (isset($blocks[$bid])) {
      unset($blocks[$bid]);
      $set = TRUE;
    }
    if ($set) {
      os_layout_set($context, $blocks, $vsite);
    }
  }

  return TRUE;
}

/**
 * Check to see if this block exists.
 *
 * It's module or plugin could have been deleted
 *
 * @param $block
 *  Block
 */
function os_layout_block_exists($block){
  if(isset($block['module']) && !module_exists($block['module'])){
    return FALSE;
  }
  if(isset($block['module']) && isset($block['delta']) && $block['module'] == 'boxes'){
    if (!boxes_box_load($block['delta'])) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Returns a context ranking.
 *
 * @todo Add weight to context table.
 */
function _os_context_get_rank($context_name) {
  $context_rank = &drupal_static(__FUNCTION__, array());

  if (!isset($context_rank[$context_name])) {
    $context = context_load($context_name);
    if (isset($context->weight)) {
      $context_rank[$context_name] = $context->weight;
    }
    else {
      // Global Special Contexts.
      $array = array(
        'os_public',
        'os_cp',
      );
      if (in_array($context_name, $array)) {
        $context_rank[$context_name] = 1;
        return $context_rank[$context_name];
      }

      // Set feature and vocab contexts to level 2.
      $map = features_get_component_map('context');
      if (array_key_exists($context_name, $map) || strpos($context_name,'os_taxonomy-vocab-') === 0) {
        // Feature context.
        $context_rank[$context_name] = 2;
      }
      else {
        // Page or other non-feature context.
        $context_rank[$context_name] = 3;
      }
    }
  }
  return $context_rank[$context_name];
}

/**
 * Sort a list of contexts by hierarchy
 *
 * @param $context1
 * @param $context2
 */
function _os_context_sort_by_hierarchy($context1, $context2) {
  $a = _os_context_get_rank($context1);
  $b = _os_context_get_rank($context2);

  if ($a == $b) {
    return 0;
  }
  return ($a < $b) ? - 1 : 1;
}

/**
 * Adjust the weights of blocks such that a parent's weights aren't changed unless they have to be.
 *
 * Considers parent block weights immutable, and changes the weights of other blocks to fit them
 *
 * @param $block_settings
 *  New block settings (already sorted).
 *
 * @param $previous_layout
 *  Layout to compare against (already sorted).
 *
 * @param $bid
 *  The block in question with the changed weight.
 */
function _os_layout_adjust_relative_weight(&$block_settings, $previous_layout) {

  $block_regions = _os_layout_expand_blocks($block_settings);
  $old_regions = _os_layout_expand_blocks($previous_layout);

  foreach ($block_regions as $r => &$blocks) {
    // determine if the parent blocks are in the same order in the parent and new
    $new_keys = array_keys($blocks);
    // ensure old_regions has the region
    if (!isset($old_regions[$r])) {
      $old_regions[$r] = array();
    }

    $parent_keys = array_keys($old_regions[$r]);
    // the parent keys that exist in the child
    // call array values because array_intersect preserves keys
    $only_parent_keys = array_values(array_intersect($new_keys, $parent_keys));
    // the order in the parent of keys that only exist in child
    $active_parents = array_values(array_intersect($parent_keys, $only_parent_keys));

    $bulldoze = true;
    if (empty($only_parent_keys)) {
      // none of the parent blocks are in the child
      // bulldoze is fine
    }
    elseif ($only_parent_keys === $active_parents) {
      // parent blocks are in the same order.
      // update their weights to match the parents
      // then put the new blocks in between them
      $bulldoze = false;
    }

    // we can't just bulldoze the weights
    if (!$bulldoze) {
      $min_weight = 'undefined';
      $between = array();
      foreach ($blocks as $bid => &$b) {
        if (in_array($bid, $parent_keys)) {
          // set the weights of blocks in the parent
          // these are fixed in stone as far as we're concerned
          $b['weight'] = $old_regions[$r][$bid]['weight'];
          $block_settings[$bid]['weight'] = $b['weight'];
          // go back through the children ahead of this one and set their weights
          $count = count($between);
          if ($count) {
            // handle case where children are before any parent
            if ($min_weight === 'undefined') {
              $min_weight = $b['weight'] - $count - 1;
            }
            // get the range we have to work with
            $range = $b['weight'] - $min_weight;
            // get the difference between children weights based on the number of children
            $delta = $range/($count + 1);
            // set the weights for the children
            foreach ($between as $pos => $b_id) {
              $block_settings[$b_id]['weight'] = $min_weight + ($pos+1)*$delta;
            }
          }

          $min_weight = $b['weight'];
          $between = array();
        }
        else {
          // put together a list of all children
          $between[] = $bid;
        }
      }
      // handle case where children are after all parents
      foreach ($between as $pos => $b_id) {
        $block_settings[$b_id]['weight'] = $min_weight + $pos + 1;
      }
    } // end no bulldoze processing
    // if its ok to bulldoze, we can use the integer values that are already in place
    // nothing needs to be done

  }
}

function _os_layout_expand_blocks($blocks) {
  $regions = array();
  foreach ($blocks as $bid => $b) {
    if ($b['region']) {
      $regions[$b['region']][$bid] = $b;
    }
  }

  foreach ($regions as $r => $bs) {
    uasort($regions[$r], 'os_layout_block_sort');
  }

  return $regions;
}

/**
 * Remove fields from blocks that are not part of the block schema.
 *
 * @param array $blocks
 */
function _os_layout_sanitize_blocks(&$blocks) {

  // Load block install file.
  module_load_include('install', 'block', 'block');

  if (function_exists('block_schema')) {
    // Populate block with the defaults from the schema.
    $block_schema = block_schema();

    foreach ( $blocks as $bid => $block ) {
      $blocks[$bid] = array_intersect_key($block, $block_schema['block']['fields']);
    }
  }
}

function _os_layout_filter_blocks($blocks, $parent_blocks) {
  // Fields to check for changes
  $block_fields = array(
    'region',
    'status',
    'title',
    'weight'
  );

  $child_blocks = array();
  foreach ($blocks as $bid => $b) {
    // if the block exists in a parent, check for overrides
    if (isset($parent_blocks[$bid])) {
      // dpm($bid.': weight: old: '.$parent_blocks[$bid]['weight'].' new: '.$blocks[$bid]['weight']);
      $changed = false;
      foreach ($block_fields as $field) {
        // one of the block's fields has been changed.
        if ($parent_blocks[$bid][$field] != $blocks[$bid][$field]) {
          $changed = true;
        }
      }
      // if something changed, it needs to be saved in the child context
      if ($changed) {
        $child_blocks[$bid] = $b;
      }
    }
    // if the block doesn't exist in the parent, but is still in a region.
    elseif ($b['region']) {
      // dpm($bid.': weight: new: '.$blocks[$bid]['weight']);
      $child_blocks[$bid] = $b;
    }
  }

  return $child_blocks;
}

/**
 * Sort callback.
 */
function os_layout_block_sort($a, $b){
  $aw = is_object($a) ? $a->weight : $a['weight'];
  $bw = is_object($b) ? $b->weight : $b['weight'];
  return ($aw - $bw);
}
