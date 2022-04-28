<?php

/**
 * @file
 * The class for "Feed Reader" type widgets.
 */
class os_boxes_feedreader extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults(){
    $options = parent::options_defaults();

    $options['feed'] = false;
    $options['number_of_items'] = 5;
    $options['feed_display'] = 'title';
    $options['time_display'] = 'formal';

    // force this so caching works
    $options['content_type'] = 'feed';

    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state){
    $options = array();
    $form = parent::options_form($form_state);
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')->propertyCondition('status', 1)->propertyCondition('type', 'feed')->propertyOrderBy('title');

    // Restrict to this site
    if (module_exists('vsite') && $space = spaces_get_space()) {
      $query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $space->id);
    }

    $result = $query->execute();

    if (! isset($result['node']) || ! count($result['node'])) {
      $form['error'] = array(
        '#type' => 'item',
        '#title' => t('No feeds found.'),
        '#markup' => t('You must create a feed first before using this widget. ') . l("Create Feed", 'node/add/feed')
      );

      return $form;
    }

    $nodes = node_load_multiple(array_keys($result['node']));
    foreach ( $nodes as $nid => $node ) {
      // First 25 chars of title
      $options[$nid] = substr($node->title, 0, 25);
    }

    $form['feed'] = array(
      '#type' => 'select',
      '#title' => t('Feed'),
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => $this->options['feed'],
      '#description' => t('Select the feed to draw items from')
    );

    $form['feed_display'] = array(
      '#type' => 'select',
      '#title' => t('Feed item display'),
      '#options' => array('title' => t("The feed title"), 'content' => "The feed title and content summary."),
      '#required' => TRUE,
      '#default_value' => $this->options['feed_display'],
      '#description' => t('Select the way you wish the feed items to be displayed.')
    );

    $options = array(
      '0' => t('Don\'t show'),
      'relative' => t('Relative <br/><span class="description">Example: 5 minutes ago</span>'),
      'formal' => t('Full date <br/><span class="description">Example: August 21, 2013</span>'),
    );
    $default_value = 'formal';
    if (isset($this->options['time_display']) && in_array($this->options['time_display'], array_keys($options))) {
      $default_value = $this->options['time_display'];
    }
    $form['time_display'] = array(
      '#type' => 'radios',
      '#title' => t('Original date and time published'),
      '#options' => $options,
      '#required' => TRUE,
      '#default_value' => $default_value,
      '#description' => t('Select the way you wish the feed\'s original publish date and time to be displayed.')
    );

    $form['number_of_items'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of items to display'),
      '#default_value' => $this->options['number_of_items']
    );

    return $form;
  }

  /**
   * Implementation of boxes_content::render().
   */
  public function render() {
    if (module_exists('vsite') && !vsite_get_vsite()) {
      return;
    }
    $output = parent::render();

    if (! $this->options['feed'] || ! ($node = node_load($this->options['feed']))) {
      return $block;
    }

    field_attach_prepare_view('node', array($node->nid => $node), 'teaser');
    entity_prepare_view('node', array($node->nid => $node));

    $instance = field_info_instance('node', 'field_url', 'feed');
    $display = field_get_display($instance, 'teaser', $node);
    $display['settings']['num_to_show'] = $this->options['number_of_items'];

    // Just display the title and no content summary.
    $display['settings']['show_content'] = (isset($this->options['feed_display']) && ($this->options['feed_display'] == 'content'));

    // How should the time be displayed.
    $display['settings']['time_display'] = 'formal';
    if (isset($this->options['time_display'])) {
      $display['settings']['time_display'] = $this->options['time_display'];
    }

    // Here we prepare the required JS for displaying the feed.
    $feed_div_id = implode("_", array(
      "fr",
      'node',
      $node->nid,
      key($node->field_url['und']),
      $display['settings']['num_to_show'],
      $display['settings']['show_content'],
      $display['settings']['time_display']
    ));

    $url_options = array('external' => true);
    if (isset($node->field_url['und']['query'])){
      $url_options['query'] = $node->field_url['und']['query'];
    }

    $content = array(
      '#prefix' => '<div id="' . $feed_div_id . '">',
      '#suffix' => '</div>',
      '#attached' => array(
        'js' => array(
          array(
            'type' => 'setting',
            'data' => array(
              'osBoxesFeedReader' => array(
                $feed_div_id => array(
                  'url' => url($node->field_url['und'][0]['url'], $url_options),
                  'num_feeds' => $display['settings']['num_to_show'],
                  'show_content' => $display['settings']['show_content'],
                  'time_display' => $display['settings']['time_display'],
                  'rss2json_api_url' => OS_BOXES_RSS2JSON_API_URL,
                  'rss2json_api_key' => variable_get('os_boxes_rss2json_api_key', ''),
                )
              )
            ),
            'weight' => 0
          ),
          array(
            'type' => 'file',
            'data' => libraries_get_path('yui') . '/build/yui/yui-min.js',
            'weight' => 1,
          ),
          array(
            'type' => 'file',
            'data' => libraries_get_path('oop') . '/oop-min.js',
            'weight' => 2,
          ),
          array(
            'type' => 'file',
            'data' => libraries_get_path('jsonp') . '/jsonp-min.js',
            'weight' => 3,
          ),
          array(
            'type' => 'file',
            'data' => libraries_get_path('jsonp-url') . '/jsonp-url-min.js',
            'weight' => 4,
          ),
          array(
            'type' => 'file',
            'data' =>  drupal_get_path('module', 'os_boxes') . '/plugins/os_boxes_feedreader/os_boxes_feedreader.render.js',
            'weight' => 7,
          ),
        ),
      )
    );

    $output['content'] = drupal_render($content);
    return $output;
  }
}
