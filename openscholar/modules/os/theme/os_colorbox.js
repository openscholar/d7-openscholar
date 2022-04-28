/**
 * 
 */
Drupal.behaviors.osColorbox = {
  attach: function (ctx) {
    jQuery(document).bind('drupalOverlayOpen', function () {
      jQuery.colorbox.close();
    });
  }
};

jQuery(document).ready(function(){
  jQuery("a.media-gallery-thumb").each(function() {
    var largeUrl = jQuery(this).attr('href');
    var $t = jQuery(this);
    jQuery.ajax({
      url: largeUrl,
      success: function(data) {
        var regex = /src="(.*?)"/;
        var srcArray = regex.exec(data.toString());
        if (typeof srcArray != 'undefined') {
          var src = srcArray[1];
          $t.append('<div class="hideclass"><img src="' + src + '" /></div>');
          $t.children('.hideclass').hide();
        }
      },
    });
  });
});