<?php

/**
 * @file
 * The box class for "Twitter feed" type widgets.
 */

class os_boxes_twitterfeed extends os_boxes_default {

  /**
   * Create a new box.
   */
  protected function __construct() {
    parent::__construct();
    $this->cache_time = '1 hour';
  }

  public $embed_as = array(
    'iframe',
    'oembed',
  );

  /**
   * Implements boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    // Provides default values for the widget (box) settings form.
    $options['twitter_username'] = '';
    $options['num_items'] = 5;
    $options['followme_link'] = 0;
    $options['followme_text'] = t("Follow me on twitter");
    $options['followme_url'] = '';
    $options['twitter_type'] = 'Username';
    $options['exclude_retweets'] = 0;

    return $options;
  }

  /**
   * Implements boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    $form['twitter_type'] = array(
      '#type' => 'select',
      '#required' => TRUE,
      '#default_value' => $this->options['twitter_type'],
      '#options' => array('user' => t('Username'), 'hashtag' => t('Hashtag Topic'), 'search' => t('Search Term')),
      '#title' => t('Display tweets from'),
    );

    $form['twitter_username'] = array(
      '#type' => 'textfield',
      //'#title' => t('Twitter key'),
      '#required' => TRUE,
      '#default_value' => $this->options['twitter_username'],
      //'#description' => t('Can be a twitter username (prepended with @), a list (@username/listname), a hashtag (prepended with #) or a search keyword (no prefix).'),
      //'#field_prefix' => '<span style="width:1em;float:left; text-align:right; border:1px gray solid;padding:1px; background-color:silver">' . $this->options['twitter_type'] . '</span>',
      '#description' => t('Show tweets coming from a single user, a specific hashtag, or a search term.'),

    );

    $form['num_items'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of tweets'),
      '#default_value' => $this->options['num_items'],
      '#description' => t('Number of tweets to display')
    );

    $form['exclude_retweets'] = array(
      '#type' => 'checkbox',
      '#title' => t('Exclude re-tweets'),
      '#default_value' => $this->options['exclude_retweets'],
      '#description' => t('This will remove re-tweets from the tweet list and might result in fewer being displayed'),
    );

    $form['followme_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include a "follow me link"'),
      '#default_value' => $this->options['followme_link'],
      '#description' => t('This will add a more link to the bottom of this widget, which will link to your twitter page'),
      '#tree' => TRUE
    );

    ctools_include('dependent');
    ctools_add_js('dependent');

    $form['followme_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text'),
      '#default_value' => $this->options['followme_text'],
      '#description' => t("The title / label of the link."),
      '#process' => array(
        'ctools_dependent_process'
      ),
      '#dependency' => array(
        'edit-followme-link' => array(
          1
        )
      )
    );

    $form['followme_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Link'),
      '#default_value' => $this->options['followme_url'],
      '#description' => t("The url/path for this link."),
      '#process' => array(
        'ctools_dependent_process'
      ),
      '#dependency' => array(
        'edit-followme-link' => array(
          1
        )
      )
    );

    return $form;
  }

  /**
   * Implements boxes_content::render().
   */
  public function render() {
    $block = parent::render();

    if (!module_exists('twitter_pull')) {
      return $block;
    }

    if (($block['content'] = $this->get_cache()) === FALSE) {
      $twitkey = $this->options['twitter_username'];

      if ($twitkey[0] != '@' || $twitkey[0] != '#') {
        switch($this->options['twitter_type']) {
          case 'user':
            $twitkey = '@' . $twitkey;
            break;

          case 'hashtag':
            $twitkey = '#' . $twitkey;
            break;
        }
      }

      $output = twitter_pull_render($twitkey, $title = NULL, $this->options['num_items'], NULL, FALSE, $this->options['exclude_retweets']);

      // Appends a followme link.
      if ($this->options['followme_link']) {
        $output .= '<div class="twitter-followme">' . l($this->options['followme_text'], $this->options['followme_url']) . '</div>';
      }

      $block['content'] = $output;
      $this->set_cache($block['content']);
    }

    return $block;
  }
}
