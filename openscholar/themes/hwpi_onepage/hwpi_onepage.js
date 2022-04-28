jQuery(document).ready(function($) {
  //number of pixels before modifying styles
	var $menuBar = jQuery('#menu-bar');
	if ($menuBar.length) {
		var num = $menuBar.offset().top;

		// Size of the un-fixed menu.
		var menu_size = $menuBar.height();

		jQuery(window).bind('scroll', function () {
			if (jQuery(window).scrollTop() > num) {
				jQuery('#page-wrapper').css('marginTop', menu_size + 'px');
				$menuBar.addClass('fixed');
			} else {
				$menuBar.removeClass('fixed');
				jQuery('#page-wrapper').css('marginTop', '0px');
			}
		});

		jQuery('.front .block-boxes-os_sv_list_box').each(function () {
			var $this = $(this);
			var count = jQuery('.node', $this).length;

			jQuery($this).addClass('lopz-' + count);
		});
	}
});
