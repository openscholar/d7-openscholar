/**
 * @file os_slideshow_slider.js
 * 
 * Initializes responsive slides with each slideshow box's settings.
 */

(function ($) {

// Behavior to load responsiveslides
Drupal.behaviors.os_slideshow = {
  attach: function(context, settings) {
    $('body').once('os_slideshow', function() {

      var c = 0;
      for (var delta in Drupal.settings.os_slideshow) {
        var $slider = $('div#' + delta).find('.rslides');
        $slider.responsiveSlides(Drupal.settings.os_slideshow[delta]);
      }
    });

   // Adds an exact height (from the ss image) to the .slide wrapper after the image is loaded and visible
    $(window).load(function() {
      var slider = $('.slide');

      slider.children().each(function(idx, value) {
        var item = $(value);
        item.parent('.slide').css('min-height', item.find('img').height());
      });
    });
  }
}

}(jQuery));
