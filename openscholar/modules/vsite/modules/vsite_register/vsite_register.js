/**
 * @file
 * vsite_register.js
 */

(function($) {

  /**
   * Provides events to change display of select current user/create new user in
   * vsite registration form.
   */
  Drupal.behaviors.vsite_register = {

    /**
     * Swaps display of the new/existing user form elements.
     *
     * This is determined by new user's display state. So when both elements are
     * showing, toggling will hide one and show the other.
     */
    toggle_user_forms: function() {
      var new_user = $('#new_user_div');
      var old_user = $('#edit-existing-username').parent();

      if (new_user.css('display') == 'none') {
        new_user.css('display', 'block');
        old_user.css('display', 'none');
      } else {
        new_user.css('display', 'none');
        old_user.css('display', 'block');
      }
    },

    /**
     * Attaches a click event to call toggle_user_forms.  Tries to do this only once by inspecting $elem.data.  Ajax calls
     * will trigger attach again, causing multiple instances of the same click event to be registered if this isn't done.
     */
    attach: function(context, settings) {
      // Ensures that focus is not on the "domain" element to the avoid AJAX errors.
      $('#edit-submit').mouseenter(function() {
        $(this).focus();
      });

      // Attaches click event to new user link.
      // only do this once, even if this behavior happens again.
      if ($('#new-user-link').length) {
        // Uses context and jQuery.once() to prevent extra attach events.
        // @see https://drupal.org/node/756722
        // @see Drupal.attachBehaviors()
        $('#new-user-link', context).once('toggleUserForms', function() {
          // Initializes the new user & existing user display state.
          Drupal.behaviors.vsite_register.toggle_user_forms();
          // Toggles display whenever the "Create new user" link is clicked.
          $('#new-user-link').click(function() {
            Drupal.behaviors.vsite_register.toggle_user_forms();
            $('input[name=create_new_user]').attr('value', 1); //store this so form appears right after refresh
          });
        });
      }

      // Hides the ajax error that comes up when an ajax call is left dangling.
      // @see http://drupal.org/node/1232416
      $.ajaxSetup({
        beforeSend: function(jqXHR, settings) {
          settings.error = function(jqXHR, textStatus, errorThrown) {};
        }
      });

      // Avoids setting up the hidden element on every page load.
      Drupal.detachBehaviors($(this));
    }

  };

})(jQuery);