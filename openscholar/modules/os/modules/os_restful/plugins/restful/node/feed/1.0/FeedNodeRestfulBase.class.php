<?php

class FeedNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['field_url'] = array(
      'property' => 'field_url',
      'required' => TRUE,
    );

    return $public_fields;
  }

}
