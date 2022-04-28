<?php 

/**
 * @file file_image.inc
 * 
 * Image plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_file_image'),
  'entity_type' => array('file'),
  'bundle' => array('image'),
);

class sv_list_file_image extends sv_list_plugin  {
  /**
   * @function entities_alter
   * 
   * Lists of images are special.  Mark them, so os_files_file_view knows what to do.
   */
  public function entities_alter($options, &$entities) {
    if ($options['content_type'] == 'image') {
      foreach ($entities as &$entity) {
        $entity->sv_list_of_images = TRUE;
      } 
    }
  }
}
