<?php

class PresentationNodeRestfulBase extends OsNodeRestfulBase {

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['date'] = array(
      'property' => 'field_presentation_date',
      'process_callbacks' => array(
        array($this, 'dateProcess'),
      ),
    );

    $public_fields['location'] = array(
      'property' => 'field_presentation_location',
    );

    $public_fields['slides'] = array(
      'property' => 'field_presentation_file',
    );

    return $public_fields;
  }

  /**
   * Process the time stamp to a text.
   */
  public function dateProcess($value) {
    return format_date($value, 'short');
  }

}
