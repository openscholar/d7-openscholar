<?php
/**
 * Gpanels are drop in multi-column snippets for displaying blocks.
 * Most Gpanels are stacked, meaning they have top and bottom regions
 * by default, however you do not need to use them. You should always
 * use all the horizonal regions or you might experience layout issues.
 *
 * How to use:
 * 1. Copy and paste the code snippet into your page.tpl.php file.
 * 2. Copy and paste the region definitions to your themes .info file.
 * 3. Clear the cache (in Performance settings) to refresh the theme registry.

Region Deinitions:

; 3 col
regions[footer_top]    = 3x33 Gpanel top
regions[footer_first]  = 3x33 Gpanel left
regions[footer_second] = 3x33 Gpanel center
regions[footer_third]  = 3x33 Gpanel right
regions[footer_bottom] = 3x33 Gpanel bottom

 */
?>
<!-- Three column 3x33 Gpanel -->
<?php if (
  $page['footer_top'] ||
  $page['footer_first'] ||
  $page['footer'] ||
  $page['footer_third'] ||
  $page['footer_bottom']
  ): ?>
  <div class="at-panel gpanel panel-display footer clearfix">
    <?php print render($page['footer_top']); ?>
    <?php print render($page['footer']); ?>
    <?php print render($page['footer_first']); ?>
    <?php print render($page['footer_third']); ?>
    <?php print render($page['footer_bottom']); ?>
  </div>
<?php endif; ?>
