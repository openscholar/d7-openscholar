/**
 * @file os_sv_list_display.js
 * 
 * Ajax for sv_list pager
 */
(function ($) {
  var data = {};
  Drupal.behaviors.os_sv_list = {
    attach: function (ctx) {

      // Sets embedded iframe width same as the parent wrapper div width if initial iframe width is greater than parent div width
      $('.block-boxes-os_sv_list_file .os_sv_list_file .file').each(function(i){
        var width_parent = $(this).parent().width();
        if (width_parent < $(this).find('iframe').attr('width')) {
          $(this).find('iframe').attr('width', width_parent);
        }
      });

      // add a click handler to lists of posts
      $('.os-sv-list', ctx).closest('.boxes-box-content').once('os-sv-list-pager').click(click_handler).each(function () {
        // save the current page to our cache
        // get the delta of the box and the page, and store them that way
        var page_elem = $(this).find('[data-page]'),
          page = page_elem.attr('data-page'),
          delta = page_elem.attr('data-delta');
        
        data[delta] = data[delta] || {};
        data[delta][page] = page_elem.parent().html();
      });
      
      function click_handler(e) {
        // do nothing if this isn't a pager link
        if ($(e.target).parents('.pager').length == 0 || e.target.nodeName != 'A') return;
        e.stopPropagation();
        e.preventDefault();
        
        // get the requested page and delta from the query args
        var args = query_args(e.target.href),
            delta = args.sv_list_box_delta;

        var block_boxes_selector = "#context-block-boxes-" + delta;
        if (! $(block_boxes_selector).length) {
          block_boxes_selector = "#block-boxes-" + delta;
        }

        // After clicking on next/prev page ajax link, page will be slowly scrolled to the position where respective 'List of Post' widget markup starts.
        $("html, body").animate({ scrollTop: $("#block-boxes-" + delta.replace(/\_/g, '-')).offset().top }, "slow");

        // if there's no page set in the query, assume its the first page
        if (typeof args.page == 'undefined') {
          args.page = 0;
        }
        
        // get data from the cache
        if (typeof data[delta][args.page] != 'undefined') {
          var parent = $(e.target).closest('.boxes-box-content').html(data[delta][args.page]);
          Drupal.attachBehaviors(parent);
        }
        // if it doesn't exist, we have to ask the server for it
        else {
          var s = Drupal.settings, 
            page = decodeURIComponent(args.page).split(',');
            page = page[args.pager_id];
            destination = args.destination;
            $.ajax({
              url: window.location.protocol + '//' + window.location.hostname + '/' + (typeof s.pathPrefix != 'undefined'?s.pathPrefix:'') + 'os_sv_list/page/'+delta,
            data: {
              page: page,
              destination: destination
            },
            beforeSend: function (xhr, settings) {
              $(e.currentTarget).append('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>')
            },
            success: function (commands, status, xhr) {
              var html, i,
              parent = $(e.currentTarget);
              
              for (i in commands) {
                if (commands[i].command == 'insert' 
                  && commands[i].method == null 
                  && commands[i].selector == null 
                  && commands[i].data != "") {
                    html = commands[i].data;
                    break;
                }
              }
              // replace the existing page with the new one
              parent.html(html);
              Drupal.attachBehaviors(parent);
              // and add it to our cache so we won't have to request it again
              var page = parseInt(parent.find('[data-page]').attr('data-page'));
              data[delta][page] = html;
              $(e.currentTarget).find('.ajax-progress').remove();
            }
          });
        }
      }
    }
  };
  
  // splits the url into an object of its query arguments
  function query_args(url) {
    var frags = url.split('?'),
      args = {};
    frags = frags[1].split('&');
    for (var i=0; i<frags.length; i++) {
      var arg = frags[i].split('=');
      args[arg[0]] = arg[1];
    }
    
    return args;
  }
  
  function get_delta(elem) {
    return elem.id.replace('block-boxes-', '');
  }
})(jQuery);
