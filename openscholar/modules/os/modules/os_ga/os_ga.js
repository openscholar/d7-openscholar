(function ($) {

$(document).ready(function() {

  // Expression to check for absolute internal links.
  var isInternal = new RegExp("^(https?):\/\/" + window.location.host, "i");

  // Attach onclick event to document only and catch clicks on all elements.
  $(document.body).click(function(event) {
    // Catch the closest surrounding link of a clicked element.
    $(event.target).closest("a,area").each(function() {
      // Fetches settings.
      var os_ga = Drupal.settings.os_ga;
      // Checks for download links (including query string)
      var isDownload = new RegExp("\\.(" + os_ga.trackDownloadExtensions + ")(\\?.*)?$", "i");

      // Is the clicked URL internal?
      if (isInternal.test(this.href)) {
        // Is download tracking activated and the file extension configured for
    // download tracking?
        if (os_ga.trackDownload && isDownload.test(this.href)) {
          // Download link clicked.
          var extension = isDownload.exec(this.href);
          _gaq.push(["_trackEvent", "Downloads", extension[1].toUpperCase(), this.href.replace(isInternal, '')]);
        }
      }
      else {
        if (os_ga.trackMailto && $(this).is("a[href^='mailto:'],area[href^='mailto:']")) {
          // Mailto link clicked.
          _gaq.push(["_trackEvent", "Mails", "Click", this.href.substring(7)]);
        }
        else if (os_ga.trackOutbound && this.href.match(/^\w+:\/\//i)) {
        // External link clicked.
        _gaq.push(["_trackEvent", "Outbound links", "Click", this.href]);
        }
      }
      // Is this link in a main menu?
      if (os_ga.trackNavigation) {
      if ($(this).closest('#block-os-secondary-menu').length) {
        var navType = "Secondary Nav";
        }
        if ($(this).closest('#block-os-primary-menu').length) {
          var navType = "Primary Nav";
        }
        if (navType) {
          _gaq.push(["_trackEvent", navType, "Click", this.href]);
        }
      }
    });
  });
});

})(jQuery);
