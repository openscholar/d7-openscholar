/**
 * Adds functionality to the "Select All" checkbox.
 */
(function ($) {
  Drupal.behaviors.selectAllStart = {
    attach: function (ctx) {

      var select_all_cbx = $('input[name$="[all]"]');
      var select_all_cbx_html = select_all_cbx.get(0);
      var stop = select_all_cbx_html.name.lastIndexOf('[');
      var name = select_all_cbx_html.name.slice(0,stop);
      var $cbxs = $('input[name^="'+name+'"]');

      function selectAll() {
        var chk = select_all_cbx_html.checked;

        $cbxs.each(function() {
          this.checked = chk;
        });
      }

      // Check/uncheck all checkboxes when the "Select All" is toggled.
      select_all_cbx.change(selectAll);

      // When unchecking an option that is not the "all" option, uncheck the
      // "all" checkbox.
      $cbxs.on('change', function(e) {
        if (!e.target.checked && e.target.value != 'all') {
          select_all_cbx.attr('checked', false);
        }
      });
    }
  };
})(jQuery);
