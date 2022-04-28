/**
 * 
 */
(function ($) {
  Drupal.behaviors.overlayCommands = {
    attach: function (ctx) {
      if (ctx != document) return;  // not the initial page load, do nothing
      
      $(document).bind('drupalOverlayLoad', onload);
    }
  };
  
  function onload() {
    var comms = Drupal.overlay.iframeWindow.Drupal.settings.overlay_commands;
    if (typeof comms == 'undefined' || comms.length == 0) return;
    
    // ajax commands require an ajax object. 
    // Create a dummy one that never executes
    var ajax = new Drupal.ajax('do-not-use', 'do-not-use', {url: 'system/ajax', event: 'do-not-use', wrapper: 'do-not-use'}); 
    
    for (var i in comms) {
      // we want to do things to the page, so don't refresh it
      Drupal.overlay.refreshPage = false;
      
      var c = comms[i]['command'];
      if (c == 'modal_dismiss') {
        Drupal.overlay.close();
      }
      if (c && Drupal.ajax.prototype.commands[c]) {
        Drupal.ajax.prototype.commands[c](ajax, comms[i], 200);
      }
    }
  }
})(jQuery);