/**
 *
 */
(function ($, undefined) {

Drupal.behaviors.osLinkExternal = {
  attach: function (ctx) {

    $('.form-item-link-text input').keyup(function(event) {
      var notes = $('.form-item-link-text .notes');
      notes.remove();

      var string = event.currentTarget.value;
      var regex = /<[a-z]*>/g;
      var matches;
      var tags = [];

      while ((matches = regex.exec(string)) !== null) {
        if (matches.index === regex.lastIndex) {
          regex.lastIndex++;
        }

        var clean_tag = matches[0].replace('<', '').replace('>', '');

        if (string.indexOf('</' + clean_tag + '>') == -1) {
          tags.push(clean_tag);
        }
      }

      if (tags.length > 0) {
        var text = 'You need to close the tags ' + tags.join(', ') + ' for keeping a valid HTML markup';
        $('.form-item-link-text .description').append('<span class="notes" style="color: red; font-weight: bold;"><br/>' + text + '</span>')
      }
      else {
        notes.remove();
      }
    });

    $('#-os-link-external-form').submit(function (e) {

      if ($(this).filter(':visible').length > 0) {
        var value = $('#edit-external', this).val();
        var target_option = $('#edit-target-option', this).prop('checked');
        var link_title = $('input[name="link-title"]').val();
        // Trims the leading slash from the raw input value.
        value = value.replace(/^\//, "");
        // If given URL is relative, i.e not have 'http' and do not have '#' at the beginning, i.e not a named anchor.
        if (value.indexOf('http') == -1 && value.indexOf('#') != 0) {
          value = Drupal.settings.basePath + Drupal.settings.pathPrefix + value;
        }
        Drupal.settings.osWysiwygLinkResult = value;
        Drupal.settings.osWysiwygLinkAttributes = {'data-url': Drupal.settings.osWysiwygLinkResult, 'title': link_title};
        if (target_option) {
          Drupal.settings.osWysiwygLinkAttributes.target = '_blank';
        }
        e.preventDefault();
      }
    });
  }
};

Drupal.behaviors.osLinkInternal = {
  attach: function (ctx) {
    $('#-os-link-internal-form').submit(function (e) {
      // need to do something here to make sure we get a path and not a node title
      if ($(this).filter(':visible').length > 0) {
        Drupal.settings.osWysiwygLinkResult = $('#edit-internal', this).val();
        e.preventDefault();
      }
    });
  }
};

Drupal.behaviors.osLinkEmail = {
  attach: function (ctx) {
    $('#-os-link-email-form').submit(function (e) {
      if ($(this).filter(':visible').length > 0) {
        var val = $('#edit-email', this).val();
        if (val) {
          Drupal.settings.osWysiwygLinkResult = 'mailto:'+val;
        }
        else {
          Drupal.settings.osWysiwygLinkResult = '';
        }
        e.preventDefault();
      }
    });
  }
};

Drupal.behaviors.osLinkFile = {
  attach: function (ctx) {
    var params = Drupal.settings.media.browser.params;
    if (params) {
      if ('fid' in params) {
        $('div.media-item[data-fid="'+params.fid+'"]', ctx).click();
      }

      $('label[for="edit-filename"]', ctx).html('Search by Filename');

      $('#edit-file .form-actions input', ctx).click(function (e) {
        if ($(this).parents('#edit-file').filter(':visible').length > 0) {
          var selected = Drupal.media.browser.selectedMedia;
          if (selected.length) {
            var fid = selected[0].fid;
            var url = selected[0].url;
          }
          else {
            var fid = $('li div.selected').attr('data-fid');
            var url = $('li#media-item-'+fid).attr('data-url');
          }
          Drupal.settings.osWysiwygLinkResult = url;
          Drupal.settings.osWysiwygLinkAttributes = {"data-fid": fid};
        }
      });
    }
  }
};

Drupal.behaviors.osLinkUpload = {
  attach: function (ctx, settings) {

    Drupal.ajax.prototype.commands.switchTab = function (ajax, response, settings) {
      jQuery('#'+response.tab).data('verticalTab').tabShow();
    };

    Drupal.ajax.prototype.commands.clickOn = function (ajax, response, settings) {
      jQuery(response.target).bind('click', Drupal.media.browser.views.click).click();
    }

    $('#file-entity-add-upload input[value="Next"]').addClass('use-ajax-submit');
    Drupal.behaviors.AJAX.attach(ctx, settings);
  }
};

Drupal.behaviors.osLinkTweaks = {
  attach: function (ctx, settings) {
    $('label[for="edit-upload-upload"]', ctx).each(function () {
      $(this).addClass('add-new').html('Add New');
    });
  }
};

})(jQuery, undefined);
