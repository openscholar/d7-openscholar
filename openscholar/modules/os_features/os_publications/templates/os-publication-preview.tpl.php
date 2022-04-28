<?php
$icon = theme('image', array('path' => drupal_get_path('module', 'os_publications') . '/misc/application-pdf.png'));
$image = theme('image', array('path' => drupal_get_path('module', 'os_publications') . '/misc/publication-cover.png'));
?>

<a href="#" class="biblio-pop os-publications-image-help" data-popbox="pop2"><span><?php print t('Help'); ?></span></a>
<span id="pop2" class="biblio-stylebox2">
  <div class="biblio-dummy-wrapper">
    <div class="biblio-dummy-image"><?php print $image; ?></div>
    <div class="biblio-dummy-body">
      Wand, Jonathan, Gary King and Olivia Lau. 2011.
      <a>Anchors: Software for Anchoring Vigenttes Data.</a>
      <em>Jornal of Statistical Software</em>
      <span>42, no. 3: 1-25.</span>
    </div>
    <div class="biblio-dummy-links">
      <a>Website</a>
      <a class="biblio-abstract-dummy-link">Abstract</a>
      <span>
        <span>
          <?php print $icon; ?>
          <a>Article</a>
        </span>
      </span>
    </div>
  </div>
</span>
