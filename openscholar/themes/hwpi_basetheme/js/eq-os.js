(function ($) {
  Drupal.behaviors.eqOS = {
    attach: function(context) {
      if (matchMedia('only screen and (min-width: 1025px)').matches) {
        $('.region-content-first .region-inner,.region-content-second .region-inner,.region-content-bottom .region-inner').equalHeight();
      }
    }
  };
})(jQuery);
