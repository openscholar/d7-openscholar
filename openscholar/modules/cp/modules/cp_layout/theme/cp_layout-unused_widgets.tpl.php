<?php
/**
 * @file
 * Default theme implementation to list unused widgets and create new link.
 *
 * Available variables:
 * - $tags: an array of link labels which correspond to named anchors.
 * - $children: A list of draggable elements, one for each unused widget.
 * - $factory_html: The "Add new widget" link markup.
 *
 * @todo Do something about tabs and factories
 */
?>
<div id="cp-layout-unused-widgets">
  <?php if (count($tags) > 0): ?>
    <ul id="widget-categories">
    <?php foreach ($tags as $t): ?>
      <li>
        <a href="#<?php echo $t; ?>" data-category="<?php echo $t; ?>">
          <?php echo $t; ?>
        </a>
      </li>
    <?php endforeach; ?>
    </ul>
  <?php endif; ?>
  <?php echo $factory_html; ?>
  <div id="edit-layout-unused-widgets">
    <div class="widget-container">
      <?php echo $children; ?>
    </div>
  </div>
  <div id="websiteLabelTab">Website Layout</div>
</div>