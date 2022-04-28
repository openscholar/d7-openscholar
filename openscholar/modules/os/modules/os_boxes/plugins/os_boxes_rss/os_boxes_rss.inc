<?php

/**
 * @file
 * The class for "RSS" type widgets.
 */
class os_boxes_rss extends os_boxes_default {

  public $embed_as = array(
    'iframe',
    'oembed',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults(){
    $options = parent::options_defaults();

    $options['show_all'] = true;
    $options['content'] = array();
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $form = parent::options_form($form_state);
    $form['show_all'] = array(
        '#type' => 'checkbox',
        '#title' => t('Show all content'),
        '#default_value' => $this->options['show_all'],
    );

    $types = os_get_bundles(array(2,1));
    $form['content'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Content Types to Display'),
        '#default_value' => $this->options['content'],
        '#options' => array_filter($types),
        '#states' => array(
            // Hide the settings when the show all checkbox is checked.
            'invisible' => array(
                ':input[name="show_all"]' => array(
                    'checked' => TRUE
                )
            ),
        )
    );

    $form += parent::options_form($form_state);
    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    // @todo
    $block = parent::render();
    $render = array(
      '#attached' => array(
        'js' => array(
            drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_rss/os_boxes_rss.js',
          ),
        ),
      );

    $arg = '';
    $types = array_filter($this->options['content']);
    if (!$this->options['show_all'] && count($types)) {
      $arg = '/' . implode("+", $types);
    }

    $vars = array(
      'path' => drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_rss/images/rss_square-24.png',
      'title' => t('Syndicate content'),
      'alt' => t('RSS Feed')
    );
    if ($image = theme('image', $vars)) {
      $rss_feed_url = url("feed" . $arg , array('absolute' => TRUE));
      $block['content'] = l($image . " <span id='rss_label'>Subscribe</span> <span id ='rss_feed'>" . $rss_feed_url . "</span>", NULL, array(
        'html' => TRUE,
        'attributes' => array(
            'class' => 'feed-icon',
        ),
        'external' => TRUE,
      ));
      $block['content'] .= drupal_render($render);
    }

    return $block;
  }

}
