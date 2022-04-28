<?php
/**
 * @file
 * Adativetheme implementation to present a Panels layout.
 *
 * Available variables:
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout.
 * - $css_id: unique id if present.
 * - $panel_prefix: prints a wrapper when this template is used in certain context,
 *   such as when rendered by Display Suite or other module - the wrapper is
 *   added by Adaptivetheme in the appropriate process function.
 * - $panel_suffix: closing element for the $prefix.
 *
 * @see adaptivetheme_preprocess_footer()
 * @see adaptivetheme_preprocess_node()
 * @see adaptivetheme_process_node()
 */
?>
<?php print $panel_prefix; ?>
<div class="at-panel panel-display footer clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <?php if ($content['footer_top']): ?>
    <div class="region region-footer-top region-conditional-stack">
      <div class="region-inner clearfix">
        <?php print $content['footer_top']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="region region-footer-first">
    <div class="region-inner clearfix">
      <?php print $content['footer_first']; ?>
    </div>
  </div>
  <div class="region region-footer-second">
    <div class="region-inner clearfix">
      <?php print $content['footer']; ?>
    </div>
  </div>
  <div class="region region-footer-third">
    <div class="region-inner clearfix">
      <?php print $content['footer_third']; ?>
    </div>
  </div>
  <?php if ($content['footer_bottom']): ?>
    <div class="region region-footer-bottom region-conditional-stack">
      <div class="region-inner clearfix">
        <?php print $content['footer_bottom']; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
