/**
 * @file os_slideshow_flexslider.js
 * 
 * Initializes flexslider javascript, attaches back, pause, next, pager functions
 */

(function ($) {

// Behavior to load FlexSlider
Drupal.behaviors.os_slideshow = {
  attach: function(context, settings) {
    function slider_closure(selector) {
      return function() {
        return $(selector);
      }
    }
    
    //event to mark active page in pager
    function markpage(e) {
      var slider = e.data().flexslider 
      var page = slider.currentSlide      
      $(slider).find('.flex-page-link').removeClass('active')
      $(slider).find('#flex-page-'+page).addClass('active')
    }
    
    $(window).load(function() {
      for (delta in Drupal.settings.os_slideshow) {
        //start slideshow
        var div = 'div#' + delta;
        var slider = $(div + ' .flexslider');        
        Drupal.settings.os_slideshow[delta]['after'] = markpage;
        slider.flexslider(Drupal.settings.os_slideshow[delta]);

        //bind controls 
        slider.find('#flex-next').click(function() { $(this).closest('.flexslider').flexslider('next') });
        slider.find('#flex-prev').click(function() { $(this).closest('.flexslider').flexslider('prev'); });
        slider.find('#flex-pause').click(function() {
          var slider = $(this).closest('.flexslider');
          if (slider.closest('.flexslider').hasClass('pause')) {
            slider.removeClass('pause').flexslider('play');
          } else {
            slider.addClass('pause').flexslider('pause');
          }
        });

        //closure for jump to page functions
        function pager_closure(page) {
          return function() {
            $(this).closest('.flexslider').flexslider(page);
          }
        }
        
        //bind pager functions
        for (var i=0; i<slider.find('img').length; i++) {
          var fn = pager_closure(i);
          slider.find('#flex-page-'+i).click(fn);        
        }
        
        //mark current pager page
        var page = (slider.currentSlide === undefined) ? 0 : slider.currentSlide      
        slider.find('#flex-page-'+page).addClass('active')
        console.log(page);
      }
    });
  }
};

}(jQuery));
