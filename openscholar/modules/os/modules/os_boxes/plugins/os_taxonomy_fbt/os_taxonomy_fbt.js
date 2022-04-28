// Adds active class in widget links anchor tag for current active link.
Drupal.behaviors.osTaxonomyFilter = {
  attach : function(ctx) {
    jQuery('.block-boxes-os_taxonomy_fbt a').removeClass('active');
    jQuery('.block-boxes-os_taxonomy_fbt a[href="'+ location.pathname + '"]').addClass('active');
  }
};

// Hides display type radio buttons when widget type is set to "Select"
Drupal.behaviors.osTaxonomyFilterDisplayTypes = {
  attach : function(ctx) {
    jQuery('#edit-widget-type').change(function() {
      if (jQuery(this).val() == 'select') {
        jQuery("div.form-item-as-nav label").css("display", "none");
        jQuery("div#edit-as-nav").css("display", "none");
      } else if (jQuery(this).val() == 'list') {
        jQuery("div.form-item-as-nav label").css("display", "inline");
        jQuery("div#edit-as-nav").css("display", "inline");
      }
    });
  }
};
