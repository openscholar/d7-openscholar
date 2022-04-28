<?php 

/**
 * @file node_book.inc
 * 
 * book plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_book'),
  'entity_type' => array('node'),
  'bundle' => array('book'),
);

class sv_list_node_book  extends sv_list_plugin {

  /**
   * @function options_defaults().
   *
   * Sets values of options in new boxes.
   */
  function options_defaults() {
    $options = array(
      'include_child_pages' => FALSE,
    );

    return $options;
  }

  /**
   * @function options_form().
   *
   * Alter the box's options form.
   */
  public function options_form($options, &$form) {
    // Add a checkbox to include child pages in the results.
    $form['content']['include_child_pages'] = array(
      '#title' => t('Include child book pages'),
      '#type' => 'checkbox',
      '#default_value' => $options['include_child_pages'],
      '#states' => array(
        // Show this checkbox when the content type is "book page".
        'visible' => array(
          ':input[name="content_type"]' => array('value' => 'book'),
        ),
      ),
    );
  }

  /**
   * @function sort_alter
   * 
   * People should use last name, not first.
   */
  public function query_alter($options, $efq) {
    if ($options['content_type'] != 'book') {
      return;
    }

    $query = db_select('menu_links', 'ml')
      ->fields('ml', array('link_path'))
      ->condition('module', 'book');

    // Limit book depth to 1 if "include child book pages" is unchecked.
    if (empty($options['include_child_pages'])) {
      $query->condition('depth', 1);
    }
    $nids = $query->execute()->fetchCol();
    $nids = preg_replace('/node\//', '', $nids);
    $nids = array_values($nids);

    if ($nids) {
      $efq->propertyCondition('nid', $nids, 'IN');
    }
  }
}
