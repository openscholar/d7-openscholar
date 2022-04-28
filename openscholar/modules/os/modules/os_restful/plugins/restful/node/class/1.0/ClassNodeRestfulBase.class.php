<?php

class ClassNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['semester'] = array(
      'property' => 'field_semester',
    );

    $public_fields['offered_year'] = array(
      'property' => 'field_offered_year',
    );

    $public_fields['class_link'] = array(
      'property' => 'field_class_link',
    );

    return $public_fields;
  }

}
