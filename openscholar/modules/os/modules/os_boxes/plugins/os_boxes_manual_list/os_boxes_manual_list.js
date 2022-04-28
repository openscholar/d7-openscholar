/**
 * Clear the input of the selected post feature after the node is being added.
 */

(function ($) {

  Drupal.behaviors.os_manual_list = {
    attach : function (ctx) {
      $('#manual-nodes-list').live('DOMSubtreeModified', function(e) {
        $("#edit-node-to-add").val('');
      });
    }
  };

})(jQuery);
