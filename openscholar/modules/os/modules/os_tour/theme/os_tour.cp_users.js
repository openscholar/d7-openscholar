/**
 * jQuery behaviors for platform notification feeds.
 */
(function ($) {

  Drupal.behaviors.os_tour_cp_users = {

    attach: function (context, settings) {

      // Setup.
      var id = 'os-tour-cp-users';
      var tourLink = '#' + id;
      var $tourLink = $(tourLink);
      if (!$tourLink.length) {
        return;
      }
      if (typeof hopscotch == 'undefined') {
        return;
      }

      $tourLink.attr('href', '#');

      // Adds our tour overlay behavior with desired effects.
      $(tourLink, context).once(id, function() {
        $(this).click(function() {
          //$('.hopscotch-bubble').addClass('animated');
          // Sets up the tour object with the loaded feed item steps.
          var tour = {
            showPrevButton: true,
            scrollTopMargin: 100,
            id: id,
            steps: [
              {
                title: Drupal.t('Welcome!'),
                content: Drupal.t('In this short tour, you\'ll learn all about managing site members.'),
                target: document.querySelector(tourLink),
                placement: "left",
                xOffset: -20,
                yOffset: -17
              },
              {
                title: Drupal.t('The People tab'),
                content: Drupal.t('This tab lists all members on your site, and what roles they are assigned.'),
                target: document.querySelector('.tabs.primary .active'),
                placement: "left",
                xOffset: 10,
                yOffset: -17
              },
              {
                title: Drupal.t('Add a member'),
                content: Drupal.t('Enter an email address to invite a new member to join your site.'),
                target: document.querySelector('.cp-add-new-user li a'),
                placement: "left",
                yOffset: -16
              },
              {
                title: Drupal.t('Managing users'),
                content: Drupal.t('Once they have joined, you can edit user roles, or remove them from your site.'),
                target: document.querySelector('.cp-manager-user-content'),
                placement: "top",
                yOffset: 14,
                xOffset: 685
              },
              {
                title: Drupal.t('The Permissions tab'),
                content: Drupal.t('Need more control? Restrict or allow certain roles to access certain site features.'),
                target: document.querySelector('.tabs.primary .active'),
                placement: "left",
                yOffset: -17,
                xOffset: 115
              },
              {
                title: Drupal.t('The Roles tab'),
                content: Drupal.t('You can even create your own new type of user role. Great for a moderation workflow or complex sites with need for restricted internal access.'),
                target: document.querySelector('.tabs.primary .active'),
                placement: "left",
                yOffset: -17,
                xOffset: 230
              }
            ]
          };
          hopscotch.startTour(tour);
          // Removes animation for each step.
          $('.hopscotch-bubble').removeClass('animated');
          // Allows us to target just this tour in CSS rules.
          $('.hopscotch-bubble').addClass(id);
        });
        $tourLink.slideDown('slow');
      });

    }

  };

})(jQuery);
