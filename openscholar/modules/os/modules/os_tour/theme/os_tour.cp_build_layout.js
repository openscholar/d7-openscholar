/**
 * jQuery behaviors for platform notification feeds.
 */
(function ($) {

Drupal.behaviors.os_notifications = {

attach: function (context, settings) {

  // Setup.
  var tourLink = '#os-tour-cp-build-layout';
  var $tourLink = $(tourLink);
  if (!$tourLink.length) {
    return;
  }
  if (typeof hopscotch == 'undefined') {
    return;
  }

  $tourLink.attr('href', '#');

  // Adds our tour overlay behavior with desired effects.
  $(tourLink, context).once('osTourCpBuildLayout', function() {
    $(this).click(function() {
      //$('.hopscotch-bubble').addClass('animated');
      // Sets up the tour object with the loaded feed item steps.
      var tour = {
        showPrevButton: true,
        scrollTopMargin: 100,
        id: "os-tour-cp-build-layout",
        steps: [
          {
            title: 'Welcome!',
            content: 'In this short tour, you\'ll learn all about layout and widgets.',
            target: document.querySelector('#os-tour-cp-build-layout'),
            placement: "left"
          },
          {
            title: 'Site sections',
            content: 'Click here to see all the App sections enabled on your site.',
            target: document.querySelector("#edit-context-selection"),
            placement: "right",
            yOffset: -20,
            onShow: function () {$('#edit-context-selection').focus();}
          },
          {
            title: 'Drag \'n\' drop!',
            content: 'This diagram shows you the current layout for the selected section. You can drag widgets around to your heart\'s content!',
            target: document.querySelector("#edit-layout"),
            placement: "top"
          },
          {
            title: 'Unused widgets',
            content: 'Scroll through the unused widget gallery to see what other existing widgets you might want to drag into your layout.',
            target: document.querySelector("#edit-layout-unused-widgets"),
            placement: "bottom",
            arrowOffset: 190,
            xOffset: 526,
            onShow: function() { $('#edit-layout-unused-widgets').stop().animate({scrollLeft: $('#edit-layout-unused-widgets').width()}, 1000); },
            onNext: function() { $('#edit-layout-unused-widgets').stop().animate({scrollLeft: 0}, 1000); }
          },
          {
            title: 'Widget categories',
            content: 'Select just a single category tab for faster searching.',
            target: document.querySelector("#widget-categories"),
            placement: "bottom"
          },
          {
            title: 'Custom widgets',
            content: 'Build your own widget from these templates to add them to your library.',
            target: document.querySelector("#ctools-dropdown-1"),
            placement: "top",
            onShow: function () { $('.ctools-dropdown-container').show(); }
          },
          {
            title: 'Headers, footers, sidebars',
            content: 'Place widgets anywhere. I mean ANYWHERE.',
            target: document.querySelector("#edit-layout-content"),
            placement: "bottom",
            arrowOffset: "center",
            xOffset: 53
          },
          {
            title: 'Save to go live',
            content: 'Nothing will be changed on this section until you save.',
            target: document.querySelector("#edit-submit"),
            placement: "top",
            onNext: function() { $('body, html').stop().animate({scrollTop: 0}, 1000); }
          },
          {
            title: 'That\'s it!',
            content: 'Try it for yourself! You may want to save a few times to get the hang of things. As always, use the <strong>Support</strong> link to share any questions or feedback. Thanks!',
            target: document.querySelector('#os-tour-cp-build-layout'),
            placement: "left"
          }
        ]
      };
      hopscotch.startTour(tour);
      // Removes animation for each step.
      //$('.hopscotch-bubble').removeClass('animated');
      // Allows us to target just this tour in CSS rules.
      //$('.hopscotch-bubble').addClass('os-tour-cp-build-layout');
    });
    $tourLink.slideDown('slow');
  });

}

};

})(jQuery);
