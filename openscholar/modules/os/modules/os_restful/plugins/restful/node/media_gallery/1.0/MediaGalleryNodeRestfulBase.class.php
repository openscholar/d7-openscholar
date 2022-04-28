<?php

class MediaGalleryNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    // Body field Isn't attached.
    unset($public_fields['body']);

    $public_fields['columns'] = array(
      'property' => 'media_gallery_columns',
    );

    $public_fields['rows'] = array(
      'property' => 'media_gallery_rows',
    );

    $public_fields['files'] = array(
      'property' => 'media_gallery_file',
    );

    return $public_fields;
  }

}
