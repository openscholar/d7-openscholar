/**
 * Modifies the content area on the Dataverse iframe page.
 * 
 * Adds a loading message, and removes it when the iframe loads.
 */
(function($){
  Drupal.behaviors.os_dataverse = {
    attach: function(context){
      // Adds "Loading Dataverse" message with throbber 
      $('#os-dataverse-data').prepend('<h2 id="os-dataverse-loading" class="title ajax-progress">Loading Dataverse<span class="throbber">&nbsp</span></h2>');
      // Removes the "Loading Dataverse" message when iframe loads
      $('#os-dataverse-data iframe').load(function () {
        $('#os-dataverse-data #os-dataverse-loading').remove();
      });
    }
  };
})(jQuery);