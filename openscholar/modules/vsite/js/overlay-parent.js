/**
 * @file
 * Override Overlay's module Drupal.overlay.refreshRegions().
 */

(function ($) {

/**
 * Refresh any regions of the page that are displayed outside the overlay.
 *
 * The original "Drupal.settings.basePath + Drupal.settings.overlay.ajaxCallback"
 * However that doesn't take PURL into account.
 *
 * @param data
 *   An array of objects with information on the page regions to be refreshed.
 *   For each object, the key is a CSS class identifying the region to be
 *   refreshed, and the value represents the section of the Drupal $page array
 *   corresponding to this region.
 */
  function refreshRegions(data) {
    $.each(data, function () {
      var region_info = this;
      $.each(region_info, function (regionClass) {
        var regionName = region_info[regionClass];
        var regionSelector = '.' + regionClass;
        // Allow special behaviors to detach.
        Drupal.detachBehaviors($(regionSelector));

        // This is where we override Overlay, and use our PURLified ajax callback
        // URL.
        $.get(Drupal.settings.overlay.ajaxCallback + '/' + regionName, function (newElement) {
          $(regionSelector).replaceWith($(newElement));
          Drupal.attachBehaviors($(regionSelector), Drupal.settings);
        });
      });
    });
  };

  function swap() {
    if (Drupal.overlay) {
      Drupal.overlay.refreshRegions = refreshRegions;
    }
    else {
      setTimeout(swap, 10)
    }
  }

  swap();

})(jQuery);
