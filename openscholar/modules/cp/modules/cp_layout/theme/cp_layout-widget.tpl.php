<div id="<?php echo $bid; ?>" class="<?php echo implode(' ', $classes_array); ?>" ng-non-bindable>
  <?php echo $widget_title; ?>
    <div class="widget-icon"></div>
    <div class="widget-controls">
      <?php
      if ($can_edit) {
        print l('Edit', $edit_path, array('html' => TRUE, 'attributes' => array('class' => array('edit'), 'title' => 'Edit Widget')));
      }

      if ($can_delete) {
        print l('Delete', $delete_path, array('html' => TRUE, 'attributes' => array('class' => array('delete'), 'title' => 'Delete Widget')));
    } ?>
    <div class="close-this" title="Remove">Remove</div>
  </div>
</div>