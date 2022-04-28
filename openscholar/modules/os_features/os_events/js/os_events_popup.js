(function($) {

  Drupal.behaviors.triggerEventPopup = {
    attach: function (ctx) {
      var dialogs = $('div[id^=event-popover]');

      // Initialize and hide all dialogs.
      dialogs.dialog();
      dialogs.dialog('close');

      $('.view-item-os_events .contents a').on('click', function(e) {
        e.preventDefault();
        var itemId = $(this).closest('div[data-item-id]').data('item-id');
        var delta = $(this).attr("href").match(/delta=(\d+)*/);
        var popOver = $('#event-popover-' + itemId + "-" + delta[1]);
        var isOpen = popOver.dialog("isOpen");

        // Toggle dialog.
        if (isOpen) {
          popOver.dialog('close');
        }
        else {
          popOver.dialog({
            position: {my: 'bottom-10', at: 'center', of: e},
            draggable: false,
            resizable: false,
            closeOnEscape: true
          });
        }
      });
    }
  };
})(jQuery);
