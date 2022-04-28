<?php

/**
 * @file
 * Filtering a file with by the real file name or file title.
 */

/**
 * Filter feed items by whether or not they were imported to the VSite.
 */
class os_reader_filter_feed_is_imported extends views_handler_filter {
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => array(
        'imported' => t('Imported'),
        'not_imported' => t('Not Imported'),
      ),
    );
  }

  function query() {
    if (empty($this->view->exposed_raw_input['feed_is_imported'])) {
      // 'All' is selected, no need to filter.
      return;
    }

    $input_value = $this->view->exposed_raw_input['feed_is_imported'];

    // Get current VSite.
    $vsite = vsite_get_vsite();

    if ($input_value == 'imported') {
      // 'Imported' was selected.
      $table = 'field_data_field_vsites_reference';
      $field = 'field_vsites_reference_target_id';

      // Adding relationship for the table of the field.
      $this->query->add_table($table, $this->relationship);

      // Alter the query.
      $this->query->add_where($this->options['group'], "{$table}.{$field}", $vsite->id, '=');
    }
    else {
      // 'Not Imported' was selected.

      // Get IDs of all feed items that were imported to the VSite.
      $query = new EntityFieldQuery();
      $result = $query
        ->entityCondition('entity_type', 'os_feed_item')
        ->fieldCondition('field_vsites_reference', 'target_id', $vsite->id)
        ->execute();

      if (empty($result['os_feed_item'])) {
        // No feed items were imported, display them all.
        return;
      }
      $imported_feed_ids = array_keys($result['os_feed_item']);

      // Alter the query so it will exclude the imported feed IDs.
      $this->query->add_where($this->options['group'], '{os_feed_item}.{id}', $imported_feed_ids, 'NOT IN');
    }
  }

  function admin_summary() {
    return t('Is Imported');
  }
}
