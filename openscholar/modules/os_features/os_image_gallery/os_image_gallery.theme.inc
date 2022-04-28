<?php

function theme_os_image_gallery_media_gallery_item(&$variables) {
  $image = $variables['image'];
  $link_path = $variables['link_path'];
  $attributes = array();
  if (!empty($variables['classes'])) {
    $attributes['class'] = $variables['classes'];
  }
  if (!empty($variables['title'])) {
    // I'm fairly sure I don't like this solution.  But as Alex mentions in
    // theme_media_gallery_file_field_inline() the File Styles module isn't allowing
    // us access to the render array pre-rendering, so I'm doing a str_replace()
    // here specifically to address the title and alt for thumbnails.  This had
    // to be further modified to remove and then add the title and alt attributes
    // video thumbnails had no title and alt attributes so the string replace was
    // not triggering for them.
    $new_image = str_replace(array('title=""', 'alt=""'), array('', ''), $image);
    $image = str_replace('/>', ' title="' . $variables['title'] . '" alt="' . $variables['title'] . '" />', $new_image);;
  }

  // Add sliding door top div and wrappers
  $item = '<div class="media-gallery-item" '.($variables['attributes']).'>
    <div class="top">
        <div class="top-inset-1">
            <div class="top-inset-2"></div>
        </div>
    </div>
    <div class="gallery-thumb-outer">
        <div class="gallery-thumb-inner">';
  // Create a link around the image
  $item .= empty($variables['no_link']) ? l($image, $link_path, array('html' => TRUE, 'attributes' => $attributes)) : $image;
  // Add sliding door bottom div and close wrappers
  $item .= '</div>
        </div>
        <div class="bottom">
            <div class="bottom-inset-1">
                <div class="bottom-inset-2"></div>
            </div>
        </div>
    </div>';
  return $item;
}