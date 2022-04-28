// Fixes tab behavior after using the skip link in IE or Chrome
Drupal.behaviors.osBase_skipLinkFocus = {
	attach : function(ctx) {
		var $ = jQuery, $skip = $('#skip-link a', ctx);

		$skip.click(function(e) {
			var target = $skip[0].hash.replace('#', '');

			setTimeout(function() {
				$('a[name="' + target + '"]').attr('tabindex', -1).focus();
			}, 100);
		});
	}
};

jQuery(document).ready(function() {
	jQuery(".cal-export").click(function() {
		jQuery(".attachment.attachment-before ul").slideToggle();
		jQuery(".os_events_export_links .last").slideToggle();

	});
	
	jQuery(".block-boxes-os_search_solr_search_box").addClass("block-os-search-solr");

    jQuery("figure img").each(function() {
        var imgwidth = jQuery(this).attr('width');
        jQuery(this).parent().find('figcaption').css({
            "width" : + imgwidth
        });
    });


});


(function ($) {

Drupal.behaviors.osCpLayout = {
    attach: function (ctx) {
        $('[id^="os-importer-content-"]').click(function(){
            var cp_ctrl_id = $(this).attr('id').replace(/-/g, '_');
            $('#' + cp_ctrl_id).click();
        });

        $("div.ctools-dropdown-container span").hover(
            function() { $(this).addClass('ctools-dropdown-hover'); },
            function() { $(this).removeClass('ctools-dropdown-hover'); }
        );
    }
};

    Drupal.behaviors.osKeepAspectImageMediaResponsive = {
        attach: function (context) {
            calculateMediaImageHeight();
            $( window ).resize(function() {
                calculateMediaImageHeight();
            });
            function calculateMediaImageHeight() {
                $('img.media-element.file-default', context).each(function () {
                    var image_width = $(this).width();
                    var height = getOriginalHeightValue($(this));
                    var width = getOriginalWidthValue($(this));
                    var image_parent_width = $(this).parent().width();
                    if (width !== '' && height !== '') {
                        var ratio = width / height;
                        if (ratio) {
                            $(this).height($(this).width() / ratio);
                        }
                    }
                });
            }
            // Get original height value.
            function getOriginalHeightValue(element) {
                var stored_height = element.attr('data-original_height');
                if (stored_height) {
                    return stored_height;
                }
                if (element[0] === null) {
                    return '';
                }
                var height = element[0].style.height.replace('px', '');
                if (!height) {
                    // fallback to height attribute.
                    height = element.attr('height');
                    if (!height) {
                        return '';
                    }
                }
                // Unable to write non-exists data attr with .data().
                element.attr('data-original_height', height);
                return height;
            }
            // Get original width value.
            function getOriginalWidthValue(element) {
                var stored_width = element.attr('data-original_width');
                if (stored_width) {
                    return stored_width;
                }
                if (element[0] === null) {
                    return '';
                }
                var width = element[0].style.width.replace('px', '');
                if (!width) {
                    // fallback to width attribute.
                    width = element.attr('width');
                    if (!width) {
                        return '';
                    }
                }
                // Unable to write non-exists data attr with .data().
                element.attr('data-original_width', width);
                return width;
            }
        }
    };

})(jQuery);