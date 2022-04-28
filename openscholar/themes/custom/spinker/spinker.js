/**
 * Load a random image on the front page.
 */

$(document).ready(function () {
	if($('.front').length) {
		var vars = $('param[name="flashvars"]').val().split('&');
		jQuery.each(vars, function(i, item) {
			if (item.indexOf("splash=") != -1) {
				vars[i] = 'splash='+encodeURIComponent('http://www.stevepinker.com/photos/random.mg%3FAlbumID=10707562%26AlbumKey=rQhmi%26Size=Large%26rand=9706');
			}
		});
		var str = vars.join('&');
		$('param[name="flashvars"]').val(str);
		$('embed[src*="www.smugmug.com"]').attr('flashvars', str);
	}
	
	//Replaces Recent Presentations with Video Archive inside block-views-scholar_publications-block_1 - requested 9/2/11
	$('#block-views-scholar_presentations-block_1 h2.block-title').html('Video Archive');
	
});