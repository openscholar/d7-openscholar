(function ($) {
    /**
     * Behavior for visibility change message display after changing visibility from 'Public' to anything else
     */
    Drupal.behaviors.vsite_access = {
        attach: function (context, settings) {
            var stored_site_visibility = $('input[name="vsite_private"]:checked').val();
            $('input[name="vsite_private"]').change(function(){
                // If stored settings has public visibility as value '0', then checking the selected value is non-zero or not.
                // Depending upon that, then message will be displayed.
                if ($(this).val() != 0 && stored_site_visibility == 0) {
                    $('#site-visibility-help-text').css({display:'block'});
                }
                else {
                    $('#site-visibility-help-text').hide();
                }
            });
        }
    };
})(jQuery);