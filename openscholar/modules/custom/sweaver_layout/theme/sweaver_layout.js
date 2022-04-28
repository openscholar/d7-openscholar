(function ($) {
Drupal.behaviors.sweaver_layout = {
	attach: function (ctx) {
		// don't do this multiple times
		var $panel = $('#sweaver_layout', ctx);
		if ($panel.length == 0 || $panel.hasClass('.swl-processed')) return;
		$panel.addClass('swl-processed');
		
		$('#sweaver-tabs a').not('#tab-sweaver_layout a').click(disableEditing);
		$('#tab-sweaver_layout a').click(enableEditing);
		
		if (Drupal.Sweaver.activeTab == "tab-sweaver_layout" && open == "true") {
			enableEditing();
		}
	}
};

function enableEditing() {
	if (Drupal.Sweaver.open == "true") return;	// backwards. this is changed after our click handler runs
	
	$this = $('#edit-widgets');
	var id = $this.attr('id');
    Drupal.contextBlockEditor = Drupal.contextBlockEditor || {};
    $this.bind('init.pageEditor', function(event) {
      Drupal.contextBlockEditor[id] = new DrupalContextBlockEditor($(this));
    });
    $this.bind('start.pageEditor', function(event, context) {
      // Fallback to first context if param is empty.
      if (!context) {
        context = $(this).data('defaultContext');
      }
      Drupal.contextBlockEditor[id].editStart($this, context);
    });
    $this.bind('end.pageEditor', function(event) {
      Drupal.contextBlockEditor[id].editFinish();
    });
}

function disableEditing() {
	
}
})(jQuery);