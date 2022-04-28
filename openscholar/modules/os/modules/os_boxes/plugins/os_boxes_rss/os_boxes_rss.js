(function ($) {
  Drupal.behaviors.osBoxesRss = {
    attach: function (ctx) {
      $('#rss_feed').hide();
      $('.feed-icon').click(function (e) {
        var rss = document.createElement("input");
        rss.setAttribute("value", $('#rss_feed').html());
        document.body.appendChild(rss);
        rss.select();
        document.execCommand("copy");
        document.body.removeChild(rss);
        $('#rss_label').html('Feed URL copied to clipboard');
      });
    }
  }

})(jQuery);