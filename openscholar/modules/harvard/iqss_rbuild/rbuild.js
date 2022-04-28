/**
 * Modifies the Software Project node form.
 * 
 * Only shows the rbuild fields if "Rbuild repository" is selected
 */
(function ($) {
  Drupal.behaviors.rbuild = {
    attach: function(context) {
      var inputSelector = '#edit-field-software-method-und';
      var urlFieldSelector = '#field-software-repo-add-more-wrapper';
      var shortNameFieldSelector = '#field-rbuild-short-name-add-more-wrapper';
      // Toggles Repo URL visible or hidden based on new selection
      $(inputSelector).change(function(){
        var newValue = $(this).val().toLowerCase();
        if (newValue !== 'rbuild repository') {
          $(urlFieldSelector).hide();
          $(shortNameFieldSelector).hide();
        } else {
          $(urlFieldSelector).show();
          $(shortNameFieldSelector).show();
        }
      }).trigger('change'); // Triggers change to initialize field visibility
    }
  };
})(jQuery);