Drupal.behaviors.shields = {
  attach: function(ctx) {
    var $ = jQuery;
    $('.shield_wrapper .form-radio').hide();
    
    $('input[checked="checked"]').parents('.form-type-radio').find('.item-shield-picker').addClass('active');

    $('.form-type-radio').click(function(){
    	// remove the active class from every li first
    	$('.item-shield-picker').removeClass('active');
    	$('.item-shield-picker', this).addClass('active');

    	$('input[name="shield"]').removeAttr('checked');
    	$('input[name="shield"]', this).attr('checked', true);
    });
  }
};
