<?php

/**
 * App specific settings for Profiles
 */
function os_profiles_settings($form, &$form_state) {
  // make sure this file is loaded on form submit
  $form_state['build_info']['files'][] = drupal_get_path('module', 'os_profiles') . '/os_profiles.pages.inc';

  // Form element for disabling the use of a default image.
  $form['default_image'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Image'),
    '#weight' => -50
  );

  $form['default_image']['os_profiles_disable_default_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable default image for people profiles'),
    '#default_value' => variable_get('os_profiles_disable_default_image', FALSE),
    '#description' => t('If checked no image will be used when viewing the "/people" page.'),
    '#weight' => -101,
  );

  $entity_info = entity_get_info('node');
  foreach ($entity_info['view modes'] as $name => $info) {
    if (!$info['custom settings'] || $name == 'links') {
      // Display only display modes the user turned on. The "links" view mode is
      // disabled in the case of a "person" content type.
      continue;
    }

    $profile_styles[$name] = $info['label'];
  }

  $profile_styles_hover = array();
  $counter = 0;

  // Create markup for style examples when hovering over each style.
  foreach ($profile_styles as $style => $name) {
    $counter ++;
    // Prepare the popup and the style name.
    $profile_example_link = '<a href="#" class="profile-pop" data-popbox="pop'.$counter.'">' . check_plain($name) . '</a>';
    $profile_example_title = '<h2>' . $name . '</h2>';
    // Prepare the profile style example for the popup.
    $profile_example = _os_profiles_build_display_style_example($style);
    $profile_example_text = '<p>' . $profile_example . '</p>';
    $profile_example_output = $profile_example_link . '<div id="pop'.$counter.'" class="stylebox">' . $profile_example_title . $profile_example_text . '</div>';
    $profile_styles_hover[$style] = $profile_example_output;
  }

  // Form element to choose display type. We use a "dummy" variable to be able
  // to get the value that is set in the vsite level and not the site-wide
  // level value.
  $form['os_profiles_display_type'] = array(
    '#type' => 'radios',
    '#title' => t('Display types'),
    '#default_value' => variable_get('os_profiles_display_type', 'teaser'),
    '#options' => $profile_styles_hover,
    '#description' => t('Choose the display type of a person in the "/people" page.'),
    '#weight' => -999,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'os_profiles') . '/misc/os_profiles_settings.css',
      ),
      'js' => array(
        drupal_get_path('module', 'os_profiles') . '/misc/os_profiles_settings_hover.js',
      )
    ),
  );


  $instance = field_info_instance('node', 'field_person_photo', 'person');
  if ($instance['widget']['settings']['resolution']) {
    list($width, $height) = explode('x', $instance['widget']['settings']['resolution']);
  }
  $dimensions = variable_get('os_profiles_default_image_properties', array(
    'x' => 0,
    'y' => 0,
    'height' => $height,
    'width' => $width,
  ));
  $dimensions['ratio'] = $dimensions['width']/$dimensions['height'];

  // Show the default image used before a custom default image has been uploaded.
  $path = variable_get('profile_default_photo_large', drupal_get_path('theme', 'os_basetheme') . '/images/person-default-image-large.png');
  $image = '<div class="field-name-field-person-photo">' . theme('image',  array('path' => $path)) . '</div>';

  // Form element for uploading a default image.
  $allowed_file_types = 'gif png jpg jpeg';
  global $language;

  $form['default_image']['os_profiles_default_image_file'] = array(
    '#type' => 'managed_file',
    '#field_parents' => array(),
    '#field_name' => array(),
    '#language' => $language->language,
    '#field_prefix' => '<div class="">',
    '#field_suffix' => '</div>' . $image,
    '#default_value' => variable_get('os_profiles_default_image_file', NULL),
    '#description' => t('The default image will be used if a profile photo is not available. Instead, you can upload your own default image.<br/>Position the cropping tool over it if necessary. Allowed file types: <strong> @allowed_file_types </strong>', array('@allowed_file_types' => $allowed_file_types)),
    '#upload_validators' => array(
      'file_validate_extensions' => array($allowed_file_types),
    ),
    '#process' => array('_os_profiles_default_image_process'),
    '#dimensions' => $dimensions,
    '#attached' => array(
      'css' => array(
        '.form-managed-file img { max-width: none; }' => array(
          'type' => 'inline'
        )
      ),
    ),
    '#weight' => -100,
  );

  // Set upload location.
  if (module_exists('vsite') && $space = spaces_get_space()) {
    $form['file']['#upload_location'] = 'public://' . $space->group->purl . '/files';
  }


  $form['#submit'][] = 'os_profiles_settings_submit';
  return system_settings_form($form);
}

/**
 * Process function; For uploading a default profile image.
 */
function _os_profiles_default_image_process($element, $form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element = os_files_imagefield_crop_process($element, $form_state, $form);
  $element['remove_button']['#submit'][] = '_os_image_remove_handler';

  // When pressing "remove" call the original form's submit handler in order to
  // set the default image file id to 0.
  $element['remove_button']['#submit'][] = 'os_profiles_display_settings_submit';

  module_load_include('inc', 'comment_sources', 'comment_sources.admin');
  $fid = isset($element['#value']['fid']) ? $element['#value']['fid'] : 0;

  // always include this, whether there's a file or not
  $settings = array(
    $element['#id'] => array(
      'box' => array(
        'ratio' => $element['#dimensions']['ratio'],
        'box_width' => 400,
        'box_height' => 400,
      ),
      'minimum' => array(
        'width' => NULL,
        'height' => NULL,
      ),
    ),
  );
  $element['#attached']['js'][] = array(
    'data' => array('imagefield_crop' => $settings),
    'type' => 'setting',
    'scope' => 'header',
  );
  $element['#attached']['js'][] = drupal_get_path('module', 'imagefield_crop').'/Jcrop/js/jquery.Jcrop.js';
  $element['#attached']['js'][] = drupal_get_path('module', 'imagefield_crop').'/imagefield_crop.js';
  $element['#attached']['css'][] = drupal_get_path('module', 'imagefield_crop').'/Jcrop/css/jquery.Jcrop.css';

  // A custom default image has been uploaded.
  if ($fid && $element['#file']) {
    // Remove the preview of the default image.
    unset($element['#field_prefix']);
    unset($element['#field_suffix']);

    unset($element['filename']);
    // file_to_crop() take the cropped image and gets the original back
    $file_to_crop = _imagefield_crop_file_to_crop($element['#file']->fid);
    $element['preview'] = array(
      '#type' => 'markup',
      '#file' => $file_to_crop, // This is used by the #process function
      '#prefix' => '<div class="imagefield-crop-preview">',
      '#suffix' => '</div>',
      '#process' => array('_os_profiles_default_image_preview_process'),
      '#theme' => 'imagefield_crop_preview',
      '#markup' => theme('image', array(
          'path' => $element['#file']->uri,
          'getsize' => FALSE,
          'attributes' => array('class' => 'preview-existing'))
      ),
    );
    $element['cropbox'] = array(
      '#markup' => theme('image', array(
          'path' => $file_to_crop->uri,
          'attributes' => array(
            'class' => 'cropbox',
            'id' => $element['#id'] . '-cropbox',
          )
        )
      ),
    );

    $element['x'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['x'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-x'
        )
      )
    );

    $element['y'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['y'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-y'
        )
      )
    );

    $element['height'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['height'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-height'
        )
      )
    );

    $element['width'] = array(
      '#type' => 'hidden',
      '#default_value' => $element['#dimensions']['width'],
      '#attributes' => array(
        'class' => array(
          'edit-image-crop-width'
        )
      )
    );
  }

  return $element;
}

/**
 * Preview process function; For previewing an uploaded default profile image.
 */
function _os_profiles_default_image_preview_process($element, $form_state, $form) {
  $element = @imagefield_crop_widget_preview_process($element, $form_state, $form);

  $scale = null;
  if (module_exists('vsite') && $vsite = spaces_get_space()) {
    $instance = field_info_instance('node', 'field_person_photo', 'person');
    if ($instance['widget']['settings']['resolution']) {
      $scale = explode('x', $instance['widget']['settings']['resolution']);
      $scale = array(
        'width' => $scale[0],
        'height' => $scale[1],
      );
    }
  }

  $element['#attached']['js'][0]['data']['imagefield_crop']['edit-file']['preview']['width'] = isset($scale) ? $scale['width'] : 0;
  $element['#attached']['js'][0]['data']['imagefield_crop']['edit-file']['preview']['height'] = isset($scale) ? $scale['height'] : 0;

  $element['#imagefield_crop']['#width'] = isset($scale) ? $scale['width'] : 0;
  $element['#imagefield_crop']['#height'] = isset($scale) ? $scale['height'] : 0;

  return $element;
}

/**
 * App settings form submit
 * Occurs before all the settings are saved to the space
 */
function os_profiles_settings_submit($form, $form_state) {
  $file_data = $form_state['input']['os_profiles_default_image_file'];

  $vsite = vsite_get_vsite();

  // save file usage and dimension information
  if ($file_data['fid']) {
    // An image was uploaded.
    $file = file_load($file_data['fid']);

    $item = array(
      'cropbox_x' => $file_data['x'],
      'cropbox_y' => $file_data['y'],
      'cropbox_height' => $file_data['height'],
      'cropbox_width' => $file_data['width'],
    );

    $scale = null;
    if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
      $instance = field_info_instance('node', 'field_person_photo', 'person');
      if ($instance['widget']['settings']['resolution']) {
        $scale = explode('x', $instance['widget']['settings']['resolution']);
        $scale = array(
          'width' => $scale[0],
          'height' => $scale[1],
        );
      }
    }

    // This creates two images. $orig is the original, unmodified image. It is
    // saved so we can refer to it later. $file is original uploaded image that
    // is then cropped and is displayed in the rendered output.
    // It relates to the original file, through the file usage table.
    $orig = _imagefield_crop_file_to_crop($file->fid);
    if ($orig->fid == $file->fid) {
      $orig = imagefield_crop_create_copy($file);
      file_usage_add($orig, 'imagefield_crop', 'file', $file->fid);
    }
    _imagefield_crop_resize(drupal_realpath($orig->uri), $item, $scale, $file);
    file_save($file);

    if (module_exists('vsite') && $space = spaces_get_space() && $file->fid) {
      // The fid is saved by spaces, so we don't need to worry about it.
      // Save the state of the crop tool image.
      $dimensions = array(
        'x' => $file_data['x'],
        'y' => $file_data['y'],
        'width' => $file_data['width'],
        'height' => $file_data['height'],
      );
      $vsite->controllers->variable->set('os_profiles_default_image_properties', $dimensions);
    }
  }
  else {
    // No file is uploaded.
    // Delete dimension information.
    $vsite->controllers->variable->del('os_profiles_default_image_properties', 0);
  }

  // Clear the os_profiles view cache so the setting will take effect.
  if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
    cache_clear_all('og:node:' . $vsite->id . ':', 'cache_views_data', TRUE);
  }

  overlay_request_page_refresh();
}
