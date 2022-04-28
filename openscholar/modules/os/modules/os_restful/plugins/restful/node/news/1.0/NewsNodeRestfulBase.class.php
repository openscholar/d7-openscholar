<?php

class NewsNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['date'] = array(
      'property' => 'field_news_date',
    );

    $public_fields['redirect'] = array(
      'property' => 'field_url',
    );

    $public_fields['photo'] = array(
      'property' => 'field_photo',
    );

    return $public_fields;
  }

}
