/**
 * Behaviors and scripts for the Columns widget
 *
 * There are 3 structures to this widget:
 * 1. The layout selection
 * 2. The unused widgets
 * 3. The regions themselves
 *
 * 1. Layout Selection
 * -------------------
 * The region arrangement is determined by a 2 step process. First the user selects the number of columns they want.
 * This is merely browsing, it has no effect on the final the widget arrangement. When they select the number of
 * columns, the list of layouts filters by the number of columns each layout has.
 * Now the user can click a layout, and the region arrangement will update. Any widgets in regions that are no longer
 * in the layout will be returned to 'Unused widgets'.
 *
 * 2. Unused Widgets
 * -----------------
 * A list of draggable widgets, with a search bar. The search bar will automatically filter the Unused Widgets list by
 * the title of the widget, as entered by the user.
 *
 * 3. Regions
 * -----------------
 * A group of regions that widgets can be dragged into. Which regions are present and when is determined by a class on
 * on the parent element. When a region is removed due to layout change, it's widgets are dumped back into unused
 * widgets. When the form is saved, all the widgets are translated into a {bid}|{bid} format and sent to the server.
 */

(function ($) {

  var layout = '';
  var textContent;

  /**
   * Changes out the classes
   */
  function changeLayout(new_layout) {
    $('#edit-regions').removeClass(layout).addClass(new_layout);
    layout = new_layout;

    // Remove widgets from no longer visible regions. Place them back in unused widgets
    var hidden = $('.region:hidden .cp-layout-widget');
    if (hidden.length) {
      hidden.each(function () {
        $(this).remove().appendTo('#edit-widgets');
      })
      $('#edit-widgets, .region:hidden').sortable('refresh');
    }
  }

  /**
   * Filter widgets by the text given in the Search field
   */
  function filterWidgets(e) {
    var str = e.currentTarget.value.toLowerCase();

    $('#unused-widgets .cp-layout-widget').each(function () {
      if (typeof this.actual === 'undefined') {
        if (textContent) {
          var first = this.textContent.indexOf('\n'),
              second = this.textContent.indexOf('\n', first+1);

          this.actual = this.textContent.substring(first, second).trim().toLowerCase();
        }
        else {
          this.actual = this.innerText.toLowerCase();
        }
      }

      if (this.actual.indexOf(str) != -1) {
        $(this).show();
      }
      else {
        $(this).hide();
      }
    });
  }

  /**
   * Allow widgets to be dragged around pages
   */
  function setupDraggables() {
    $('#edit-widgets, .region').sortable({
      appendTo: '#boxes-box-form',
      helper: 'clone',
      cursorAt: {top: 25, left: 38},
      connectWith: $('#edit-widgets, .region'),
      tolerance: 'pointer',
      forceHelperSize: true,
      over: function (event, ui) {
        $(event.target).addClass('active');
      },
      out: function (event, ui) {
        $(event.target).removeClass('active');
      }
    });
  }

  /**
   * Save the region widgets to the hidden input elements
   */
  function saveForm() {
    $('.region').each(function () {
      var ids = [],
        rid = $(this).attr('data-region_id');
      $('.cp-layout-widget', this).each(function () {
        ids.push(this.id);
      })
      $('.region_storage[data-region_id="'+rid+'"]').val(ids.join('|'));
    })
  }

  Drupal.behaviors.osBoxesColumns = {
    attach: function (ctx) {
      textContent = typeof document.body.textContent !== 'undefined';

      setupDraggables();
      
      changeLayout(layout = $('input[name="layout"]:checked').val());

      $('input[name="layout"]:radio').change(function(e) {
        changeLayout(e.currentTarget.value);
      });

      $('#edit-search').keyup(filterWidgets);

      $('#boxes-box-form').submit(saveForm);
    }
  };

})(jQuery);