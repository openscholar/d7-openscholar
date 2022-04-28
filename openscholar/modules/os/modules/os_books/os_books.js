/**
 * Sets up a linkage between the links in a Table of Contents block
 * or the book links in the content area and the actual contents of a book page
 */
(function($){
	var container, header, content = {}, active, orig_nid, blocks = $().not("*");

	Drupal.behaviors.os_book_linkage = {
		attach: function(ctx) {
			if (!$('body.node-type-book').length) return;
			var nid, url;

			if (ctx == document) {
				content = Drupal.settings.os_books.pages;
				orig_nid = active = $('body').attr('class').match(/page-node-(\d+)/)[1];
				container = $('#content');
				header = $('#page-title');

				try {
					history.replaceState({nid: orig_nid}, header.text(), location.href);
				}
				catch (ex) {
					// do nothing
				}
			}

			// Pages is a list of every page in the book.
			// We parse this content and assign the contents to an nid in a js object
			// as we do this, we check for links that have the same title and assign
			// the nid as an attribute.
			for (nid in content) {
				url = content[nid].url;
				// add nid as attribute of links with same path
				$('.block, .book-menu, .book-navigation')
						.find('a[href$="'+url+'"]')
						.not('[data-nid]')
						.each(function (index, elem) {
					elem.setAttribute('data-nid', nid);
					var parents = $(elem).parents('.block, .book-menu, .book-navigation');
					blocks = blocks.add(parents);
				});
			}

			// attach click handler to blocks
			blocks.click(toc_click);
			
			// update the toc
			update_toc(active);

			window.onpopstate = history_change;
		},
		detach: function(ctx) {
			blocks.unbind('click', toc_click);
		}
	};
	function toc_click(e) {
		if (!e.target) {
			e.target = e.srcElement;
		}
		var nid = e.target.getAttribute('data-nid');
		if (content[nid]) {
			e.preventDefault();

			try {
				history.pushState({nid: nid}, content[nid].title, e.target.href);
			}
			catch (ex) {
				// don't break the script in crappy browsers
			}
			page_swap(nid);
		}
	}

	function history_change(e) {
		if (e.state !== null) {
			var nid = e.state.nid;
			page_swap(nid);
		}
	}

	function page_swap(nid) {
		Drupal.detachBehaviors($('.node', container)[0]);
		$('#comments', container).remove();
		// Swap out the breadcrumbs.
		$('div.breadcrumb').html(content[nid].breadcrumb);
		$('.node', container).replaceWith(content[nid].content);
		// the 2nd .node is a different element from the first
		var node = $('.node', container).hide().fadeIn();
		header.html(content[nid].title);
		$('h2[property="dc:title"]').remove();

		// change drupal settings
		Drupal.settings.getQ = "node/"+nid;
		if (typeof Drupal.settings.disqus == 'object')
			Drupal.settings.disqus.identifier = "node/"+nid;	//TODO: Find a better way to do this

		Drupal.attachBehaviors(node[0]);

		// deal with the 'active' class
		$('a[data-nid="'+active+'"], a[data-nid="'+orig_nid+'"]').removeClass('active');
		$('a[data-nid="'+nid+'"]').addClass('active');
		active = nid;

		if ($('.fb-social-comments-plugin').length) {
			fbAsyncInit();
		}
		
		update_toc(nid); 	
	}
    
	function update_toc(nid) {
	  if (Drupal.settings.os_books.settings.toc_type != 'partial') return;
	  var links = $('a[data-nid="'+active+'"]'),
	    blocks = links.parents('.block-boxes-os_boxes_booktoc');
	  blocks.find('.block-content li').hide();
	  links.parents('.block-boxes-os_boxes_booktoc li').show().siblings().show();
	  links.parent().find('a[data-nid="'+active+'"] + ul > li').show();
	}
})(jQuery);
