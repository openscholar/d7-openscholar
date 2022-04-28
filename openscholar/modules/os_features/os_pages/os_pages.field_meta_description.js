/**
 * Enhances node path alias entry with automatic preview on forms.
 */
(function ($) {

// Stores behaviors as a property of Drupal.behaviors.
Drupal.behaviors.os_pages_meta_description = {

attach: function (context, settings) {
  $(document).ready(function () {
    var sourceSel = '#edit-field-meta-description';
    var targetSel = '.field-meta-description.os-javascript-load .fieldset-wrapper';
    $(sourceSel).appendTo(targetSel);
  });
}

}

}(jQuery));