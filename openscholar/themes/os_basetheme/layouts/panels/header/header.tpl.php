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
 * @see adaptivetheme_preprocess_header()
 * @see adaptivetheme_preprocess_node()
 * @see adaptivetheme_process_node()
 */
?>
<?php print $panel_prefix; ?>
<div class="at-panel panel-display header clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <?php if ($content['header_top']): ?>
    <div class="region region-header-top region-conditional-stack">
      <div class="region-inner clearfix">
        <?php print $content['header_top']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="region region-header-first">
    <div class="region-inner clearfix">
      <?php print $content['header_first']; ?>
    </div>
  </div>
  <div class="region region-header-second">
    <div class="region-inner clearfix">
      <?php print $content['header_second']; ?>
    </div>
  </div>
  <div class="region region-header-third">
    <div class="region-inner clearfix">
      <?php print $content['header_third']; ?>
    </div>
  </div>
  <?php if ($content['header_bottom']): ?>
    <div class="region region-header-bottom region-conditional-stack">
      <div class="region-inner clearfix">
        <?php print $content['header_bottom']; ?>
      </div>
    </div>
  <?php endif; ?>
</div>
<?php print $panel_suffix; ?>
