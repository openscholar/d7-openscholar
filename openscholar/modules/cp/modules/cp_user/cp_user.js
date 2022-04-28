/**
  * @file cp_user
  *
  * Provides events to change display of select current user/create new user in cp/user/add.
  * 
  */

(function ($) {
  Drupal.behaviors.cp_user = {
    attach: function(context, settings) {
      // Check to see what the hidden value is set too.
      if($('#switch').val() === '1') {
        // Hide the user register form upon page load.
        $('#user-register-form').hide();
        // Attach the onclick event to the new-user-link link
        $('#new-user-link').click(function() {
          // Change the title of the h1 to reflect we're creating a new user.
          $('#overlay-title').text('Create and Add a New Member');
          // Hide the og-ui-add-users form
    	  $('#og-ui-add-users').hide();
    	  // Toggle the display of the user-register-form
    	  $('#user-register-form').show();
    	  // Toggle the hidden form value
    	  $('#switch').val('0');
        });
      }else {
        $('#user-register-form').show();
        $('#og-ui-add-users').hide();
      }
    }
  };
}(jQuery));