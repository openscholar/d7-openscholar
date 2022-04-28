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
regions[header_top]    = 3x33 Gpanel top
regions[header_first]  = 3x33 Gpanel left
regions[header_second] = 3x33 Gpanel center
regions[header_third]  = 3x33 Gpanel right
regions[header_bottom] = 3x33 Gpanel bottom

 */
?>
<!-- Three column 3x33 Gpanel -->
<?php if (
  $page['header_top'] ||
  $page['header_first'] ||
  $page['header_second'] ||
  $page['header_third'] ||
  $page['header_bottom']
  ): ?>
  <div class="at-panel gpanel panel-display header clearfix">
    <?php print render($page['header_top']); ?>
    <?php print render($page['header_second']); ?>
    <?php print render($page['header_first']); ?>
    <?php print render($page['header_third']); ?>
    <?php print render($page['header_bottom']); ?>
  </div>
<?php endif; ?>
