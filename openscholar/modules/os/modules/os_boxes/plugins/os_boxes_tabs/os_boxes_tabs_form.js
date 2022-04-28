(function ($) {

Drupal.behaviors.tabs = {
  attach: function () {

  	// initialize the tabs and the sortable
  	$('#tabs').tabs({
			// keep the new_tab link from being a tab
			disabled: [$('#tabs .links li').length-1],
			// use fieldset instead of div
			panelTemplate: '<fieldset class="panel"></fieldset>',
			activate: function (e, ui) {
        $('.panel .tab-title').change();
      }
    })
    .find('.ui-tabs-nav')
    .sortable({
			axis: 'x',
			// don't let them sort the new_tab
			items: 'li:not(.new_tab)',
			update: updateWeightAndDefault
	  });
  	
  	// update the weights of the tabs
  	// should end up being unique
  	function updateWeightAndDefault() {
  		var first = true;
  		$('#tabs .links:not(.new_tab) a').each(function (i) {
  			if (first) {
  				$('#edit-default').val($(this).attr('href').replace('#tab-',''));
  				first = false;
  			}
  			$(this.hash).find('.tab-weight').val(i);
  		});
  	}
  	
  	// hide the tab title selection when there's no widget selected
  	// ctools dependency would've had a good deal of overhead
  	// since I can't tell it to display something when not this value
  	$('.ui-tabs-panel .tab-delta').live('change', function () {
  		var $this = $(this);
  		if ($this.val() != 'os_boxes_tabs-remove') {
  			$('.tab-title', $this.parents('.ui-tabs-panel')).parent().show();
  		}
  		else {
  			$('.tab-title', $this.parents('.ui-tabs-panel')).parent().hide();
  		}
  	});
  	
  	// update the title of the tab
  	$('.ui-tabs-panel .tab-title').live('change', function () {
  		var $this = $(this),
  			id = $this.parents('.ui-tabs-panel').attr('id');
  		$('#tabs .links a[href="#'+id+'"]').text($this.val());
  	});
  	
  	// add a new tab when the new_tab is clicked on
  	$('#tabs .links a[href="#tab-new"]').click(function () {
  		var id = 'tab-'+($('#tabs .links li').length-1),
          panel = $('#tab-new').html().replace(/tab-new/g, id);

      $('#tabs .ui-tabs-nav .new').before('<li><a href="#'+id+'">New Tab</a></li>');
      $('#tabs').append('<fieldset id="'+id+'" class="form-wrapper">'+panel+'</fieldset>');
  		$('#tabs').tabs('refresh');
  
  		// get the full html, including tags of the panel 
  		// and replace the automatically generated panel with it
  		$('#tabs').tabs('option', 'active', -2);
      $('#tabs').tabs('option', 'disabled', [$('#tabs .links li').length-1]);
  		var count = $('#edit-count');
  		count.val(parseInt(count.val())+1);
  		
  		$('#'+id+' .tab-title').parent().hide();
  		
  		updateWeightAndDefault();
  	});
  }
};
})(jQuery);