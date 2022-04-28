<?php
class os_boxes_html extends os_boxes_default {

  public $embed_as = array(
    'iframe',
    'oembed',
  );

  public function options_defaults() {
    $options = parent::options_defaults();

    $options['text'] = array(
      'value' => '',
      'format' => filter_default_format(),
    );
    $options['format'] = 1;

    return $options;
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    if (module_exists('os_common')) {
      os_common_angular_apps('os-boxes');
      $form['#attached']['library'][] = array('os_common', 'FileEditorModal');
      $form['#attached']['js'][] = drupal_get_path('module', 'os_boxes') . '/misc/os_boxes.app.js';
    }

    $form['text'] = array(
      '#type' => 'text_format',
      '#default_value' => $this->options['text']['value'],
      '#format' => isset($this->options['text']['format'])?$this->options['text']['format']:NULL,
      '#title' => t('Content'),
      '#description' => t('You can put almost anything into here to place it in a widget. Text, HTML, Images, Video, whatever works in the editor.')
    );

    array_unshift($form['#attached']['js'], drupal_get_path('module', 'os_wysiwyg') . '/js/wrap_plugins.js');
    array_unshift($form['#attached']['js'], drupal_get_path('module', 'os_wysiwyg') . '/os_wysiwyg.widget.js');

    $form['files'] = array(
      '#type' => 'value',
      '#value' => $this->files(),
    );

    return $form;
  }

  public function options_submit($form, $form_state) {
    parent::options_submit($form, $form_state);

    // Only run this block if we're in a vsite.
    // Don't just return early because we need the $vsite variable later
    if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
      // Construct a fake node so we can pass it into os_wysiwyg's helper function.
      $old_files = $form_state['values']['files'];
      $new_files = $this->files();

      // Copied from _media_filter_add_file_usage_from_fields in media.filter.inc.
      foreach ($old_files as $fid => $old_file_count) {
        // Were there more files on the node just prior to saving?
        if (empty($new_files[$fid])) {
          $new_files[$fid] = 0;
        }
        if ($old_file_count > $new_files[$fid]) {
          $deprecate = $old_file_count - $new_files[$fid];
          // Now deprecate this usage.
          if($file = file_load($fid)) {
            file_usage_delete($file, 'media', 'node', $vsite->group->nid, $deprecate);
          }
          // Usage is deleted, nothing more to do with this file.
          unset($new_files[$fid]);
        }
        // There are the same number of files, nothing to do.
        elseif ($new_files[$fid] ==  $old_file_count) {
          unset($new_files[$fid]);
        }
        // There are more files now, adjust the difference for the greater number.
        // file_usage incrementing will happen below.
        else {
          // We just need to adjust what the file count will account for the new
          // images that have been added since the increment process below will
          // just add these additional ones in.
          $new_files[$fid] = $new_files[$fid] - $old_file_count;
        }
      }

      // Each entity revision counts for file usage. If versions are not enabled
      // the file_usage table will have no entries for this because of the delete
      // query above.
      foreach ($new_files as $fid => $entity_count) {
        if ($file = file_load($fid)) {
          file_usage_add($file, 'media', 'node', $vsite->group->nid, $entity_count);
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function validate(&$values) {
    if (empty($values['text']['value'])) {
      form_set_error('', t('If the content is empty, you may have difficulties displaying the widget.'));
    } else if (strlen($values['text']['value']) < 65000) {
        return;
    } else {
        form_set_error('', t('The text you entered is more than 65,000 characters. Please shorten the text before re-submitting the form.'));
    }
  }

  public function render() {
    $block = parent::render();

    if (($block['content'] = $this->get_cache()) === FALSE) {
      $block['content'] = check_markup($this->options['text']['value'], $this->options['text']['format']);
      $this->set_cache($block['content'], array('js' => drupal_add_js('//cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.1/MathJax.js?config=TeX-MML-AM_CHTML', 'external')));
    }

    return $block;
  }

  // copied from media_filter_parse_from_fields in media.filter.inc
  private function files() {
    $file_references = array();
    preg_match_all(MEDIA_TOKEN_REGEX, $this->options['text']['value'], $matches);
    foreach ($matches[0] as $tag) {
      $tag = str_replace(array('[[', ']]'), '', $tag);
      $tag_info = drupal_json_decode($tag);
      if (isset($tag_info['fid']) && $tag_info['type'] == 'media') {
        @$file_references[$tag_info['fid']] += 1;
      }
    }

    preg_match_all(MEDIA_TOKEN_REGEX_ALT, $this->options['text']['value'], $matches_alt);
    foreach ($matches_alt[0] as $tag) {
      $tag = urldecode($tag);
      $tag_info = drupal_json_decode($tag);
      if (isset($tag_info['fid']) && $tag_info['type'] == 'media') {
        @$file_references[$tag_info['fid']] += 1;
      }
    }

    return $file_references;
  }
}
