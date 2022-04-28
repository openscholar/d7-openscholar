<?php
/**
 * template file for theming the logo picker
 * Note that this is used just for the img
 * and not the html radios
 * Variables:
 * ----------
 * $file : the whole file object  (dpm(file) to see everything
 *
 */


  $shield = t('no preview available');
  if (file_exists($file->uri)) {
    $shield = theme('image_style', array('path' => $file->uri, 'attributes' => array(), 'style_name' => 'os_thumb_180_140'));
  }
  print '<div class="item-shield-picker">'. $shield .'</div>';
