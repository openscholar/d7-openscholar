<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if (!$label_hidden): ?>
    <div class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</div>
  <?php endif; ?>
  <div class="field-items"<?php print $content_attributes; ?>>
      <?php foreach ($items as $delta => $item): ?>
      <div class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"<?php print $item_attributes[$delta]; ?>>
      <figure>
        <?php print render($item); ?>
        <?php if(!empty($item['#item']['os_file_description'][LANGUAGE_NONE][0]['value'])): ?>
         <figcaption>
           <?php print $item['#item']['os_file_description'][LANGUAGE_NONE][0]['value'];?>
         </figcaption>
        <?php endif;?>
      </figure>
      </div>
      <?php endforeach; ?>
  </div>
</div>