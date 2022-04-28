<?php

class SlideshowSlideNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['image'] = array(
      'property' => 'field_image',
    );

    $public_fields['link'] = array(
      'property' => 'field_link',
    );

    $public_fields['headline'] = array(
      'property' => 'field_headline',
    );

    $public_fields['description'] = array(
      'property' => 'field_description',
    );

    $public_fields['slideshow_alt_text'] = array(
      'property' => 'field_slideshow_alt_text',
    );

    $public_fields['slideshow_title_text'] = array(
      'property' => 'field_slideshow_title_text',
    );

    return $public_fields;
  }

  /**
   * {@inheritdoc}
   */
  protected function isValidEntity($op, $entity_id) {
    global $user;

    if (in_array('administrator', $user->roles)) {
      return TRUE;
    }

    return parent::isValidEntity($op, $entity_id);
  }

}
