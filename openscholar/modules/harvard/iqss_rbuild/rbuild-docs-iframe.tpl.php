<?php
// $Id$

// Includes iframe styles
drupal_add_css(drupal_get_path('module', 'rbuild') . '/rbuild.css');

/**
 * @file
 * Iframe that displays remote rbuild docs from http://r.iq.harvard.edu
 */
?>
<div id="rbuild-docs">
  <?php if (!empty($docs_url)): ?>
    <iframe name="res" scrolling="auto" src ="<?php print $docs_url; ?>">
    </iframe>
  <?php else: ?>
    <p>The requested page does not exist in the current recommended version's documentation.</p>
  <?php endif; ?>
</div>
