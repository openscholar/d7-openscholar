
(function ($) {

  /**
   * When the user is on a mobile device we need to disable the super fish menu.
   */
  Drupal.behaviors.RepsponsiveMenu = {
    attach: function (context) {

      // see https://stackoverflow.com/a/13819253/847651.
      var isMobile = {
        Android: function() {
          return navigator.userAgent.match(/Android/i);
        },
        BlackBerry: function() {
          return navigator.userAgent.match(/BlackBerry/i);
        },
        iOS: function() {
          return navigator.userAgent.match(/iPhone|iPad|iPod/i);
        },
        Opera: function() {
          return navigator.userAgent.match(/Opera Mini/i);
        },
        Windows: function() {
          return navigator.userAgent.match(/IEMobile/i) || navigator.userAgent.match(/WPDesktop/i);
        },
        any: function() {
          return (isMobile.Android() || isMobile.BlackBerry() || isMobile.iOS() || isMobile.Opera() || isMobile.Windows());
        }
      };

      if (isMobile.any()) {
        // On mobile, don't call the menu open functionality.
        $.fn.showSuperfishUl = function() {}
      }
    }
  };

})(jQuery);
