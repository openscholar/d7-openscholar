<?php

class os_boxes_site_logo extends os_boxes_default {

    function options_defaults() {
      $options = parent::options_defaults();

      $options += array(
        'fid' => NULL,
        'x' => 0,
        'y' => 0,
        'height' => 100,
        'width' => 150,
      );

      return $options;
    }

    function box_form(&$form_state) {
      $form = parent::options_form($form_state);

      if (!module_exists('imagefield_crop')) {
        $form['warning'] = array(
          '#markup' => t('The imagefield_crop module must be enabled for this box to function.'),
        );
        return $form;
      }

      if (module_exists('os_common')) {
        os_common_angular_apps('os-boxes');
        $form['#attached']['library'][] = array('os_common', 'FileEditorModal');
        $form['#attached']['js'][] = drupal_get_path('module', 'os_boxes') . '/misc/os_boxes.app.js';
      }

      $fid = $this->options['fid'];
      $x = $this->options['x'];
      $y = $this->options['y'];
      $height = $this->options['height'];
      $width = $this->options['width'];
      $ratio = 0;
      $description = '';
      if (module_exists('vsite') && $vsite = spaces_get_space()) {
        $file = field_get_items('node', $vsite->group, 'field_site_logo');
        $file = $file[0];
        $fid = $file['fid'];
        $x = $file['cropbox_x'];
        $y = $file['cropbox_y'];
        $height = $file['cropbox_height'];
        $width = $file['cropbox_width'];

        $instance = field_info_instance('node', 'field_site_logo', $vsite->group->type);
        if ($instance['widget']['settings']['resolution']) {
          $ratio = explode('x', $instance['widget']['settings']['resolution']);
          $ratio = $ratio[0]/$ratio[1];
        }
        $description = $instance['description'];
      }
      $allowed_file_types = 'gif png jpg jpeg';
      $form['file'] = array(
        '#type' => 'managed_file',
        '#title' => t('Site Logo'),
        '#description' => t('Upload a photo and position the cropping tool over it if necessary.<br/>Allowed file types: <strong>@file_types</strong><br />@description',
          array(
            '@file_types' => $allowed_file_types,
            '@description' => $description,
          )
        ),
        '#process' => array(
          '_os_boxes_site_logo_process',
        ),
        '#default_value' => $fid,
        '#upload_validators' => array(
          'file_validate_extensions' => array($allowed_file_types),
        ),
        '#dimensions' => array(
          'x' => !empty($x)?$x:0,
          'y' => !empty($y)?$y:0,
          'height' => !empty($height)?$height:140,
          'width' => !empty($width)?$width:180,
          'ratio' => $ratio
        ),
        '#attached' => array(
          'css' => array(
            '.form-managed-file img { max-width: none; }' => array(
              'type' => 'inline'
            )
          )
        ),
        '#field_parents' => array(),
        '#field_name' => 'field_site_logo',
        '#language' => LANGUAGE_NONE,
      );

      if (function_exists('imagefield_crop_upload_validate')) {
        $form['file']['#upload_validators']['imagefield_crop_upload_validate'] = array(null, $instance);
      }

      if (module_exists('vsite') && $space = spaces_get_space()) {
        $form['file']['#upload_location'] = 'public://'.$space->group->purl.'/files';
      }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#submit' => array('os_boxes_site_logo_submit'),
      );
      $form['cancel'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#limit_validation_errors' => array(),
      );

      return $form;
    }

    public function options_submit($form, $form_state) {
      // do the cropping bit now
      $file_data = $form_state['input']['file'];

      $this->options['fid'] = $file->fid;

      $this->options['x'] = $file_data['x'];
      $this->options['y'] = $file_data['y'];
      $this->options['height'] = $file_data['height'];
      $this->options['width'] = $file_data['width'];
    }

    function render() {
      if (module_exists('vsite') && !vsite_get_vsite()) {
        return;
      }
      $block = parent::render();

      // get the title of the site so we can use it in an alt attribute
      $site_info = boxes_box_load('site_info');
      $site_title = $site_info->options['site_title'];

      // if we happen to be in a vsite, load the logo from their field
      $fid = $this->options['fid'];
      if (module_exists('vsite') && $vsite = spaces_get_space()) {
        $fid = field_get_items('node', $vsite->group, 'field_site_logo');
        $fid = $fid[0]['fid'];
        // if the site title is the default, use the vsite's title
        if ($site_title == 'Your Name') {
          $site_title = $vsite->group->title;
        }
      }

      $width = 180;
      $height = 140;

      try {
        if (!empty($fid) && $file = file_load($fid)) {
          $path = $file->uri;
          $width = $file->image_dimensions['width'];
          $height = $file->image_dimensions['height'];
        }
        else {
          $path = drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_site_logo/default_logo.jpg';
        }
      }
      catch (Exception $e) {
        $path = drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_site_logo/default_logo.jpg';
      }

      $output = theme('image', array('path' => $path, 'alt' => $site_title, 'width' => $width, 'height' => $height));
      $block['content'] = l($output, '<front>', array('html' => true));

      return $block;
    }
}

function os_boxes_site_logo_submit($form, $form_state) {
  $file_data = $form_state['input']['file'];
  if ($file_data['fid'] != 0) {
    $file = file_load($file_data['fid']);

    $item = array(
      'cropbox_x' => $file_data['x'],
      'cropbox_y' => $file_data['y'],
      'cropbox_height' => $file_data['height'],
      'cropbox_width' => $file_data['width'],
    );

    $scale = null;
    if (module_exists('vsite') && $vsite = spaces_get_space()) {
      $instance = field_info_instance('node', 'field_site_logo', $vsite->group->type);
      if ($instance['widget']['settings']['resolution']) {
        $scale = explode('x', $instance['widget']['settings']['resolution']);
        $scale = array(
          'width' => $scale[0],
          'height' => $scale[1],
        );
      }
    }

    // this creates two images
    // $orig is the original, unmodified image
    // its saved so we can refer to it later
    // $file is original uploaded image that is then cropped to fit
    // it is the file that is displayed in render
    // it 'contains' the orig file, through the file usage table
    $orig = _imagefield_crop_file_to_crop($file->fid);
    if ($orig->fid == $file->fid) {
      $orig = imagefield_crop_create_copy($file);
      file_usage_add($orig, 'imagefield_crop', 'file', $file->fid);
    }
    _imagefield_crop_resize(drupal_realpath($orig->uri), $item, $scale, $file);
    file_save($file);
    file_usage_add($file, 'os_boxes', 'box', 1);  // prevents errors when trying to reuse the same image
  }
  else {
    // there's no file. remove the old file, remove file_usage and set sensible defaults for the vsite
    $file = (object)array(
      'fid' => 0,
    );

    $file_data += array(
      'x' => 0,
      'y' => 0,
      'height' => 140,
      'width' => 180,
    );
  }

  if (module_exists('vsite') && $space = spaces_get_space()) {
    $node = $space->group;

    if ($file->fid) {
      $node->field_site_logo[LANGUAGE_NONE][0] = (array)$file + array(
        'cropbox_x' => $file_data['x'],
        'cropbox_y' => $file_data['y'],
        'cropbox_width' => $file_data['width'],
        'cropbox_height' => $file_data['height'],
      );
    }
    else {
      $node->field_site_logo = array();
    }

    node_save($node);
  }
  else {
    boxes_box_form_submit($form, $form_state);
  }
}

function _os_boxes_site_logo_process($element, $form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element = os_files_imagefield_crop_process($element, $form_state, $form);
  $element['remove_button']['#submit'][] = '_os_image_remove_handler';

  $fid = isset($element['#value']['fid']) ? $element['#value']['fid'] : 0;

  if ($fid && $element['#file']) {
    unset($element['filename']);
    // file_to_crop() take the cropped image and gets the original back
    $file_to_crop = _imagefield_crop_file_to_crop($element['#file']->fid);
    $element['preview'] = array(
      '#type' => 'markup',
      '#file' => $file_to_crop, // This is used by the #process function
      '#prefix' => '<div class="imagefield-crop-preview">',
      '#suffix' => '</div>',
      '#process' => array('_os_boxes_site_logo_preview_process'),
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
          'id' => $element['#id'] . '-cropbox')
        )
      ),
    );
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

function _os_boxes_site_logo_preview_process($element, $form_state, $form) {
  $element = @imagefield_crop_widget_preview_process($element, $form_state, $form);

  $scale = null;
  if (module_exists('vsite') && $vsite = spaces_get_space()) {
    $instance = field_info_instance('node', 'field_site_logo', $vsite->group->type);
    if ($instance['widget']['settings']['resolution']) {
      $scale = explode('x', $instance['widget']['settings']['resolution']);
      $scale = array(
        'width' => $scale[0],
        'height' => $scale[1],
      );
    }
  }

  $element['#attached']['js'][0]['data']['imagefield_crop']['edit-file']['preview']['width'] = isset($scale)?$scale['width']:0;
  $element['#attached']['js'][0]['data']['imagefield_crop']['edit-file']['preview']['height'] = isset($scale)?$scale['height']:0;

  $element['#imagefield_crop']['#width'] = isset($scale)?$scale['width']:0;
  $element['#imagefield_crop']['#height'] = isset($scale)?$scale['height']:0;

  return $element;
}
