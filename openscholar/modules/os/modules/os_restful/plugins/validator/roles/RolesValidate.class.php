<?php

class RolesValidate extends ObjectValidateBase {

  /**
   * Overrides ObjectValidateBase::publicFieldsInfo().
   */
  public function publicFieldsInfo() {
    $fields = parent::publicFieldsInfo();
    unset($fields['group_type'], $fields['rid']);

    FieldsInfo::setFieldInfo($fields['gid'])
      ->setRequired(FALSE);

    return $fields;
  }

}
