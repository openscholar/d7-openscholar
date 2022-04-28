/**
 * jQuery behaviors for platform notification feeds.
 */
(function ($) {

  Drupal.behaviors.os_tour_node_add_page = {

    attach: function (context, settings) {

      // Setup.
      var id = 'os-tour-node-add-page';
      var tourLink = '#' + id;
      var $tourLink = $(tourLink);
      if (!$tourLink.length) {
        return;
      }
      if (typeof hopscotch == 'undefined') {
        return;
      }

      $tourLink.attr('href', '#');

      // Allows our click inside an iframe to trigger a body click.
      $("#edit-body-und-0-value_ifr", context).once(id, function() {
        $("#edit-body-und-0-value_ifr").contents().find('body').click(function() {
          $('#edit-body').click();
          console.log('Cross click!');
        });
      });
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
                content: Drupal.t('In this short tour, you\'ll learn all about authoring great web content.'),
                target: document.querySelector(tourLink),
                placement: "bottom",
                xOffset: 0,
                yOffset: 0
              },
              {
                title: Drupal.t('Pages are posts'),
                content: Drupal.t('If your site has other apps enabled, you can also create things like events, person profiles and much more.'),
                target: document.querySelector(tourLink),
                placement: "bottom",
                xOffset: 0,
                yOffset: -50
              },
              {
                title: Drupal.t('Titles'),
                content: Drupal.t('Short titles are good. You may insert any special characters you wish!'),
                target: document.querySelector('#edit-title'),
                placement: "bottom",
                xOffset: 0,
                yOffset: 0,
                onNext: _os_tour_example_title
              },
              {
                title: Drupal.t('Page alias (URL)'),
                content: Drupal.t('When you enter a title on a new page, a pretty URL alias is generated on the fly.'),
                target: document.querySelector('#edit-title'),
                placement: "bottom",
                xOffset: 0,
                yOffset: 30
              },
              {
                title: Drupal.t('Rich text editor'),
                content: Drupal.t('Click to expand rich text editor with dozens of useful tools.'),
                target: document.querySelector('#edit-body'),
                placement: "top",
                xOffset: 0,
                yOffset: 0
              },
            /**
             *  @FIXME get WYSIWYG to stay open so we can tour inside it.
             {
               title: Drupal.t('Embed (almost) anything'),
               content: Drupal.t('Insert and resize images, videos, maps and much much more.'),
               target: document.querySelector('#edit-body'),
               placement: "bottom",
               xOffset: 500,
               yOffset: -400,
               onShow: _os_tour_focus_body
             },
             {
               title: Drupal.t('Even more tools'),
               content: Drupal.t('Click this icon to expand the second row of toolbar icons.'),
               target: document.querySelector('#edit-body'),
               placement: "bottom",
               xOffset: 550,
               yOffset: -400
             },
             {
               title: Drupal.t('For best results, format'),
               content: Drupal.t('For higher ranking on the web, use plenty of short, descriptive headings to make your paragraphs stand out and easy to skim.'),
               target: document.querySelector('#edit-body'),
               placement: "bottom",
               xOffset: 0,
               yOffset: -400
             },
             */
              {
                title: Drupal.t('Attach files'),
                content: Drupal.t('Files uploaded here will list like email attachments at the bottom of the post. Perfect for spreadsheets, PDFs, or anything else that can\'t be previewed.'),
                target: document.querySelector('#edit-field-upload'),
                placement: "bottom",
                xOffset: 0,
                yOffset: -15,
                onNext: _os_tour_expand_options
              },
              {
                title: Drupal.t('Saving as draft'),
                content: Drupal.t('Posts will publish by default. Uncheck "Published" to save as a draft.'),
                target: document.querySelector('#edit-options'),
                placement: "bottom",
                xOffset: 0,
                yOffset: 24
              },
              {
                title: Drupal.t('Sticky: for important posts'),
                content: Drupal.t('Check "Sticky" to make this post come first in line at the top of lists. Best if used sparingly!'),
                target: document.querySelector('#edit-options'),
                placement: "bottom",
                xOffset: 0,
                yOffset: -5
              },
              {
                title: Drupal.t('Happy posting!'),
                content: Drupal.t('Thanks for taking the tour.'),
                target: document.querySelector('#edit-title'),
                placement: "bottom",
                xOffset: 0,
                yOffset: 0,
                onShow: _os_tour_focus_title
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

      function _os_tour_example_title() {
        $('#edit-title').val('Example title').change();
      }
      function _os_tour_expand_options() {
        $('#edit-options .fieldset-title').click();
      }
      function _os_tour_focus_body() {
        $("#edit-body-und-0-value_ifr").contents().find('body').click();
        console.log('Clicked on the iframe body tag');
      }
      function _os_tour_focus_title() {
        $('#edit-title').focus().select();
      }
    }

  };

})(jQuery);
