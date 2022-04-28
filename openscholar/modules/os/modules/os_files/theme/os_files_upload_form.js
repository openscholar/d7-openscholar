/**
 * Adds a new button for uploading and submits the form automatically
 */
Drupal.behaviors.os_upload_form = {
  attach: function (ctx) {
    var $ = jQuery,
        $input = $('<label for="edit-upload-upload" class="file-select form-submit">Upload</label>'),
        $help = $('<div class="form-help"></div>'),
        $file_select = $('#edit-upload-upload', ctx);

    if ($('label[for="edit-upload-upload"]').length == 0) {
      $file_select.before($input)
    }

    $('label[for="edit-upload-upload"]', ctx).after($help);

    function changeHandler (e) {
      if (!('result' in e) || e.result) {

        // validation
        var passes = true,
          errors = [],
          files = e.target.files,
          file;

        if (files) {
          // if files is null or undefined, that means this browser doesn't support the File and FileList classes
          // we won't be able to test the file until it actually gets sent to the server
          // this is fine, though. As long as those browsers can still upload the file, we're good
          file = files[0];

          // file size
          if (file.size > Drupal.settings.osFiles.maxFilesize) {
            passes = false;
            errors.push(Drupal.t('This file exceeds the maximum filesize of @filesize', {'@filesize': Drupal.settings.osFiles.rawMaxFilesize}));
          }

          // file type
          var accepted = e.target.accept.split(','),
            ext = file.name.slice(file.name.lastIndexOf('.')),
            typeCheck = false;

          for (var i = 0; i<accepted.length; i++) {
            if (accepted[i] == file.type || accepted[i] == ext) {
              typeCheck = true;
              break;
            }
          }
          passes = passes && typeCheck;
          if (!typeCheck) {
            errors.push(Drupal.t('This file is not one of the accepted file types.'));
          }
        }

        if (passes) {
          $('#file-entity-add-upload .form-actions #edit-next', ctx).click();
          $(e.target).parent().find('.messages').remove();
        }
        else {
          // print errors
          var messages = $(e.target).parent().find('.message .error');
          if (messages.length == 0) {
            $(e.target).parent().prepend('<div class="messages error">'+errors.join('<br />')+'</div>')
          }
          else {
            $(e.target).parent().find('.messages').append('<br />'.errors.join('<br />'));
          }
        }
      }
    }

    $file_select.change(changeHandler);
  }
};
