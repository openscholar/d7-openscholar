/**
 * jQuery behaviors for the Theme Picker form.
 */
(function ($) {
  Drupal.behaviors.cp_appearance_theme_picker = {
    attach: function (context, settings) {

      /**
       * Updates the checked status when a screenshot is clicked.
       */
      $('li.item-theme-picker').click(function(){
        // Removes the active class from every li first.
        $("ul.theme-picker").children(".item-theme-picker").removeClass('checked');
        if(!$(this).hasClass('current')){
          // Adds the class to this one.
          $(this).addClass('checked');
        }
        $("#edit-theme-default-" + $(this).attr('id').substr(6)).attr("checked", "checked").change();
      });

      /**
       * Updates the checked status when a flavor is selected from a dropdown.
       */
      $('li.item-theme-picker').find('select').change(function() {
        // Removes the active class from every li first.
        $(".item-theme-picker").removeClass('checked');
        // Then adds the class to this one.
        $(this).closest('li.item-theme-picker').addClass('checked');

        $("#edit-theme-default-" + $(this).closest('li.item-theme-picker').attr('id').substr(6)).attr("checked", "checked").change();
      });

      /**
       * Waits on form submit if flavor AJAX callback is in process.
       */
      $('#cp-appearance-theme-picker-form').submit(function(event) {
        // Adds the ajax spinner.
        if(!$('#design-submit-waiting').length) $('#edit-submit').after('<div id="design-submit-waiting" class="ctools-ajaxing" count="0"> &nbsp; </div>');

        // Loops until the ajax is done or we have waited 5 cycles.
        if($('.ctools-ajaxing:not(#design-submit-waiting)').length > 0 && $('#design-submit-waiting').attr('count') < 6){
          event.preventDefault();
          // @TODO: Replace this line with `delay` when we upgrade to jquery >= 1.4.
          $('#edit-submit').animate({ dummy: 1 }, 1000)
            .queue('fx',function(){
              $('#design-submit-waiting').attr('count',parseInt($('#design-submit-waiting').attr('count'))+1);
              $('#cp-appearance-theme-picker-form').submit();
              $(this).dequeue();
            });
        }
        return true;
      });
    }
  };

})(jQuery);
