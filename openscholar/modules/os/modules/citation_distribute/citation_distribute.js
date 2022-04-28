/**
 * @file
 * 
 * When citation_distribute options change, this script updates the vertical tab to display new settings.
 */

(function($) {
	

Drupal.behaviors.citation_distribute_fieldset = {
  attach: function (context) {
    $('fieldset.citation-distribute-form', context).drupalSetSummary(function (context) {
    	
    	
    	var vals = [];
    	$('fieldset.citation-distribute-form input:checked')
    		.parent()
    		.children('label')
    		.each(function() {
    			vals.push( $.trim($(this).text()) );
    		})
    	
    	
    		//.parent()
    		
    		//.each( function(vals) {vals.push('1'); alert(vals)}(vals) );
    		
    	if (vals.length < 1) {
    		return Drupal.t('None');
    	} else {
    		return vals.join(', ');
    	}
    });
  }
}

})(jQuery);