/**
 * @file os_publications_autopager.js
 * 
 * Initializes autopager library for infinite scrolling.
 * http://lagoscript.org/jquery/autopager
 */

(function($) {
  
  Drupal.behaviors.os_infinite_scroll = {
    attach: function (context, settings) {
      $('body').once('os_infinite_scroll', function() {
        if (typeof(jQuery.autopager) == 'function') {
          for (var mod in Drupal.settings.autopager) {
            
            // load and start should be able to call several small functions instead of requiring us to bloat a single function
            function _load() {
              for (var f in settings._load) {
                if ((typeof(settings._load[f]) == 'function') && (settings._load[f] != _load)){
                  settings._load[f]();
                }
              }
              return true;
            }
  
            
            function _start() {
              for (var f in settings._start) {
                if ((typeof(settings._start[f]) == 'function') && (settings._start[f] != _start)){
                  settings._start[f]();
                }
              }
              return true;
            }
            
            //show and hide the thobber while ajax is loading
            function show_throbber(context) {
              $('.jquery-autopager-ajax-loader').remove();
              $(settings.content + ':last').after(img);
            }
            
            function hide_throbber() {
              $('.jquery-autopager-ajax-loader').remove();
            }
            
            //hide the pager.  each page load can add a new one.
            function hide_pager() {
              if (settings.pager) {
                $(settings.pager).hide();
              }
            }

            //hide redundant biblio headers
            function os_publications_hide_biblio_category_bar() {
              selector = '.biblio-separator-bar';
              $selectors = $(selector);

              $selectors.each(function(selectors) {
                html = $(this).html()
                $selectors.filter(function() {
                  return $(this).text() === html;
                }).not(':first').remove();
              });
            }

            //prep settings
            var settings = Drupal.settings.autopager[mod]
            if (settings.loading_image) {
              var img = '<div class="jquery-autopager-ajax-loader" style="text-align:center;"><img src="' + settings.loading_image + '" alt="loading..."/></div>';
            }
            
            settings._load = [hide_throbber, hide_pager, Drupal.attachBehaviors];     
            if (settings.hide_biblio_categories) {
              settings._load.push(os_publications_hide_biblio_category_bar);
            }
            settings.load = _load;
            
            settings._start = [show_throbber]; 
            settings.start = _start;
            
            //init
            var $pager = $.autopager(settings);
          }
          
          //hide the pager.  this also happens after each load
          if (settings.pager) {
            $(settings.pager).hide();
          }
        }
      });
    }
  }

})(jQuery);
