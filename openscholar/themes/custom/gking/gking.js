/**
 * Custom jQuery effects for gking theme.
 */
(function ($) {
  Drupal.behaviors.gking = {
    attach: function (context) {

      /**
       * Prepares Bio overlay for front page header.
       */
      // Bio set to display: none by default, for browsers without js.
      var container = $(".front .region-header-second .block-boxes-os_boxes_manual_list");
      if (container.length) {
        container.toggle();
        container.css('display','block');

      if($('a.more').length == 0) {
        // Adds necessary more/close links to show/hide bio node content.
        // Adds the "more >" link at the end of the default-visible blurb.
        $('<a class="more" href="#">More</a>')
          .appendTo('.front .region-header-second .block-boxes-os_boxes_html .boxes-box-content');
        // Adds the "close x" link on the default-hidden bio/cv overlay.
        container.find('.node-page')
          .prepend('<a class="more" href="#">CLOSE X</a>');
        
     
      }
        // Prevent the click from being bound everytime the pager is paged.
        $(".front .region-header-second a.more").unbind("click");
        // Both the "More" and "Close X" links trigger this animate event.
      $(".front .region-header-second a.more").click(function (event) {
          if (container.hasClass("bio-open")) {
            container.removeClass("bio-open").stop().animate({height: '0'}, "1500");
          } else {
            container.addClass("bio-open").stop().animate({height: '70%'}, "1500");
          }
        });
      }
      
      /**
       * Prepares the "Areas of Research" front page taxonomy widget.
       */
      var $sel = '#content-column .block-boxes-os_taxonomy_fbt .boxes-box-content';
      var areas = $($sel + ' ul li ul li:not(.aor-processed)');
      
      // Stand back! Complex jQuery effects ahead!
      if (areas.length) {

      // Updates the displayed taxonomy term item on hover event
        $($sel + ' ul li ul li:not(.aor-processed)').hover(function (event) {

          // Exits without any effect if this item is already active.
          if ($(this).hasClass('active')) {
          return;
          }
       
          // Removes active class from previous item
          var prev = $($sel + ' ul li ul li.active');
          if (prev.length) {
            prev
              .removeClass('active')
              // Hides the previous item's description
              .find('div.description')
              .fadeOut('fast');
          }
          else {
            $($sel + ' ul li ul li div.description')
              // hide everything but this one.
              // runs only once per page load
              .not($(this).find('div.description'))
              .fadeOut('fast');
          }
          // Hides the previous item's more link
          $($sel + ' .more')
            .hide();
          // Adds active class to new item, shows description with fadeIn.
          $(this)
            .addClass('active')
            .find('div.description')
            .fadeIn('fast');
        }, $.noop);
        
        // Initializes first hover event.
        var first_term = $sel + ' ul li:nth-child(2) ul li:not(.aor-processed)';
        $(first_term)
          .filter(":first")
          .each(function (index) {
            $(this).trigger('mouseover')
              .find('div.description')
              .css('opacity', '');
        });
        
        // Marks all items as processed, so this only runs once per pageload.
        $($sel + ' ul li ul li')
          .addClass('aor-processed');
      }
      
      // Moves messages (i.e. error messages) underneath main menu.
      if ($('.front #messages').length) {
        $('.front #messages').prependTo('#header-container');
      }
      // Handles non-front pages a little differently from front page.
      else if ($('#messages').length) {
        $('#messages').prependTo('#columns');
      }
      /**
       * Scrolls to the top of the page when you click the sort links
       */
//      if ($('body.page-taxonomy').length) {
//        $('a.term-admin-sort-link')
//          .click(function () {
//          window.scroll(0, 0);
//        });
//      }
    }
  };
})(jQuery);

jQuery(document).ready(function() {
  if (jQuery('#biblio-node').find('div.biblio-upload-wrapper').length) {
    jQuery('#citation-wrapper').addClass('has-attached');
  }

//REMOVE THE TITLE/TOOLTIPS FROM THE AOR BLOCK ON FRONT PAGE
  jQuery(".front ul.termchild-applications a").removeAttr("title");
  jQuery(".front ul.termchild-methods a").removeAttr("title");

});




