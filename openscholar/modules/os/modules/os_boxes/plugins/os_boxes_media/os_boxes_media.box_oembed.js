/**
 * This file is attached whenever an Embed Media box is used to display a Box from another site/install/whatever.
 */
(function ($) {

  /**
   * Replaces the os_boxes_media class with whichever class the source box uses
   * This is done for styling purposes
   */
  Drupal.behaviors.osBoxesMediaOembed = {
    attach: function (ctx) {
      var maps = Drupal.settings.osBoxesOembedMedia;
      if (typeof maps == 'undefined') return;

      /**
       * The maps variable contains an array with the following format
       * Local box delta => Remote box plugin key
       */
      $.each(maps, function (i, val) {
        var id = '#block-boxes-'+ i,
            clss = 'block-boxes-'+val;
        $(id).removeClass('block-boxes-os_boxes_media').addClass(clss);
      });
    }
  }

})(jQuery);