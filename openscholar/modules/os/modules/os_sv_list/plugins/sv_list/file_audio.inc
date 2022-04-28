<?php 

/**
 * @file file_audio.inc
 * 
 * audio plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_file_audio'),
  'entity_type' => array('file'),
  'bundle' => array('audio'),
);

class sv_list_file_audio extends sv_list_plugin  {
  /**
   * @function entities_alter
   * 
   * Lists of audios are special.  Mark them, so os_files_file_view knows what to do.
   */
  public function entities_alter($options, &$entities) {
    if ($options['content_type'] == 'audio') {
      foreach ($entities as &$entity) {
        $entity->sv_list_of_audio = TRUE;
      } 
    }
  }
}
