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

; 2 col
regions[content_top]    = 2x50 Gpanel top
regions[content_first]  = 2x50 Gpanel left
regions[content_second] = 2x50 Gpanel right
regions[content_bottom] = 2x50 Gpanel bottom

 */
?>
<!-- Two column 2x50 -->
<?php if (
  $page['content_top'] ||
  $page['content_first'] ||
  $page['content_second'] ||
  $page['content_bottom']
  ): ?>
  <div class="at-panel gpanel panel-display content clearfix">
    <?php print render($page['content_top']); ?>
    <?php print render($page['content_first']); ?>
    <?php print render($page['content_second']); ?>
    <?php print render($page['content_bottom']); ?>
  </div>
<?php endif; ?>
