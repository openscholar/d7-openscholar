/**
 * jQuery to toggle form elements according to content type
 */

(function($) {
  Drupal.behaviors.os_sv_list = {
    attach : function(context) {

        function showField(sort_type, show_type) {
          var expire_event_appear = jQuery('.form-item-event-expire-appear');
          expire_event_appear.hide();
          if (sort_type == 'sort_event_asc' && show_type == 'upcoming_events') {
            expire_event_appear.show();
            expire_event_appear.find('label').text(Drupal.t('Events should expire'));
          }
          else if (sort_type == 'sort_event_desc' && show_type == 'past_events') {
            expire_event_appear.show();
            expire_event_appear.find('label').text(Drupal.t('Events should appear'));
          }
          else {
            expire_event_appear.hide();
          }
        }

      $('#os_sv_list_content_type').once('once', function() {
        // when content type changes, update all the options
        $('#os_sv_list_content_type').change(function() {
          var $sortby = $('#edit-sort-by');
          var $display_style = $('#edit-display');
          var $vocabs = $('#edit-vocabs');
  
          var content_type = $('#os_sv_list_content_type').val();
          var more_link = $('#more_link_div input[name="more_link"]');
          var defaults = Drupal.settings.more_link_defaults;
  
          
          //apply content_type appropriate sorts when ct changes
          $sortby.children('option').each(function() {
            var sorts = Drupal.settings.sv_list_sort;
            
            var this_sort = $(this).attr('value');
            var hide = ((sorts[this_sort] !== undefined) && ($.inArray(content_type, sorts[this_sort]['bundle']) == -1));            
            $(this).attr('hidden', hide).attr('disabled', hide);
          });
          
          //uncheck if selected option is no longer valid.
          $sortby.children('option:checked').filter(':disabled').attr('selected', false);
  
          //if the content type has hidden the grid layout, switch back to list.
          //this must happen BEFORE display styles are chosen
          if ($('.form-item-layout').css('display') == 'none') {
            jQuery('#edit-layout').attr('value', 'List');
          }
          
          //show or hide columns
          if ($(':input:visible[name="layout"][value="Grid"]').length > 0) {
            $('.form-item-grid-columns').show();
          } else {
            $('.form-item-grid-columns').hide();
            $(':input[name="layout"]').attr('selected', false);
          }
                  
          //only show the content appropriate display styles
          $display_style.children('option').each(function() {
            var this_display = $(this).attr('value');
            if ($('#edit-layout').length>0) {
              // User can select a layout.
              // In certain types of data, for example "List of Files" when the
              // file is of type "Image", it is possible to select a layout,
              // for example "list" or "grid". So if the "Layout" field is shown
              // we must consider the layout type when enabling display types.
              var this_layout =  $('#edit-layout').attr('value').toLowerCase();              
              var hide = ($.inArray(this_display, Drupal.settings.entity_view_modes[this_layout][content_type]) == -1)
              $(this).attr('hidden', hide).attr('disabled', hide);
            }
            else {
              // User cannot select a layout, so the default layout is "list".
              var hide = ($.inArray(this_display, Drupal.settings.entity_view_modes['list'][content_type]) == -1)
              $(this).attr('hidden', hide).attr('disabled', hide);
            }
          });
          
          //uncheck if selected option is no longer valid.
          $display_style.children('option:checked').filter(':disabled').attr('selected', false);

          // swap out the more link url.
          more_link.val(defaults[content_type]);
                
          //apply content type to available vocabs
          var hidden = true;
          for (var vid in Drupal.settings.sv_list_vocab_bundles) {
            var $div = $vocabs.find('.form-item-vocabs-vocab-' + vid);
            if ((content_type == 'all') || $.inArray(content_type, Drupal.settings.sv_list_vocab_bundles[vid]) != -1) {
              $div.show();
              hidden = false
            } else {
              $div.hide();
            }
          }

          //show/hide the vocab label if there are any remaining vocabs
          if (hidden) {
            $vocabs.hide();
          } else {
            $vocabs.show();
          }
          
          //layout changes should trigger the display style refresh
          $('#edit-layout').change(function() {
            $('#os_sv_list_content_type').change();
          });

          // Handle the "event" content type.
          if (content_type == 'event') {
            $('.form-item-show').show();
            var sort_by = $('#edit-sort-by');
            var show = $('#edit-show');

            // Show the "expire-event-appear" field if needed based on current
            // form values.
            showField(sort_by.val(), show.val());

            // Show the "expire-event-appear" field when user selects ascending order
            // for upcoming events or descending order for past events.
            sort_by.change(function() {
              var sort_type = $(this).val();
              var show_type = show.val();
              showField(sort_type, show_type);
            });
            show.change(function() {
              var sort_type = sort_by.val();
              var show_type = $(this).val();
              showField(sort_type, show_type);
            });            
          }
          else {
            $('.form-item-event-expire-appear').hide();
            $('.form-item-show').hide();
          }
        });
  
        // perform the change callback once now.
        $('#os_sv_list_content_type').change();
  
        // Get the default value of the content_type.
        var content_type = $('#os_sv_list_content_type').val();
        var show_all_checked = $('#biblio_show_all_check').is(':checked') ? true : false;
  
      });

      // Select2.
      //console.log($('#vocabs', context).find('.form-select:not(.select2-processed)'))
      if (typeof Drupal.settings.sv_list_vocab_bundles !== 'undefined') {
        $("#edit-vocabs", context).addClass('select2-processed').find('.form-select:not(.select2-processed)').select2({
          placeholder: Drupal.t("Click here to select terms"),
          width: '20em'
        });
      }
    }
  };
}(jQuery));