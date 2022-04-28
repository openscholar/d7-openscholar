<?php

class os_boxes_media extends os_boxes_default {

  public function options_defaults() {
    $options = parent::options_defaults();
    $options['fid'] = array(
      'fid' => 0
    );
    $options['maxwidth'] = 0;
    return $options;
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $saved = is_array($this->options['fid']) ? $this->options['fid']['fid'] : $this->options['fid'];
    $fid = (isset($form_state['input']['fid'])) ? $form_state['input']['fid'] : $saved;
    $form['fid'] = array(
      '#title' => t('File or HTML'),
      '#type' => 'media',
      '#value' => array('fid' => $fid),
      '#description' => t('Choose, enter or link to the media you wish to embed.'),
      '#media_options' => array(
        'global' => array(
          'id' => 'media_embed_widget',
          'types' => array(
            'image',
            'audio',
            'video',
            'html'
          ),
          // Example: array('http', 'ftp', 'flickr');
          'schemes' => array(
            'public' => 'public',
            'oembed' => 'oembed',
            'embed' => 'embed',
          ),
        ),
      )
    );

    $form['maxwidth'] = array(
      '#title' => t('Max Width of Media'),
      '#type' => 'textfield',
      '#default_value' => $this->options['maxwidth'],
      '#field_suffix' => 'px',
      '#description' => t('To change the width of the media, enter a maximum width value that is LESS than the the media\'s original width. 
      Entering 0 or a value larger than the media\'s original width will result in the media displaying at its default size. 
      WARNING: This may cause the video to break out of columns in your page layout. Some trial and error may be needed to ensure the 
      embed looks good in your site layout.')
    );

    $form['help'] = array(
      '#prefix' => '<p>',
      '#markup' => t('If you\'d like additional text around your image, video or iframe, you should use the Custom
        Text/HTML box. It will allow you to embed media around text.'),
      '#suffix' => '</p>',
    );

    if (module_exists('os_common')) {
      os_common_angular_apps('os-boxes');
      $form['#attached']['library'][] = array('os_common', 'FileEditorModal');
      $form['#attached']['js'][] = drupal_get_path('module', 'os_boxes') . '/misc/os_boxes.app.js';
    }

    return $form;
  }

  public function options_submit($form, $form_state) {
    parent::options_submit($form, $form_state);

    $old = $form['options']['fid']['#value']['fid'];
    $saved = is_array($this->options['fid']) ? $this->options['fid']['fid'] : $this->options['fid'];

    if ($old != $saved) {

      $id = FALSE;
      if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
        $id = $vsite->group->nid;
      }

      if ($id && $file = file_load($saved)){
        file_usage_add($file, 'os_boxes', 'node', $id, 1);
      }

      if ($old && $old_file = file_load($old)) {
        if ($id) {
          file_usage_delete($old_file, 'os_boxes', 'node', $id, 1);
        }
        file_delete($old_file);
      }
    }
  }

  public function render() {
    $block = parent::render();
    $block['allow_binding'] = true;
    if (($block['content'] = $this->get_cache()) === FALSE) {
      $fid = is_array($this->options['fid']) ? $this->options['fid']['fid'] : $this->options['fid'];
      if ($file = file_load($fid)) {
        if (!empty($this->options['maxwidth'])) {
          if (isset($file->image_dimensions)) {
            // This is an image, so we have the dimensions for it and need to scale it down.
            $ratio = $file->image_dimensions['height'] / $file->image_dimensions['width'];
            $file->image_dimensions['height'] = $ratio * $this->options['maxwidth'];
            $file->image_dimensions['width'] = $this->options['maxwidth'];
          }
          else {
            // Override both maxwidth and width just in case.
            $file->override['maxwidth'] = $file->override['width'] = $this->options['maxwidth'];
            // Setting this to 0 enforces the aspect ratio for oembed.
            // Without it, the oembed would be taller than normal
            // it's ignored for iframe-type embeds.
            $file->override['maxheight'] = $file->override['height'] = 0;
          }
        }

        $render = file_view($file, 'full');
        // If embedded object is an image, then $file->image_dimensions will be set.
        if (isset($file->image_dimensions)) {
          $render['file']['file-info']['file']['#width'] = $file->image_dimensions['width'];
          $render['file']['file-info']['file']['#height'] = $file->image_dimensions['height'];
        }

        if (isset($render['file']['file-info']['file'])) {
          $render['file'] = $render['file']['file-info']['file'];
        }

        // For oembed media objects, need to define additional pre_render callback to control final output dimension.
        if (isset($file->oembed)) {
          $render['file']['#pre_render'][] = 'os_boxes_media_oembed_pre_render';
        }

        if (isset($file->oembed) && $file->oembed['provider'] == 'box') {
          $render['file']['#attached']['js'][] = drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_media/os_boxes_media.box_oembed.js';
          $render['file']['#attached']['js'][] = array(
            'type' => 'setting',
            'data' => array(
              'osBoxesOembedMedia' => array(
                $this->delta => $file->oembed['widget_type']
              )
            )
          );
        }
        // If there is any non-zero overridden value for max width for the widget.
        if (!empty($this->options['maxwidth']) && $this->options['maxwidth'] != 0) {
          $render['file']['#attached']['js'][] = array(
            'type' => 'setting',
            'data' => array(
              'widget_max_width' => array('boxes-box-' . $this->delta => $this->options['maxwidth'])
            )
          );
        }
        if (!empty($render['#file']->field_file_image_alt_text)) {
          $render['file']['#alt'] = $render['#file']->field_file_image_alt_text[LANGUAGE_NONE][0]['safe_value'];
        }
        if (!empty($render['#file']->field_file_image_title_text)) {
          $render['file']['#title'] = $render['#file']->field_file_image_title_text[LANGUAGE_NONE][0]['safe_value'];
        }
        // Removing embedded media title from render element.
        unset($render['file']['file-info']['title']);
        $block['content'] = drupal_render($render);
        $assets = NULL;
        if (isset($render['file']['#attached'])) {
          $assets = $render['file']['#attached'];
        }
        $block['assets'] = $assets;
        $this->set_cache($block['content'], $assets);
      }
      else {
        $block['content'] = '';
        $this->set_cache($block['content']);
      }
    }
    return $block;
  }
}

/**
 * #pre_render calback for oembed objects to control the dimension as per max width settings.
 * Also sets the height for oembed objects based on aspect ratio.
 */
function os_boxes_media_oembed_pre_render($embed) {
  if (isset($embed['#width']) && $embed['#width'] < $embed['#embed']['width']) {
    $embed['#embed']['width'] = $embed['#width'];    
  }
  if (isset($embed['#height']) && $embed['#height'] < $embed['#embed']['height']) {
    $embed['#embed']['height'] = ceil($embed['#height']);    
  }
  return $embed;
}
