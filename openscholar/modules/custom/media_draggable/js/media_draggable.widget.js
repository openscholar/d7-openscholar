/**
 * Required interactions:
 * * Clicking Browse button opens Media Browser, which returns a mediaFile object
 * * Dragging files to Drop region uploads and returns a mediaFile object
 * * Doing either of those things adds row to the list below the drop region
 * * Editing a file works, and should update the filename if necessary
 * * Removing a file removes it from the list
 * *
 * * Adding and Removing files both save properly to the node
 */

(function ($) {
  var template;

  function openBrowser(settings) {
    Drupal.media.popups.mediaBrowser(function (files) {
      $.each(files, function (k, v) {
        addRow(v);
      })
    }, settings.global);
  }

  function browserClickHandler(e) {
    var id = $(e.currentTarget).parents('.field-widget-media-draggable-file').attr('id'),
      settings = Drupal.settings.mediaDraggable.elements[id];

    openBrowser(settings);

    e.stopImmediatePropagation();
    return false;
  }

  function removeFile(e) {
    $(e.currentTarget).parents('.file-list-single').remove();

    e.preventDefault();
    return false;
  }

  function addRow(file) {
    var str = template.html(),
      id = template.attr('id'),
      count = $('.file-list-single').length - 1;

    str = str.replace('template', count);
    str = str.replace('/0/', '/'+file.fid+'/');
    str = str.replace('value="0"', 'value="'+file.fid+'"');
    str = str.replace(' blank', ' '+file.filename);
    id = id.replace('template', count);

    var $row = $(str),
      icon = file.icon;
    $row.find('img.file-icon').attr('src', icon);
    $row.find('.ctools-use-modal-process').removeClass('ctools-use-modal-processed');
    $row.find('.ajax-processed').removeClass('ajax-processed');
    var $wrapper = $row.wrapAll('<div class="file-list-single form-wrapper media-draggable-processed" id="id"></div>').parent();
    setupRowHandlers.call($wrapper);
    $('.file-list-single').parent().append($wrapper);
    Drupal.attachBehaviors($wrapper);
  }

  function editRow(file) {
    // find the row
    var row = $('.file-list-single input[value="'+file.fid+'"]').parents('.file-list-single'),
      image = row.find('span img');
    row.find('span').text(" "+file.filename).prepend(image);
  }

  function setupRowHandlers() {
    //$('.edit a', this).click(openEdit);
    $('.remove a', this).click(removeFile);
  }

  Drupal.behaviors.mediaDraggableWidget = {
    attach: function (ctx, settings) {
      if (typeof template == 'undefined') {
        template = $('.file-list-single[hidden]');
      }
      $('.field-widget-media-draggable-file .form-type-dragndrop-upload').once('media-draggable', function () {
        $('.droppable-browse-button', this).unbind().click(browserClickHandler);
      });

      $('.field-widget-media-draggable-file .file-list-single').once('media-draggable', setupRowHandlers);

      if (typeof settings.mediaDraggable != 'undefined' && $(ctx).prop('tagName') == 'FORM') {
        if (typeof settings.mediaDraggable.newFile != 'undefined') {
          addRow(settings.mediaDraggable.newFile);
          delete settings.mediaDraggable.newFile;
        }
        else if (typeof settings.mediaDraggable.editedFile != 'undefined') {
          editRow(settings.mediaDraggable.editedFile);
          delete settings.mediaDraggable.editedFile;
        }
      }
    }
  };
})(jQuery);