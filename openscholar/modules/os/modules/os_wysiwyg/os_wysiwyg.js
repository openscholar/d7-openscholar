(function($) {
  /**
   * Hides the text format help when the wysiwyg is disabled
   */
  Drupal.behaviors.osWysiwygHideTips = {
    attach: function (context) {
      // don't do this for every thing
      function toggle (e) {
        $(e.currentTarget).parents('.text-format-wrapper').find('.filter-wrapper').toggle();
      }
      $('.filter-wrapper', context).hide();
      $('.wysiwyg-toggle-wrapper a', context).off('click', toggle).on('click', toggle);
    }
  };

  Drupal.behaviors.osWysiwygBrowserAutoSubmit = {
    attach: function (ctx) {

      Drupal.media.popups.mediaStyleSelector.mediaBrowserOnLoad = function (e) {
        var doc = $(e.currentTarget.contentDocument);
        if ($('#edit-format option', doc).length == 1) {
          $('#edit-format', doc).parent('.form-item').hide();
        }

        if ($('#media-format-form fieldset#edit-options .fieldset-wrapper *:visible', doc).length == 0) {
          e.currentTarget.contentWindow.Drupal.media.formatForm.submit.apply($('#media-format-form a.fake-ok', doc)[0]);
        }
      };
    }
  };

  Drupal.behaviors.osWysiwygImagePropertiesDialog = {
    attach: function (ctx) {
      if (CKEDITOR.osWysiwygImagePropertiesDialog == undefined) {
        CKEDITOR.osWysiwygImagePropertiesDialog = true;

        CKEDITOR.on('dialogDefinition', function (ev) {
          var dialogName = ev.data.name,
            dialogDefinition = ev.data.definition;

          if (dialogName == 'image') {
            var infoTab = dialogDefinition.getContents('info');
            console.log(dialogDefinition);
            //infoTab.remove('txtWidth');
            //infoTab.remove('txtHeight');
            //infoTab.remove('ratioLock');
            //infoTab.remove('cmbAlign');
            //infoTab.remove('htmlPreview');
            //cleanUpDialog(infoTab.elements);
            console.log(infoTab.elements);

            var advTab = dialogDefinition.getContents('advanced');
            advTab.remove('txtdlgGenStyle');

            dialogDefinition.removeContents('Link');
            var onShow = dialogDefinition.onShow;
            dialogDefinition.onShow = function () {
              onShow.call(this);
              this.getContentElement('info', 'txtUrl').disable();
            }
          }
        });
      }
    }
  }

  /*function cleanUpDialog(elements) {
    var splice = [];
    if (!elements) return;
    for (var i = 0, l = elements.length; i < l; i++) {
      if (elements[i].children == undefined) {
        continue;
      }
      else if (elements[i].children.length == 0) {
        splice.push(i);
      }
      else {
        cleanUpDialog(elements[i].children);
        if (elements[i].children.length == 1) {
          if (elements[i].type == 'vbox' || elements[i].type == 'hbox') {
            elements.splice(i, 1, elements[i].children[0]);
          }
        }
      }
    }
    while (splice.length) {
      i = splice.pop();
      elements.splice(i, 1);
    }
  }*/

})(jQuery);
