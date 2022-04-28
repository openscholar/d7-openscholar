
(function ($) {

  Drupal.behaviors.OsReaderOverlayRefresh = {
    attach: function (context, settings) {

      $(document).ajaxComplete(function(event, xhr, settings) {
        if (settings.url.indexOf('os-reader/copy-feed-to-news') == -1) {
          // The user didn't imported a news item. Return early.
          return;
        }

        parent.Drupal.overlay.refreshPage = true;
      });
    }
  };

  Drupal.behaviors.OsReaderExpandNews = {
    attach: function (context, settings) {

      $('.feed-item-title').unbind('click').click(function() {
        $(this).parent().find('.feed-item-description').toggle();
      });

    }
  }

})(jQuery);
