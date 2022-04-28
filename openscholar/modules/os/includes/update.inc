<?php

/**
 * Common-use utility functions for deploying updates.
 */

/**
 * Invokes variable_set() on an array of variables and values.
 *
 * Use this instead of features_revert() when possible.
 *
 * Use whenever a strongarm variable export has been added, updated or removed
 * and production instances will require the value stored in the database to
 * be modified.
 *
 * @param array $variables
 *   An indexed array, where the key is a variable name, and the value is the
 *   newly exported or newly modified value.
 *
 * @return string $message
 *   Status message(s) reporting on what variables have been set.
 *
 * Example usage:
 * @code
 * function mymodule_update_7000() {
 *   module_load_include('inc', 'os', 'includes/update');
 *   $variables = array(
 *     'myvariable' => 'myvalue',
 *   );
 *   $message = _os_update_strongarm($variables);
 *   return $message;
 * }
 * @endcode
 */
function _os_update_strongarm($variables) {
  $message = '';
  foreach ($variables as $name => $value) {
    variable_set($name, $value);
    $replacements = array(
      '@name' => $name,
      '@value' => $value,
    );
    $message .= t('Variable "@name" has been set to @value.<br/>', $replacements);
  }
  return $message;
}
