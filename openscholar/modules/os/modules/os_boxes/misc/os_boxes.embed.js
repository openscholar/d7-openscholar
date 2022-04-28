
(function ($) {

  Drupal.behaviors.osBoxesEmbedLinks = {
    attach: function (ctx) {
      $('.embed-popup').tabs().dialog({
        autoOpen: false,
        title: 'Share',
        height: 300,
        width: 500,
        modal: true
      });
      $('.os-embed-link').click(function (e) {
        var id = '#embed-popup-'+($(this).attr('data-delta'));
        $(id).dialog('open');
        e.preventDefault();
      });
      // Displaying widget embed link for widgets rendered within widgets on hover event for widgets, i.e columns, random etc.
      $('.os-embed-link').closest('.block-boxes').each(function(i){
        $(this).hover(function(){
          $(this).find('.os-embed-link').show();
        }, function(){
          $(this).find('.os-embed-link').hide();
        });
      });
    }
  }

})(jQuery);
