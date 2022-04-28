 WebFontConfig = {
    google: { families: [ 'Merriweather::latin' ] }
  };
  (function() {
    var wf = document.createElement('script');
    wf.src = 'https://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js';
    wf.type = 'text/javascript';
    wf.async = 'true';
    var s = document.getElementsByTagName('script')[0];
    s.parentNode.insertBefore(wf, s);
  })();
  

jQuery(window).scroll(function() {    
    var scroll = jQuery(window).scrollTop();

    if(scroll >= 1000) {
        jQuery(".top-arrow").addClass("show");
    } else {
        jQuery(".top-arrow").removeClass("show");
    }
});


jQuery(document).ready(function(){

    var highestBox = 0;
        jQuery('.region-content-first .block-boxes-os_boxes_columns .region .block').each(function(){  
                if(jQuery(this).height() > highestBox){   
                highestBox = jQuery(this).height();  
        }
    });    
    jQuery('.region-content-first .block-boxes-os_boxes_columns .region .block').height(highestBox);

jQuery(".get-site").click(function() {
   jQuery(".getsite_content").slideToggle('fast');
    });

});
