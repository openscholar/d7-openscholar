<?php 
/**
 * Slides and their captions
 */

?>

<li>
  <?php print $image; ?>
  <?php if ($headline || $description): ?>
    <div class="caption slide-copy">
      <?php if ($headline): ?><h2><?php print $headline; ?></h2><?php endif; ?>
      <?php if ($description): ?><p><?php print $description; ?></p><?php endif; ?>
    </div>
  <?php endif; ?>
</li>

