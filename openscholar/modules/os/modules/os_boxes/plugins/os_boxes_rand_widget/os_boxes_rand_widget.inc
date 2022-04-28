<?php

class os_boxes_rand_widget extends os_boxes_widget_collection {

  public $embed_as = array(
    'iframe',
  );
  
  // Overriding boxes_box form to execute '_os_boxes_rand_widget_field_display_handler' callback as #after_build property.
  function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    $form['advanced']['#after_build'][] = '_os_boxes_rand_widget_field_display_handler';
    return $form;
  }

  public function render() {
    $block = parent::render();

    $k = array_rand($this->options['widgets']);
    list(,$delta) = explode('-', $this->options['widgets'][$k]['bid'], 2);
    $box = boxes_box_load($delta);
    $box->title = $this->options['widgets'][$k]['title'];
    $box->options['make_embeddable'] = $this->options['make_embeddable'];
    if (!empty($this->options['additional_class'])) {
      $this->options['additional_classes'] = $this->options['additional_class'];
    }
    if ($box instanceof boxes_box) {
      return $box->render();
    }

    return $block;
  }
}

/**
 * #after_build handler in options_form to display settings form field for random widgets.
 * To hide the 'Title' field while editing random widget.
 */
function _os_boxes_rand_widget_field_display_handler($element, &$form_state) {
  $form_state['complete form']['title']['#access'] = FALSE;  
}