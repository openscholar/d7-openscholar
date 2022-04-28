<?php

class os_boxes_accordion extends os_boxes_widget_collection {

  public $embed_as = array(
    'iframe',
  );

  function options_defaults() {
    return parent::options_defaults() + array(
      'collapsed' => false,
    );
  }

  function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    // Remove embeddable option.
    unset($form['make_embeddable']);

    $form['collapsed'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['collapsed'],
      '#title' => t('All Panels Closed?'),
      '#description' => t('By default, the first panel of the Accordion will be opened. Checking this will leave it
        closed until the user opens it.')
    );

    return $form;
  }

  function render() {
    $output = parent::render();
    $accordion_plugin_path = drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_accordion';

    // Static variable to hold multiple accordion per page.
    $os_accordion_boxes = &drupal_static('os_accordion_boxes');

    if ($os_accordion_boxes[$this->delta] >= 1) {
      $delta = $this->delta . '--' . $os_accordion_boxes[$this->delta]++;
    } else {
      $delta = $this->delta;
      $os_accordion_boxes[$this->delta] = 2;
    }

    // Disable embeddable link.
    $output['embed_link'] = FALSE;
    // disable caching for this block for now
    // if (($output['content'] = $this->get_cache()) === FALSE) {
      $content = array(
        '#prefix' => '<div class="accordion">',
        '#suffix' => '</div>',
        '#attached' => array(
          'library' => array(
            array('system', 'ui.accordion'),
          ),
          'css' => array($accordion_plugin_path . '/os_boxes_accordion.css'),
          'js' => array(
            $accordion_plugin_path . '/os_boxes_accordion.render.js',
            array(
              'type' => 'setting',
              'data' => array(
                'os_boxes' => array(
                  'accordion' => array(
                    $this->delta => array(
                      'active' => $this->options['collapsed'] ? FALSE : 0,
                      'delta' => $delta,
                    )
                  )
                )
              )
            ),
          ),
        )
      );

      foreach ($this->options['widgets'] as $k => $w) {
        $block_output = $this->render_widget($k);
        if (!empty($block_output)) {
          // hide titles for widgets within the accordian block
          unset($block_output['#block']->subject);

          $content[$k] = array(
            '#weight' => $w['weight'],
            'title' => array(
              '#prefix' => '<h3>',
              '#suffix' => '</h3>',
              '#markup' => $w['title'],
            ),
            'body' => array(
              '#prefix' => '<div class="accordion-panel">',
              '#suffix' => '</div>',
              'output' => $block_output,
            )
          );
        }
      }

      $output['content'] = drupal_render($content);
    //   $this->set_cache($output['content'], $content['#attached']);
    // }

    return $output;
  }
}
