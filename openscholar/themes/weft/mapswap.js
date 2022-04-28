jQuery(document).ready(function() {
    jQuery('<div id="bates-front-menu"><div id="bg"><ul id="map"><li class="publications"><a href="'+Drupal.settings.basePath+'rbates/publications">Vita with Publications</a></li><li class="bio"><a href="'+Drupal.settings.basePath+'rbates/biocv">Bio</a></li><li class="data"><a href="'+Drupal.settings.basePath+'rbates/data">Data</a></li><li class="classes"><a href="'+Drupal.settings.basePath+'rbates/classes">Classes</a></li><li class="contact"><a href="'+Drupal.settings.basePath+'rbates/contact_owner">Contact</a></li></ul></div></div>').appendTo('.front .os-rbates-map');
    jQuery('#map li').hover(function(){
      var thisclass= jQuery(this).attr("class");
      jQuery(this).parent().attr("class","");
      jQuery(this).parent().addClass("map-" +  thisclass);
      },
      function () {
        jQuery(this).parent().removeClass();
      });
});
