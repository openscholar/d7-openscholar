(function ($) {

  Drupal.behaviors.osEventsSignupDescription = {

    attach: function () {
      var repeat = $('#edit-field-date-und-0-show-repeat-settings');
      var signup = $('#field-event-registration-add-more-wrapper');

      if (repeat.is(':checked')) {
        signup.find('.description').html(Drupal.t('If checked, users will be able to sign up for <b>future</b> occurrences of this repeating event. Users will not have the ability to register for past occurrences of repeating events.'));
      }
      
      repeat.change(function() {
        if ($(this).is(':checked')) {
          signup.find('.description').html(Drupal.t('If checked, users will be able to sign up for <b>future</b> occurrences of this repeating event. Users will not have the ability to register for past occurrences of repeating events.'));
        }
        else {
          signup.find('.description').text(Drupal.t('If checked, users will be able to signup for this event.'));
        }
      });
    }
  };

})(jQuery);

