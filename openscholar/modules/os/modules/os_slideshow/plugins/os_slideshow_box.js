/**
 * Attach event to remove button in slideshow box form.
 */
Drupal.behaviors.os_slideshow_box = {
  attach : function (ctx) {
    
    var $ = jQuery;
    
    /* Events for remove links */
    var $form = $('#boxes-add-form, #boxes-box-form');

    // set up remove links.
    function setup_remove(ctx) {
      $('.remove', ctx).click(function () {
        var $this = $(this);
        $this.parents('tr').remove();             
        return false;
      });
    }
    
    setup_remove($form);
  }
};
