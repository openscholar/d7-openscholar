<?php

class os_files_handler_field_file_uri_copy extends views_handler_field_file_uri {

  function render($values) {
    $output = file_create_url($values->{$this->field_alias});
    return os_files_clippy($output, array('bgcolor' => '#eeeeee'));
  }
}
