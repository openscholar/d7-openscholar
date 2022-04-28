<?php

class os_boxes_follow extends os_boxes_default {

  public $embed_as = array(
    'iframe',
  );

  public function options_defaults() {
    return parent::options_defaults() + array(
      'links' => array(),
      'display_service_name' => 1,
      'use_rss' => 0,
    );
  }

  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);

    // add js and settings
    $domains = os_follow_default_networks();
    drupal_alter('os_follow_networks', $domains);
    $form['#attached']= array(
      'js' => array(
        array(
          'data' => array('follow_networks' => $domains),
          'type' => 'setting',
        ),
        array(
          'data' => drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_follow/misc/os_boxes_follow.js',
          'type' => 'file',
        ),
      ),
      'css' => array(
        drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_follow/misc/os_follow.css',
      ),
    );

    $domain_list = array();
    foreach ($domains as $d) {
      $domain_list[] = $d['title'];
    }

    $form['add'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add New Profile'),
      'link_to_add' => array(
        '#type' => 'textfield',
        '#title' => t('Profile URL'),
        '#description' => t('Enter the URL of your profile on a social media site. Accepted sites are: @domains', array('@domains' => implode(', ', $domain_list))),
        '#field_suffix' => '<input type="button" class="add_new" value=" Add " />',
      ),
    );

    // build a normal form
    $form['links'] = array(
      '#type' => 'markup',
      '#tree' => TRUE,
      '#pre_render' => array('os_boxes_follow_table'),
    );

    $links = &$form['links'];
    usort($this->options['links'], 'element_sort');

    $this->options['links']['blank'] = array('title' => '__blank__', 'domain' => '', 'weight' => 2000000); // blank element for copying purposes
    foreach ($this->options['links'] as $k => $v) {
      $offset = 0;
      if (isset($domains[$v['domain']])) {
        $offset = $domains[$v['domain']]['offset'];
      }
      $n = array(
        '#weight' => $v['weight'],
        'text' => array(
          '#type' => 'markup',
          '#markup' => '<span class="follow-icon" style="background-position: -'.$offset.'px 0px">'.$v['title'].'</span> <a href="#" class="remove" title="Remove Link">Remove</a>',
        ),
        'domain' => array(
          '#type' => 'hidden',
          '#default_value' => $v['domain'],
        ),
        'title' => array(
          '#type' => 'hidden',
          '#default_value' => $v['title'],
        ),
        'weight' => array(
          '#type' => 'textfield',
          '#default_value' => $v['weight'],
          '#size' => 3,
          '#attributes' => array(
            'class' => array((string)$k!='blank'?'field-weight':''),
          )
        ),
      );
      $links[$k] = $n;
    }

    // Add an option to display the service name.
    $form['display_service_name'] = array(
      '#type' => 'radios',
      '#title' => t('Display Social Media Service Name?'),
      '#description' => t('This will display the icons vertically and the name of the social media service next to each icon.'),
      '#default_value' => isset($this->options['display_service_name']) ? $this->options['display_service_name'] : 1,
      '#options' => array(t('Yes'), t('No')),
    );

    $form['use_rss'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add link to RSS Feed page to list.'),
      '#description' => t('Adds a link the page where users can select an RSS feed to subscribe to.'),
      '#default_value' => isset($this->options['use_rss']) ? $this->options['use_rss'] : 0,
    );

    $form['count'] = array(
      '#type' => 'hidden',
      '#default_value' => count($this->options['links'])-1,
      '#process' => array('os_boxes_follow_add_new_fields'),
    );

    return $form;
  }

  public function render() {
    $block = parent::render();
    $domains = os_follow_default_networks();
    drupal_add_css(drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_follow/misc/os_follow.css');
    $links = array();
    $list_class = 'follow-me-links';

    $block['content'] = '';

    if (!empty($this->options['use_rss']) && empty($this->options['links'])) {
      $block['content'] .= theme('image', array('path' => 'misc/feed.png', 'title' => t('Syndicate content'), 'alt' => t('RSS Feed')));
      $block['content'] .= l(t('RSS'), 'rss', array('attributes' => array('class' => array('rss-feed-icon'))));
    }

    if (!$this->options['links']) {
      return $block;
    }

    $links = array();

    if (!empty($this->options['use_rss'])) {
      $image = theme('image', array(
        'path' => drupal_get_path('module', 'os_boxes').'/plugins/os_boxes_rss/images/rss_square-24.png',
        'title' => t('Syndicate content'),
        'alt' => t('RSS Feed'),
        'width' => 24,
        'height' => 28
      ));
      $l = l($image, 'rss', array(
        'attributes' => array(
          'class' => array('rss-follow-icon'),
          'target' => '_blank',
        ),
        'html' => TRUE,
      ));
      if (isset($this->options['display_service_name']) && $this->options['display_service_name'] == 0) {
        $service_name = '<span class="service-name-span">'.l('RSS', 'rss', array('attributes' => array(
            'class' => array(
              'follow-icon-service-name',
            ),
            'target' => '_blank',
          ))).'</span>';
        $links[0] = $l . $service_name;
      }
      else {
        $links[0] = $l;
      }
    }

    foreach ($this->options['links'] as $n) {
      // Build the image link.
      $l = l($n['title'], $n['title'], array(
        'attributes' => array(
          'class' => 'follow-icon',
          'style' => 'background-position: -'.$domains[$n['domain']]['offset'].'px 0px',
          'target' => '_blank',
        ))
      );
      // Build the service name link.
      $b = "<span class='service-name-span'>" . l(($this->options['display_service_name'] == 0) ? $domains[$n['domain']]['title'] : $n['title'], $n['title'], array(
        'attributes' => array(
          'class' => 'follow-icon-service-name',
          'target' => '_blank',
        ))
      ) . "</span>";
      // If the option is selected to show the service name, concatenate the two.
      if($this->options['display_service_name'] == 0) {
        $links[$n['weight']] = $l . $b;
      }else {
        $links[$n['weight']] = $l;
      }
    }
    ksort($links);
    // Add the links to an array for the item_list.
    foreach($links as $social_link) {
      $items[] = array('data' => $social_link);
    }
    // Change the class of the ul depending on the option selected.
    if($this->options['display_service_name'] == 0) {
      $list_class = 'follow-me-links-vertical';
    }
    // Add the links to theme item_list funciton.
    $block['content'] .= theme('item_list', array(
      'items' => $items,
      'title' => '',
      'type' => 'ul',
      'attributes' => array(
        'class' => $list_class
        )
      )
    );
    return $block;
  }
}

/**
 * Adds the neccessary js to the form
 * @param unknown_type $form
 */
function os_boxes_follow_add_externals($form) {
  drupal_add_js(drupal_get_path('module', 'os_boxes').'/theme/js/os_boxes_follow.js');
  drupal_add_css(drupal_get_path('module', 'os_boxes').'/theme/css/os_follow.css');
  $networks = os_follow_default_networks();
  drupal_alter('os_follow_networks', $networks);
  drupal_add_js(array('follow_networks' => $networks), 'setting');

  return $form;
}

/**
 * Renders the list of links as a table.
 * @param unknown_type $element
 */
function os_boxes_follow_table($element) {
  // render the form as a table
  $rows = array();
  uasort($element, 'element_sort');
  foreach (element_children($element) as $c) {
    $rows[] = array(
      'data' => array(
        drupal_render($element[$c]['text']).drupal_render($element[$c]['title']).drupal_render($element[$c]['domain']),
        drupal_render($element[$c]['weight']),
      ),
      'class' => array((string)$c!='blank'?'draggable':''),
    );
  }
  $element['#headers'] = array();
  $element['#attributes'] = array('id' => 'follow-links-list');
  $element['#rows'] = $rows;
  $element['#sticky'] = false;
  $element['#empty'] = true;
  $element['#theme'] = 'table';
  drupal_add_tabledrag('follow-links-list', 'order', 'sibling', 'field-weight');
  return $element;
}

/**
 * Adds new elements to the form array so their values appear in form_state
 *
 * When we get this form back, it will have some fields added through js that weren't there
 * before. In here, we rebuild that section of the form array, and pass it through form builder
 * so form_state[values] is populated with our new fields
 * @param unknown_type $element
 * @param unknown_type $post
 * @param unknown_type $form_state
 * @param unknown_type $form
 */
function os_boxes_follow_add_new_fields(&$element, &$form_state, &$form) {
  for ($i=$element['#default_value']; $i < $element['#value']; $i++) {
    $form['options']['links'][$i] = array(
      '#attributes' => array('id' => $i),
      '#parents' => array('links', $i),
      '#array_parents' => array('options', 'links', $i),
      '#tree' => TRUE,
    );
    $form['options']['links'][$i] += $form['options']['links']['blank'];
    foreach ($form['options']['links'][$i] as $key => &$child) {
      if (strpos($key, '#') !== 0) {
        unset($child['#parents']);
        unset($child['#name']);
        unset($child['#value']);
      }
    }
    form_builder($form['form_id']['#value'], $form['options']['links'][$i], $form_state);
  }
  unset($form_state['values']['links']['blank']);

  // remove links that aren't in the post
  if (is_array($form_state['values'])) {
    foreach ($form_state['values']['links'] as $k => $v) {
      if (!isset($form_state['input']['links'][$k]) || !isset($form_state['input']['links'][$k]['domain'])) {
        unset($form_state['values']['links'][$k]);
      }
    }
  }

  return $element;
}

/**
 * Retrieves the default networks available.
 * Copied from Follow module
 *
 * @return
 *   An associative array, keyed by the machine name. The values are an array
 *   including title of the network, along with the domain to be used for
 *   input validation of the network.
 */
function os_follow_default_networks() {
  $networks = array(
    'facebook'  => array(
      'title' => t('Facebook'),
      'domain' => 'facebook.com',
      'offset' => 531,
    ),
    'virb'      => array(
      'title' => t('Virb'),
      'domain' => 'virb.com',
      'offset' => 142,
    ),
    'myspace'   => array(
      'title' => t('MySpace'),
      'domain' => 'myspace.com',
      'offset' => 290,
    ),
    'twitter'   => array(
      'title' => t('Twitter'),
      'domain' => 'twitter.com',
      'offset' => 189,
    ),
    'google_photos'    => array(
      'title' => t('Google Photos'),
      'domain' => 'photos.google.com',
      'offset' => 676,
    ),
    'flickr'    => array(
      'title' => t('Flickr'),
      'domain' => 'flickr.com',
      'offset' => 483,
    ),
    'youtube'   => array(
      'title' => t('YouTube'),
      'domain' => 'youtube.com',
      'offset' => 94,
    ),
    'vimeo'     => array(
      'title' => t('Vimeo'),
      'domain' => 'vimeo.com',
      'offset' => 47,
    ),
    'lastfm'    => array(
      'title' => t('last.fm'),
      'domain' => 'last.fm',
      'offset' => 389,
    ),
    'linkedin'  => array(
      'title' => t('LinkedIn'),
      'domain' => 'linkedin.com',
      'offset' => 340,
    ),
    'delicious' => array(
      'title' => t('Delicious'),
      'domain' => 'del.icio.us',
      'offset' => 579,
    ),
    'tumblr'    => array(
      'title' => t('Tumblr'),
      'domain' => 'tumblr.com',
      'offset' => 241,
    ),
    'pinterest'    => array(
      'title' => t('Pinterest'),
      'domain' => 'pinterest.com',
      'offset' => 0,
    ),
    'instagram'    => array(
      'title' => t('Instagram'),
      'domain' => 'instagram.com',
     'offset' => 630,
    ),
    'soundcloud'    => array(
      'title' => t('soundcloud'),
      'domain' => 'soundcloud.com',
     'offset' => 436,
    ),
    'Google+'    => array(
      'title' => t('Google+'),
      'domain' => 'plus.google.com',
      'offset' => 722,
    ),
  );

  return $networks;
}
