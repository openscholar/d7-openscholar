jQuery(document).ready(function() {
	// ADDS A RANDOM CLASS TO THE BODY - APPLY BG IMAGES
	var classes = ["bg-one", "bg-two", "bg-three", "bg-four", "bg-five", "bg-six", "bg-seven", "bg-eight"];

	jQuery("body.not-front").each(function() {
		jQuery(this).addClass(classes[~~(Math.random() * classes.length)]);
	});
});

