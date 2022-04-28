<?php

/**
 * @file
 * Filtering a file with by the real file name or file title.
 */

/**
 * Filter feed items by text in the title or body.
 */
class os_reader_filter_feed_by_text extends views_handler_filter {
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Contains'),
    );
  }

  function query() {
    if (empty($this->view->exposed_raw_input['feed_by_text'])) {
      // Empty string, no need to filter.
      return;
    }

    $input_value = $this->view->exposed_raw_input['feed_by_text'];

    // Alter the query.
    $or = db_or();

    $or->condition('{os_feed_item}.{title}', '%' . $input_value . '%', 'LIKE')
      ->condition('{os_feed_item}.{description}', '%' . $input_value . '%', 'LIKE');

    $this->query->add_where($this->options['group'], $or);
  }

  function admin_summary() {
    return t('Contains');
  }
}
