<?php

class ClassMaterialNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['parent'] = array(
      'property' => 'field_class',
    );

    return $public_fields;
  }

}
