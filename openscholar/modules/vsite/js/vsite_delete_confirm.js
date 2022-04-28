(function ($) {

  Drupal.behaviors.vsiteDeleteConfirm = {
    attach: function (ctx) {
      $('#edit-cancel').click(function (e) {
        if (typeof Drupal.overlayChild != 'undefined') {
          e.preventDefault();
          e.stopPropagation();
          parent.Drupal.overlay.close();
        }
      });
    }
  }

})(jQuery);