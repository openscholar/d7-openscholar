<?php

/**
 * Menu callback; updates a single data key/value on the given $user->data.
 */
function os_tour_user_edit_ajax($user, $key) {
  // Only continues if we have a key to store a value on.
  if (empty($key)) {
    return NULL;
  }

  // Gets the current time to update 'notifications_read' value.
  switch ($key) {
    case 'notifications_read':
      $value = time();
      break;
  }
  if (!isset($value)) {
    return NULL;
  }

  // Saves the new data key/value pair to the user.
  $edit = array(
    'data' => array(
      'os_tour' => array(
        $key => $value,
      ),
    ),
  );
  user_save($user, $edit);

  // Outputs the updated key and value.
  $output = (object) array($key => $value);
  drupal_json_output($output);
}
