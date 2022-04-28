(function ($) {

	if (typeof Drupal.overlay != 'undefined') {
	  /**
	   * OVERRIDE overlay Event handler: if the child window suggested that the parent refresh on
	   * close, force a page refresh.
	   *
	   * @param event
	   *   Event being triggered, with the following restrictions:
	   *   - event.type: drupalOverlayClose
	   *   - event.currentTarget: document
	   */
  	  Drupal.overlay.eventhandlerRefreshPage = function (event) {
	    if (Drupal.overlay.refreshPage) {
		  if (Drupal.settings.admin_panel.keep_open) {
		    urlQueryString = document.location.search;
		    if (urlQueryString) {
		  	  document.location.search = urlQueryString + '&admin_panel=1';
		    } else {
			  document.location.search = '?admin_panel=1';
		    }
		  } else {
	        window.location.reload(true);
		  }
	    }
  	  };
	}
	
	 /**
	   * URL processing for admin panel
	   */
	  Drupal.behaviors.AdminPanelProcessURL = {
	    attach: function() {
// Commenting this out till issues with tabs loading and seeing a different URL is fixed	    	
	      //Remove Admin panel param from URL
//		  var current_url = jQuery(location).attr('href');
//		  history.pushState(null, null, current_url.replace(/\?admin_panel=1&?(.*)/, '\?$1'));
	    }
	  };
	
	


})(jQuery);




	