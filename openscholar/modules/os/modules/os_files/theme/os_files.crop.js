/**
 @file

 Use the Media Browser to select an image to crop
*/
(function ($) {

Drupal.behaviors.osFilesImageCropBrowser = {
  attach: function (ctx) {
    $('.media-button', ctx).once('os-files-crop').each(function () {
      var element_settings = {},
        id = $(this).attr('id'),
        target = $(this).parent().find('[name$="[fid]"]');

      target.attr('id', id.replace('selected-file', 'fid'));

      element_settings.url = $(this).attr('data-ajax_url');
      element_settings.event = 'change';
      element_settings.wrapper = $(this).parents('.form-type-managed-file').parent().attr('id');
      // Clicked form buttons look better with the throbber than the progress bar.
      element_settings.progress = { 'type': 'throbber' };
      element_settings.submit = {
        js: true,
        // force the validator to only look at the file element
        _triggering_element_name: target.attr('name').replace('[fid]', '')
      };

      var root = $(this).parents('.form-wrapper');

      var base = target.attr('id');
      Drupal.ajax[base] = new Drupal.ajax(base, target[0], element_settings);

    }).click(mediaButtonClick);
  }
}

  function mediaButtonClick(e) {
    var settings = {
      types: ['image'],
      browser: {
        panes: {
          upload: true,
          library: true
        }
      },
      id: 'os_files_imagefield_crop'
    },
      self = this;

    Drupal.media.popups.mediaBrowser(function (selected) {
      // construct the html fragment and put it in place
      if (selected.length == 0) {
        // remove cropping html
      }
      else {
        var file = selected[0],
          id = $(self).attr('id');

        $('#'+id.replace('selected-file', 'fid')).val(file.fid).change();
      }
    }, settings);
    return false;
  }

})(jQuery);