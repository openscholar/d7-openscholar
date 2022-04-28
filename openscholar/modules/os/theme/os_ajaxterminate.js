/**
 * 
 */
Drupal.behaviors.os_ajaxFix = {
    attach: function (ctx) {
      //http://drupal.org/node/1232416   hide the ajax error that comes up when an ajax call is left dangling
      var $ = jQuery;
      
      // Everything but that stupid alert
      Drupal.ajax.prototype.error = function(response, uri) {
        // Remove the progress element.
        if (this.progress.element) {
          $(this.progress.element).remove();
        }
        if (this.progress.object) {
          this.progress.object.stopMonitoring();
        }
        // Undo hide.
        $(this.wrapper).show();
        // Re-enable the element.
        $(this.element).removeClass('progress-disabled').removeAttr('disabled');
        // Reattach behaviors, if they were detached in beforeSerialize().
        if (this.form) {
          var settings = response.settings || this.settings || Drupal.settings;
          Drupal.attachBehaviors(this.form, settings);
        }
      }
     /* jQuery.ajaxSetup({
        beforeSend: function(jqXHR, settings) {
          settings.error = function(jqXHR, textStatus, errorThrown) {
            if (console) {
              console.error('Ajax Error: ' + textStatus);
              console.log(errorThrown);
              console.log(jqXHR);
            }
          };
        }
      });*/
    }
};