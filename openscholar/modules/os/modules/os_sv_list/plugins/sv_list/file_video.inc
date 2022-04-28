<?php 

/**
 * @file file_video.inc
 * 
 * Video plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_file_video'),
  'entity_type' => array('file'),
  'bundle' => array('video'),
);

class sv_list_file_video extends sv_list_plugin  {
  /**
   * @function entities_alter
   * 
   * Lists of videos are special.  Mark them, so os_files_file_view knows what to do.
   */
  public function entities_alter($options, &$entities) {
    if ($options['content_type'] == 'video') {
      foreach ($entities as &$entity) {
        $entity->sv_list_of_videos = TRUE;
      } 
    }
  }
}
