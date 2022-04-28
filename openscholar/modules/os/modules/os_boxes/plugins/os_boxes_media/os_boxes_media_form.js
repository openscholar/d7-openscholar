/**
 * Wires the form controls to submit to the tab form
 */
Drupal.behaviors.box_media = {
	attach: function (ctx) {
		jQuery('.form-actions', ctx).css({display: 'none'});	// so it hides already hidden elements
		var fid = jQuery('input[name="fid"]', ctx).val();
		if (fid) {
			jQuery('a[data-fid="'+fid+'"] .media-item', ctx).click();
			jQuery('a[href="#media-tab-media_default--media_browser_1"]').click();
		}
		
		jQuery('.form-submit', ctx).filter(':visible').click(function (e) {
			var panel = jQuery('.media-browser-tab').not('.ui-tabs-hide'),
				title = jQuery('input[type="text"][name="title"]').val(),
				descr = jQuery('input[type="text"][name="description"]').val();
			panel.find('input[name="title"]').val(title);
			panel.find('input[name="description"]').val(descr);
			
			e.preventDefault();
			panel.find('.form-submit').click();
		});
	}
};