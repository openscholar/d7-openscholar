<div id="layout_16_9_side">
  <div class="slide">
    <ul class="rslides <?php if($display_scrollbar):?>scroll<?php endif;?>" <?php if(!empty($widget_height)):?>style="height:<?php print $widget_height;?>px;"<?php endif;?>>
      <?php print implode("\n", $slides); ?>
    </ul>
  </div>
</div>