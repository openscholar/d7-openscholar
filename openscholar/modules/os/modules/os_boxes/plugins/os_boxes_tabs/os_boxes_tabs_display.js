/**
 * Saves the current active tab for the session.
 * Restores this tab as the active tab when we return.
 */

Drupal.behaviors.os_boxes_tabs = { attach: function (ctx) {
  var $ = jQuery;

  // without this, the contextual links for a node are treated as tabs
  // thus, it loads crap like the edit form
  // which loads the cp theme and causes css weirdness
  $.widget('os_boxes.tabsBox', $.ui.tabs, {
    _getList: function() {
      var list = this.element.find('.tab-links');
      return list.eq(0);
    }
  });

  $('.os-boxes-tabbed', ctx).once('tabs', function () {
    // filter out children .block-content elements so we don't make them into tabs too
    // if they are a tab widget (nesting tabs, ugh), then we'll handle them in a separate loop iteration
    var id = $(this).closest('.boxes-box').attr('id');
    $(this).not('.os-boxes-tabbed .os-boxes-tabbed').tabsBox({
      show: clickHandle,
      selected: (typeof window.sessionStorage != 'undefined' && typeof sessionStorage[id] != 'undefined')?sessionStorage[id]:0
    });
  });

  // When there is embeded media inside tabs widget, iframe src attribute needs to be reloaded for corresponding tab click event to render scrolbars.
  $('.ui-tabs-anchor').on("click", function( event, ui ) {
    var container_id = $(this).parent().attr('aria-controls');

    // Trumba calendars have empty HTML codes in its iframe src on page load, so their content cannot be refreshed after clicking on tab.
    var refreshElements = $('#' + container_id + ' iframe:not([id^="trumba.spud"])');
    refreshElements.each(function(index) {
      var srcValue = $(this).attr("src");
      // Do not refresh the src attribute for Google Calendar embeds (G Cal)
      if (/www.google.com\/calendar/.exec(srcValue) !== null) {
        return; //this is equivalent of 'continue' for jQuery loop
      }
      $(this).attr("src", srcValue);
    });
  });

  function clickHandle(e) {
    var $this = $(this),
      id = $this.closest('.boxes-box').attr('id'),
      val = $(this).tabs('option', 'selected');

    if (typeof window.sessionStorage != 'undefined')
      window.sessionStorage[id] = val;
  }
}};