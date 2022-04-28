/**
 * Starts up accordion UI for faceted taxonomy vocabulary items
 */
(function ($) {
  Drupal.behaviors.osBoxesFacetedTaxonomyAccordion = {
    attach: function (ctx) {
      $.each(Drupal.settings.os_boxes.faceted_taxonomy, function (delta, data) {
        $('#boxes-box-' + data.delta + ' > .boxes-box-content > .accordion > .item-list').accordion({
          collapsible: true,
          heightStyle: 'content',
          active: true
        });
      });
      $('.block-boxes-os_boxes_facetapi_vocabulary a.facetapi-active').closest('div.item-list').find('h3').not('.ui-accordion-header-active').trigger('click');
    }
  }
})(jQuery);