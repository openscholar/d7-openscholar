(function ($) {
  Drupal.behaviors.osRevisionsConfirms = {
    attach: function () {
      /**
       * Un-collapse the revision fieldset when node already has
       * the max number for that content type
       */
      if ($("input[name='max_revisions']").val()) {
        $max = $("input[name='max_revisions']").val();
        $current = $("input[name='revisions']").val();
        if ($current - $max >= 0) {
          $('fieldset#edit-revision-information').removeClass('collapsed');
        }
      }
      /**
       * Display warning message when user clicks link to view revisions before any changes
       * that have been made have been saved
       */
      $editform = 0;
      $('form.node-form input,form.node-form select,form.node-form textarea').change(function() {
        $editform = 1;
      });
      if($('form.node-form').length) {
        CKEDITOR.on('instanceReady', function(readyEvent) {
          readyEvent.editor.on('blur', function(blurEvent) {
            if(blurEvent.editor.checkDirty()) {
              $editform = 1;
            }
          });
        });
      }
      $('fieldset#edit-revision-information a#revisions-link').click(function(clickEvent) {
        if($editform) {
          if (!confirm('Leaving this page will leave your changes unsaved.')) {
            clickEvent.preventDefault();
            return false;
          }
        }
      });
    }
  }
})(jQuery);
