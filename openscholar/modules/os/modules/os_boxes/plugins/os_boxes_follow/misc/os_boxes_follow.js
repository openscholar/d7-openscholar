/**
 * Allows users to add posts to their manual lists without an additional 
 * page load on top of the ajax call
 */
Drupal.behaviors.os_boxes_follow = {
  attach: function (ctx) {
    var $ = jQuery;
  	if ($('#follow-links-list', ctx).length == 0) return;	// do nothing if our table doesn't exist
  	
  	var $form = $('#boxes-box-form'),
  		template = '<tr class="draggable">'+$('input[name="links[blank][title]"]').parents('tr').hide().html()+'</tr>',
  		tableDrag = Drupal.tableDrag['follow-links-list'],
  		count = $('input[type="hidden"][name="count"]'),
  		new_id = parseInt(count.val());
  	
  	// add a new row to the table, set all its form elements to the right values and make it draggable
  	$('.add_new', $form).click(function (e) {
  		var val = $('#edit-link-to-add', $form).val(),
  			patt = /^https?:\/\/([^\/]+)/,
  			matches = patt.exec(val),
  			new_row, id, i, fd, weight = -Infinity;
  		
  		// there should actually be something in the field
  		if (matches != null) {
  			var domain = matches[1],
  				domains = Drupal.settings.follow_networks;
  			
  			// get domain
  			for (i in domains) {
  				fd = domains[i];
  				if (domain.indexOf(fd.domain) != -1) {
  					domain = i;
  					break;
  				}
  			}
  			
  			// if we don't have a valid domain, don't make a new row
  			if (domain != matches[1]) {
  				id = new_id++;
  				new_row = $(template.replace(/blank/g, id));
  				count.val(parseInt(count.val())+1);
  				
  				// get the new weight
  				$('.field-weight', $form).each(function () {
  					if ($(this).val() > weight) {
  						weight = parseInt($(this).val());
  					}
  				});
  				// there are no existing form elements, start at 0.
  				if (weight == -Infinity) {
  					weight = 0;
  				}
  				// set all the form elements in the new row
  				$('span', new_row).addClass('follow-icon '+domain).text(val);
  				$('input[name="links['+id+'][title]"]', new_row).val(val);
  				$('input[name="links['+id+'][domain]"]', new_row).val(domain);
  				$('#edit-links-'+id+'-weight', new_row).addClass('field-weight').val(weight+1);
  				$('#edit-links-'+id+'-weight', new_row).parents('td').css('display', 'none');
  				$('.follow-icon', new_row).css('background-position', '-'+fd.offset+'px 0px');
  				//$('.tabledrag-handle', new_row).remove();
  				$('table tbody', $form).append(new_row);
  				new_row = $('input[name="links['+id+'][title]"]', $form).parents('tr');
  				
  				setup_remove(new_row);
  
  				tableDrag.makeDraggable(new_row[0]);
  				
  				// refreshes the variable
  				$form = $('#boxes-box-form');
  			}
  			else {
  				// alert the user that the domain was not invalid.
  				// bein' lazy for now
  				alert(val+' is not from a valid social media domain.');
  			}
  		}
  		else {
  			alert(val+' is not from a valid social media domain.');
  		}
  		$('#edit-link-to-add', $form).val('');
  	});
  	
  	// set up remove links.
  	function setup_remove(ctx) {
  		$('.remove', ctx).click(function () {
  			var $this = $(this);
  			$this.parents('tr').remove();
  			
  			// decrement counter
  			count.val(parseInt(count.val())-1);
  			
  			return false;
  		});
  	}
  	
  	setup_remove($form);
  }
};