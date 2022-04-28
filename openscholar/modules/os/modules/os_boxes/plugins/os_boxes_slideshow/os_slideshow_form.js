/**
 * Attaches a slider to the 'width' form element 
 */

Drupal.behaviors.osSlideShowSlider = {
  attach: function (ctx) {
  	var input = jQuery('#edit-size', ctx);
  	if (input.length == 0) return;
  	input[0].type = "range";
  	if (input.attr('type') == 'range') {
  		// this browser supports the new form input types
  		// set the rest of them
  		input.attr({min: 400, max: 960});
  		// and display the actual pixel width as the user changes it
  		input.change(function (e) {
  			input.parent().find('.field-suffix').html(input.val()+"px");
  		});
  		input.change();
  	}
  }
};