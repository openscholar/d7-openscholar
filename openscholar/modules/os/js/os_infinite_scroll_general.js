/**
 * General handlers for all autopagers.
 */
(function ($) {

  var loadingAll = false,
    old_load,
    behaviorRun = false;

  function load (current, next) {
    if (old_load) {
      old_load.call(this, current, next);
    }

    if (next == undefined) {
      // next is undefined when clicking on "Loading previous". Return early.
      return;
    }

    if (!next.page || next.page == NaN) {
      loadingAll = false;
    }
    if (loadingAll) {
      setTimeout($.autopager.load, 1);
    }

    // Remove the "Load all button" that's already next to a "Load more" button.
    $('#autopager-load-more + .autopager-load-all').remove();
  }

  function loadAllClickHandler(e) {
    e.preventDefault();
    loadingAll = true;
    $.autopager.load(e);

    $('.autopager-load-all').remove();
  }

  Drupal.behaviors.osInfiniteScrollGeneral = {
    attach: function (ctx) {
      if (behaviorRun) return;
      behaviorRun = true;

      if (load != $.autopager.option('load')) {
        old_load = $.autopager.option('load');
        $.autopager.option('load', load);
      }

      $(window).scroll(function(e) {
        // Add a "Load more" button if we don't have one already and we are not
        // in "loading all" state (the "Load all" has been clicked.
        if (!$('#autopager-load-more + .autopager-load-all').length && !loadingAll) {
          $('<div class="autopager-load-all"><a>Load All</a></div>').insertAfter('#autopager-load-more');
          $('div.autopager-load-all', ctx).live('click', loadAllClickHandler);
        }
      });
    }
  }

})(jQuery);
