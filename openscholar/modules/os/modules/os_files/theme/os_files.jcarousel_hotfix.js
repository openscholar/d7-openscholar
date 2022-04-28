/**
 @file

 Fix next pager button on first load.
 */
(function ($) {

    Drupal.behaviors.osFilesjCarouselHotfix = {
        attach: function (ctx) {
            window.dispatchEvent(new Event('resize'));
        }
    }

})(jQuery);