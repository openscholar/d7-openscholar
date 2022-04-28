<?php

/**
 * @file
 * The class for "Book Table of Contents" widgets.
 */
class os_boxes_booktoc extends os_boxes_default {
  /**
   * Implementation of boxes_content::options_defaults().
   */
  public function options_defaults() {
    $options = parent::options_defaults();

    $options['book_nid'] = 0;
    $options['toc_type'] = 'partial';
    $options['as_nav'] = false;
    return $options;
  }

  /**
   * Implementation of boxes_content::options_form().
   */
  public function options_form(&$form_state) {
    $form = parent::options_form($form_state);
    // custom query
    $q = db_select('book', 'b');
    $q->join('node', 'n', 'b.bid = n.nid');
    $q->fields('b', array('bid'))
      ->fields('n', array('title'))
      ->distinct();

    if (module_exists('vsite') && $vsite = spaces_get_space()) {
      $q->join('og_membership', 'ogm', 'n.nid = ogm.etid');
      $q->condition('gid', $vsite->group->nid);
    }

    $results = $q->execute();
    $options = array(-1 => 'Current Book');
    foreach ($results as $row) {
      $options[$row->bid] = $row->title;
    }
    $form['book_nid'] = array(
      '#type' => 'select',
      '#title' => t('Which Book'),
      '#options' => $options,
      '#default_value' => $this->options['book_nid'],
    );

    $form['toc_type'] = array(
      '#type' => 'select',
      '#title' => t('Render Table of Contents as'),
      '#options' => array(
        'full' => 'Full Table of Contents',
        'partial' => 'Path to current post and its children',
      ),
      '#default_value' => $this->options['toc_type'],
    );

    $form['as_nav'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['as_nav'],
      '#title' => t('Display As Navigation'),
      '#description' => t('Check this to make this table display like a menu.'),
      '#return_value' => TRUE,
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
    $block = parent::render();

    // Checks for the table of contents display position setting configured
    // in os_booklets; defaults to show widget, but may optionally hide this
    // widget and instead display table of contents in node page content.
    // @see os_booklets_settings()
    // @see os_booklets_node_view()
    $toc_position = variable_get('os_booklets_toc_position');
    if ((int) $toc_position === 1) {
      $block['title'] = '';
      $block['subject'] = '';
      return $block;
    }

    $book_nid = $this->options['book_nid'];
    if ($book_nid <= 0) {
      // the toc of the book on the current page
      $node = menu_get_object();
      if (isset($node->book['bid']) && book_type_is_allowed($node->type)) {
        $book_nid = $node->nid;
      }
      else {
        $block['title'] = $block['subject'] = '';
        return $block;
      }
    }

    $toc_type = $this->options['toc_type'];
    if (variable_get('os_books_swap_enabled', FALSE)) {
      drupal_add_js(array(
        'os_books' => array(
          'settings' => array(
            'toc_type' => isset($this->options['toc_type'])?$this->options['toc_type']:'full',
          ),
        ),
      ), 'setting');
      $toc_type = 'full';
    }

    $block = os_books_build_toc($book_nid, $toc_type) + $block;

    if ($this->options['as_nav']) {
      $this->options['additional_classes'] = 'navigation';
    }

    return $block;
  }

  /**
   * Return the cache parameters for this box
   */
  public function cache_info() {

    //Clear when book,class,class_material nodes are saved
    $cache_key = array(
      'os_og' => array('current' => 'current'),
      'node' => array('book' => 'book', 'class' => 'class', 'class_material' => 'class_material'),
    );

    return array('type' => BLOCK_CACHE_CONTENT_CACHE, 'keys' => $cache_key);
  }
}
