(function ($) {

  Drupal.behaviors.vsiteVocabHideAddTerm = {
    attach: function (context) {
      // Hiding the "Add term" link when reset the vocabulary to alphabetical
      // order.
      $(".confirmation#taxonomy-overview-terms").parents("#page").find(".action-links").hide();
    }
  };

})(jQuery);
