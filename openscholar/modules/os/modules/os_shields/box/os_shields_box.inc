<?php

/**
 * @file
 * The class for "Shield" type widgets.
 */

class os_shields_box extends os_boxes_default {

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['shield'] = variable_get('os_shields_shield', '');
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $default_shield = FALSE;
    if ($all_shields = variable_get('os_shields_shield_info', array())) {
      $shield_options = array();
      foreach ($all_shields as $file_name => $shield)  {
        if ($shield['hidden']) {
          // Shield should be hidden.
          continue;
        }

        $shield_file = file_load($shield['fid']);
        $shield_options[$shield_file->filename] = theme('os_shields_theme_shieldpicker', array('file' => $shield_file));

        if (!empty($shield['default'])) {
          $default_shield = $file_name;
        }
      }

      $selected_shield = !empty($this->options['shield']) ? $this->options['shield'] : $default_shield;

      $form['shield'] = array(
        '#prefix' => '<div class="shield_wrapper clear-block">',
        '#title' => t('Shield'),
        '#type' => 'radios',
        '#options' => $shield_options,
        '#default_value' => $this->get_shield(),
        '#suffix' => '</div>',
        '#attached' => array(
          'css' => array(
            drupal_get_path('module', 'os_shields') . '/theme/os_shields-shield-picker.css',
          ),
          'js' => array(
            drupal_get_path('module', 'os_shields') . '/theme/os_shields-shield-picker.js',
          ),
        ),
      );
    }
    if (empty($shield_options)) {
      $form['shield'] = array(
        '#type' => 'markup',
        '#value' => t('Your site has no shields to display.'),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
      );
    }

    $form += parent::options_form($form_state);
    return $form;
  }

  /**
   * Implements boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }

    $block = parent::render();
    $block['title'] = $block['content'] = '';

    $filename = $this->get_shield();

    if ($filename && file_exists('public://os_shields/' . $filename)) {
      $params = array(
        'path' => 'public://os_shields/' . $filename,
        'alt' => t('Shield'),
      );
      $image = theme('image', $params);
      $block['content'] = '<div id="scholar-shield">' . $image . '</div>';
    }
    elseif (spaces_access_admin() && !variable_get('os_shields_shield_info', array())) {
      $block['content'] = t('No shields have been added to your install. Contact your site administrator or remove this widget to hide this message.');
    }

    return $block;
  }

  /**
   * Retruns the selected shield or a default one if exists.
   */
  protected function get_shield() {
    if (!empty($this->options['shield']) && file_exists('public://os_shields/' . $this->options['shield'])) {
      // Return the selected shield.
      return $this->options['shield'];
    }

    if (($file_name = variable_get('os_shields_shield', '')) && file_exists('public://os_shields/' . $file_name)) {
      // Return the default shield.
      return $file_name;
    }

    // No selected shield, so select the one marked as "default".
    foreach (variable_get('os_shields_shield_info', array()) as $file_name => $shield)  {
      if ($shield['hidden']) {
        // Shield should be hidden.
        continue;
      }

      $shield_file = file_load($shield['fid']);
      $shield_options[$shield_file->filename] = theme('os_shields_theme_shieldpicker', array('file' => $shield_file));

      if (!empty($shield['default'])) {
        return $file_name;
      }
    }
  }
}
