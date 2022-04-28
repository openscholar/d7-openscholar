<?php

function grouper_field_attach_create_bundle($entity_type, $bundle) {
  if (og_is_group_content_type($entity_type, $bundle)) {
    $instance = grouper_get_field_instance($entity_type, $bundle);
    field_create_instance($instance);
  }
}