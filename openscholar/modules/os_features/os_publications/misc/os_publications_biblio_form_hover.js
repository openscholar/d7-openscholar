/**
 * jQuery for creating a popup box in the biblio node form.
 */
(function ($) {
  Drupal.behaviors.biblio_form_hover = {
    attach: function (ctx) {
      var moveLeft = 0;
      var moveDown = 0;
      $('a.biblio-pop').hover(function(e) {

        var target = '#' + ($(this).attr('data-popbox'));

        $(target).show();
        moveLeft = $(this).outerWidth();
        moveDown = ($(target).outerHeight());
      },
      function() {
        var target = '#' + ($(this).attr('data-popbox'));
        $(target).hide();
      });
    }
  }
})(jQuery);
