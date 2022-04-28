<?php
/**
 * @file
 * os_shields.context.inc
 */

/**
 * Implements hook_context_default_contexts_alter().
 */
function os_shields_context_default_contexts_alter(&$contexts) {
  if (isset($contexts['os_public'])) {
    $fp_blocks = &$contexts['os_public']->reactions['block']['blocks'];
    $fp_blocks['boxes-shield'] = array(
        'module' => 'boxes',
        'delta' => 'shield',
        'weight' => '-10',
        'region' => 'header_third',
    );
  }
}