/**
 * Handles toggling of the visibility parameter
 */
(function ($) {
Drupal.behaviors.osPagesOutline = {
  attach: function (ctx) {
    $('.form-checkbox').change(toggleChild);
  }
}

function toggleChildren(mlid, value) {
  $('.book-plid[value='+mlid+']').each(function () {
    var row = $(this).parents('tr'),
        box = row.find('.form-checkbox').get(0);
    box.checked = value;
    box.readOnly = value;
    toggleChildren(row.find('.book-mlid').val());
  });
}

  function toggleChild(e) {
    if (e.target.readOnly) {
      e.target.checked = true;
      e.preventDefault();
      return false;
    }

    var box = e.target,
      mlid = $(box).parents('tr').find('.book-mlid').val();

    toggleChildren(mlid, box.checked);
  }
})(jQuery)