WebFontConfig = {
	google : {
		families : ['Oswald:400,300,700:latin', 'Quattrocento']
	}
};
(function() {
	var wf = document.createElement('script');
	wf.src = 'https://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js';
	wf.type = 'text/javascript';
	wf.async = 'true';
	var s = document.getElementsByTagName('script')[0];
	s.parentNode.insertBefore(wf, s);
})();

jQuery(document).ready(function() {
	// ADDS A RANDOM CLASS TO THE BODY - APPLY BG IMAGES
	var classes = ["bg-one", "bg-two", "bg-three", "bg-four", "bg-five", "bg-six", "bg-seven", "bg-eight"];

	jQuery("body.not-front").each(function() {
		jQuery(this).addClass(classes[~~(Math.random() * classes.length)]);
	});
	// TOGGLES THE SEARCH BLOCK
	jQuery("#block-os-primary-menu .nice-menu li.last a").click(function() {
		jQuery(".block-os-search-solr").toggleClass('expose');
	});
	// ADDS A SPAN TAG AFTER THE DESCRIPTION DIV IN THE AREAS OF RESEARCH WIDGET
	jQuery(".research-by-topic ul .item-list ul li .description").after("<span></span>");

	// TOGGLECLASS OPEN ON LIS IN AREAS OF RESEARCH WIDGET
	jQuery('.research-by-topic ul .item-list ul li span').click(function() {
		jQuery(this).parent('.research-by-topic ul .item-list ul li').toggleClass('open');
	});

	// KILL THE TITLE ATTR IN THE ANCHORS IN THE AREAS OF RESEARCH WIDGET
	jQuery(".research-by-topic a").removeAttr("title");

	// ADDS ANNOTATE ICON AND TOGGLE-NESS TO TAXO TERM PAGES
	jQuery('<span class="annotate-icon"></span>').insertBefore('.context-annotation');

	jQuery('.annotate-icon').click(function() {
		jQuery(this).next('.context-annotation').slideToggle(0);
	});

});

(function($) {

	Drupal.behaviors.gkingTheme = {
		attach : function(context) {

			// Only show the title and the date in the presentation teaser.
			$('.front-lop-presentations .node-presentation .node-inner').each(function() {
				var title = $(this).find('.title').clone();
				var contextual = $(this).find('.contextual-links-wrapper').clone();
				var date = $(this).find('.date-display-single').clone();
				$(this).html(' ').append(title).append(contextual).append(date);
			});
		}
	};

})(jQuery);

