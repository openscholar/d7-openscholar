/**
 * 
 */
Drupal.behaviors.osPublications = {
  attach: function (ctx) {
    // change the author category to the role when the form is submitted
    var $ = jQuery;
    $('.biblio-contributor-type .form-select', ctx).change(function (i) {
      var tar = $(this).parents('tr').find('.biblio-contributor-category input[type="hidden"]').not('.autocomplete');
      tar.val($(this).val());
    }).change();

    // Handle year fields.
    var codedYear = $("input[name='biblio_year_coded']");
    var yearField = $("#edit-biblio-year");

    // Add validation warning.
    var yearWarning = $("#biblio-year-group-validate");
    if (!yearField.hasClass('error')) {
      yearWarning.css('visibility', 'hidden');
    }
    yearWarning.css('color', 'red');

    // Allowed year input.
    var numbers = /^[0-9]+$/;

    // Publication year can be either given in a numerical value or by a coded
    // value ("in press", "submitted" and so on). If the user fills a numerical
    // value the radio buttons are unchecked and disabled. Clearing the
    // numerical value enables the radio buttons again.
    validate = !codedYear.val();
    yearField.keyup(function() {
      if (this.value != '' && validate) {

        // Validate year input.
        var userInput = this.value;
        if ((userInput.length != 4 && userInput.match(numbers)) || !userInput.match(numbers)) {
          yearWarning.css('visibility', 'visible');
          yearField.addClass("error");
        }
        else if (userInput.length == 4 && userInput.match(numbers)) {
          yearWarning.css('visibility', 'hidden');
          yearField.removeClass("error");
        }
      }
      else {
        yearWarning.css('visibility', 'hidden');
        yearField.removeClass("error");
      }
    });
    codedYear.change(function() {
      // Empty the year field.
      if (this.value != '') {
        // Empty year field.
        if ($("#edit-field-biblio-pub-month-und").length) {
          $('#edit-field-biblio-pub-month-und').val('_none');
          $('#s2id_edit-field-biblio-pub-month-und span:first').text('Month');
        }

        if ($("#edit-field-biblio-pub-day-und").length) {
          $('#edit-field-biblio-pub-day-und').val('_none');
          $('#s2id_edit-field-biblio-pub-day-und span:first').text('Day');
        }
      }

      if (this.value == "0") {
        validate = true;
      }
      else {
        validate = false;
      }
    });
  }
};

(function ($) {
  // Override pathauto.js implementation to change summaries of the path field.
  Drupal.behaviors.pathFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.path-form', context).drupalSetSummary(function (context) {
        var path = $('.form-item-path-alias input').val();
        var automatic = $('.form-item-path-pathauto input').attr('checked');

        if (automatic) {
          return Drupal.t('Automatic URL');
        }
        if (path) {
          return Drupal.t('URL: @alias', { '@alias': path });
        }
        else {
          return Drupal.t('No URL');
        }
      });
    }
  };

  /**
   * Override pathauto.js implementation to change summaries of the path field.
   */
  Drupal.behaviors.stipTagsFromTitleOnPaste = {
    attach: function () {
      Drupal.textPasted = false;

      CKEDITOR.on('instanceReady', function (ev) {
        var editor = CKEDITOR.instances['edit-title-field-und-0-value'];
        if (typeof editor !== 'undefined') {
          editor.on('paste', function(event) {
            event.data.dataValue = strip_tags(event.data.dataValue, '<i><sub><sup>')
          }, editor.element.$);
        }
      });

      // Stripping text from tags. Taken from phpjs library.
      function strip_tags(input, allowed) {
        allowed = (((allowed || '') + '')
          .toLowerCase()
          .match(/<[a-z][a-z0-9]*>/g) || [])
          .join('');
        // making sure the allowed arg is a string containing only tags in
        // lowercase (<a><b><c>).
        var tags = /<\/?([a-z][a-z0-9]*)\b[^>]*>/gi, commentsAndPhpTags = /<!--[\s\S]*?-->|<\?(?:php)?[\s\S]*?\?>/gi;
        return input.replace(commentsAndPhpTags, '').replace(tags, function($0, $1) {
          return allowed.indexOf('<' + $1.toLowerCase() + '>') > -1 ? $0 : '';
        });
      }
    }
  };

  /**
   * Behavior for Year of Publication radio button field.
   */
  Drupal.behaviors.yearFieldDisplay = {
    attach: function () {
      var target_extrayear = $('.form-item-biblio-year-coded-extrayear');
      var target_date_published = $('.form-item-biblio-date');
      // In edit mode, if one radio button is selected.
      if ($('input[name="biblio_year_coded"]:checked').length > 0) {
        var selectedOption = $.trim($('input[name="biblio_year_coded"]:checked').next().html());
        // todo: check the value and not the title. Could be different due to
        // tranglation.
        if (selectedOption == 'Forthcoming' || selectedOption == 'Submitted') {
          target_extrayear.hide();
          target_date_published.show();
        } else {
          target_extrayear.css({position:'relative', left: $('input[name="biblio_year_coded"]:checked').position().left + 'px', top:'-20px'});
          target_extrayear.show();
          target_date_published.hide();
        }
      } else {
        target_extrayear.hide();
      }
      // The onchange event handling for 'biblio_year_coded' radio buttons.
      $('input[name="biblio_year_coded"]').change(function(){
        var selectedOption = $.trim($(this).next().html());
        if (selectedOption == 'Forthcoming' || selectedOption == 'Submitted') {
          target_extrayear.hide();
          target_date_published.show();
        } else {
          target_extrayear.css({position:'relative', left:$(this).position().left + 'px', top:'-20px'});
          target_extrayear.show();
          target_date_published.hide();
        }
      });
    }
  };

  /**
   * Hide/shoe the year field when selection different publishing modes.
   */
  Drupal.behaviors.publishedYearHidden = {
    attach: function () {

      var changeMonthAndDay = function(hide) {
        var year = $('.form-item-biblio-year');
        var month = $("#edit-field-biblio-pub-month");
        var day = $("#edit-field-biblio-pub-day");
        var help_text = $("#biblio-month-group-validate");

        if (hide) {
          year.hide();
          month.hide();
          day.hide();
          help_text.hide();
        }
        else {
          year.show();
          month.show();
          day.show();
          help_text.show();
        }
      };

      $("#edit-biblio-year-coded-0").click(function() {
        changeMonthAndDay(false);
      });

      $("#edit-biblio-year-group input[type='radio']").not("#edit-biblio-year-coded-0").click(function() {
        changeMonthAndDay(true);
      });

      changeMonthAndDay($("#edit-biblio-year-coded-0").is(':checked') == false);
    }
  };

})(jQuery);
