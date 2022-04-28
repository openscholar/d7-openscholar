/**
 * Pairs a link and a block of content. The link will toggle the appearance of
 * that block content
 */
 
(function ($) {
  Drupal.behaviors.os_toggle = {
    attach: function (ctx) {
      $('.toggle', ctx).once(function() {
        $(this, ctx).click(function(event) {
          event.preventDefault();

          $(this).toggleClass("expanded");
          var slider = null;

          $(this).add($(this).parents()).each(function () {
            var potentials = $(this).siblings('.os-slider');
            if (potentials.length) {
              slider = $(potentials[0]);
            }
          });

          if (navigator.appName == 'Microsoft Internet Explorer' && navigator.userAgent.match(/msie 6/i)) {
            // IE8 Does not work with the slider.
            if ($(this).hasClass('expanded')) {
              slider.show();
            }
            else {
              slider.hide();
            }
          }
          else {
            slider.slideToggle("fast");
          }
        });
      });
    }
  };

  Drupal.behaviors.os_showterms = {
  attach: function (ctx) {
  // Configure/customize these variables.
  var moretext = "More <span>&#x25BC;</span>";
  var lesstext = "Less <span>&#x25B2;</span>";

    $('.more').once(function() {
      var content = $(this).html();
      var html = content + '<span>,</span>&nbsp;&nbsp;<a  class="morelink togglemore">' + moretext + '</a>';
      $(this).html(html);
    });

    $(".morelink").once().click(function(){
        if($(this).hasClass("toggleless")) {
            $(this).removeClass("toggleless");
            $(this).addClass("togglemore");
            $(this).html(moretext);
        } else {
            $(this).removeClass("togglemore");
            $(this).addClass("toggleless");
            $(this).html(lesstext);
        }
        $(this).prevAll(".morecontent").children(".morechildren").toggle();
        return false;
      });
    }
  };
})(jQuery);