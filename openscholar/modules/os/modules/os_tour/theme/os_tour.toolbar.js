(function ($) {
  Drupal.behaviors.os_notifications = {
  attach: function (context) {

    // Setup.
    var menuLinkSel = '#os-tour-notifications-menu-link';
    if ($(menuLinkSel + '.os-notifications-processed').length) {
      return;
    }
    $(menuLinkSel).attr('href', '#').text('').addClass('os-notifications-processed');
    var settings = Drupal.settings.os_notifications;
    if (typeof google == 'undefined') {
      return;
    }

    // @TODO: Add support for multiple feeds.
    var feed = new google.feeds.Feed(settings.url);
    var items = [];
    feed.setNumEntries(settings.max);
    feed.load(function (result) {
      if (!result.error) {
        for (var i = 0; i < result.feed.entries.length; i++) {
          var num_remaining = (result.feed.entries.length - i);
          var entry = result.feed.entries[i];
          var target = document.querySelector("#os-tour-notifications-menu-link");
          if (osTour.notifications_is_new(entry)) {
            var item = osTour.notifications_item(entry, num_remaining, '#os-tour-notifications-count', target);
            items.push(item);
          }
        }

        // Only continues if we have the hopscotch library defined.
        if (typeof hopscotch == 'undefined') {
          return;
        }

        // If there are items to display in a hopscotch tour...
        if (items.length) {
          // Sets up the DOM elements.
          $(menuLinkSel).append($("<i class='os-tour-notifications-icon'/>"));
          $(menuLinkSel).append($("<span id='os-tour-notifications-count'/>"));
          osTour.notifications_count('#os-tour-notifications-count', items.length);
          $('#os-tour-notifications-menu-link').slideDown('slow');
          // Sets up the tour object with the loaded feed item steps.
          var tour = {
            showPrevButton: true,
            scrollTopMargin: 100,
            id: "os-tour-notifications",
            steps: items,
            onEnd: function() {
              osTour.notifications_count('#os-tour-notifications-count', -1);
              osTour.notifications_read_update();
            }
          };

          // Adds our tour overlay behavior with desired effects.
          $('#os-tour-notifications-menu-link').click(function() {
            $('html, body').animate({scrollTop:0}, '500', 'swing', function() {
              $('.hopscotch-bubble').addClass('animated');
              hopscotch.startTour(tour);
              // Removes animation for each step.
              $('.hopscotch-bubble').removeClass('animated');
              // Allows us to target just this tour in CSS rules.
              $('.hopscotch-bubble').addClass('os-tour-notifications');
            });
          });
        }
      }
    });
  }
};

})(jQuery);
