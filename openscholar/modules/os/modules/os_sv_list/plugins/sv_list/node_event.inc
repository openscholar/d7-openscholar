<?php

/**
 * @file node_event.inc
 *
 * event plugin for sv_list box
 */

$plugin = array(
  'handler' => array('class' => 'sv_list_node_event'),
  'entity_type' => array('node'),
  'bundle' => array('event'),
);

class sv_list_node_event extends sv_list_plugin  {

  /**
   * Sets values of options in new boxes.
   */
  public function options_defaults() {
    return array(
      'event_expire_appear' => 'after_event_start',
      'show' => 'upcoming_events',
    );
  }

  public function options_form($options, &$form) {
    $defaults = $this->options_defaults();

    $form['content']['show'] = array(
      '#type' => 'select',
      '#title' => 'Show',
      '#default_value' => !empty($options['show']) ? $options['show'] : $defaults['show'],
      '#options' => array(
        'upcoming_events' => t('Upcoming Events'),
        'past_events' => t('Past Events'),
        'all_events' => t('All Events'),
      ),
    );

    // Add a select list when sorting events by event date.
    $form['content']['event_expire_appear'] = array(
      '#type' => 'select',
      '#title' => t('Events should expire'),
      '#default_value' => !empty($options['event_expire_appear']) ? $options['event_expire_appear'] : $defaults['event_expire_appear'],
      '#options' => array(
        'after_event_start' => t('30 minutes after an event starts'),
        'end_of_day' => t('At the end of the day'),
        'end_of_event' => t('When the event ends'),
      ),
    );
  }

  /**
   * @function register_sorts()
   *
   * Adds a custom sort for this entity/bundle
   */
  public function register_sorts() {
    return array(
      'sort_event_asc' => t('Event Date Ascending (Past to Future)'),
      'sort_event_desc' => t('Event Date Descending (Future to Past)'),
    );
  }

  /**
   * @function query_alter().
   */
  public function query_alter($options, $efq) {
    if ($options['content_type'] != 'event') {
      return;
    }

    // We do this to deal with sites that have the old "sort_event" selected.
    // This is now named "sort_event_asc".
    if ($options['sort_by'] == 'sort_event') {
      $options['sort_by'] = 'sort_event_asc';
    }

    $efq->addTag('sv_list_events');

    $events_to_show = $options['show'];
    switch ($events_to_show) {
      case 'all_events':

        // force the date field to be added to the query
        $efq->fieldDeltaCondition('field_date', NULL, 'IS NOT NULL');
        break;
      case 'past_events':
        // Show past events. For all sorts except for
        // 'Event Date Descending (Future to Past)' we get events that already
        // ended.
        if ($options['sort_by'] != 'sort_event_desc') {
          $efq->fieldCondition('field_date', 'value2', gmdate('Y-m-d H:i:s'), '<', 'sv_list_events');
        }
        break;
      default:
        // The default option is for upcoming events. For all sorts except
        // for 'Event Date Ascending (Past to Future)' we get events that
        // haven't started yet.
        if ($options['sort_by'] != 'sort_event_asc') {
          $efq->fieldCondition('field_date', 'value', gmdate('Y-m-d H:i:s'), '>=', 'sv_list_events');
        }
        break;
    }

    $this->query = $efq;
  }

  /**
   * @function sort_event_asc().
   *
   * Custom sort for events. Filters down the displayed events according to the
   * selected expiry time.
   */
  public function sort_event_asc($options, EntityFieldQuery $efq) {
    $efq->addTag('sv_list_events');
    $efq->addMetaData('sort_event', 'ASC');

    if ($options['show'] != 'upcoming_events') {
      $this->query = $efq;
      return;
    }

    // Change the query according to the selected expiry time of the events.
    switch ($options['event_expire_appear']) {
      case 'end_of_day';
        // The expiry time is at the end of the day.
        $str = 'today';
        $efq->fieldCondition('field_date', 'value2', gmdate('Y-m-d H:i:s', strtotime($str)), '>=', 'sv_list_events');
        break;
      case 'end_of_event';
        // The expiry time is at the end of the event.
        $efq->fieldCondition('field_date', 'value2', gmdate('Y-m-d H:i:s'), '>=', 'sv_list_events');
        break;
      default:
        // The default expiry time is 30 minutes after an event starts.
        $str = '-30 minutes';
        $efq->fieldCondition('field_date', 'value', gmdate('Y-m-d H:i:s', strtotime($str)), '>=', 'sv_list_events');
        $efq->fieldCondition('field_date', 'value2', gmdate('Y-m-d H:i:s'), '>=', 'sv_list_events');
    }
    $efq->fieldOrderBy('field_date', 'value', 'ASC');

    $this->query = $efq;
  }

  /**
   * @function sort_event_desc()
   *
   * Custom sort for events. Filters down the displayed events according to the
   * selected appear time.
   */
  public function sort_event_desc($options, EntityFieldQuery $efq) {
    $efq->addTag('sv_list_events');
    $efq->addMetaData('sort_event', 'DESC');
    $efq->fieldOrderBy('field_date', 'value', 'DESC');

    if ($options['show'] != 'past_events') {
      $this->query = $efq;
      return;
    }

    // Change the query according to the selected appear time of the events.
    switch ($options['event_expire_appear']) {
      case 'end_of_day';
        // The appear time is at the end of the day.
        $str = 'today';
        $efq->fieldCondition('field_date', 'value2', gmdate('Y-m-d H:i:s', strtotime($str)), '<', 'sv_list_events');
        break;
      case 'end_of_event';
        // The appear time is at the end of the event.
        $efq->fieldCondition('field_date', 'value2', gmdate('Y-m-d H:i:s'), '<', 'sv_list_events');
        break;
      default:
        // The default appear time is 30 minutes after an event starts.
        $str = '-30 minutes';
        $efq->fieldCondition('field_date', 'value', gmdate('Y-m-d H:i:s', strtotime($str)), '<', 'sv_list_events');
    }

    $this->query = $efq;
  }

  /**
   * @function sort_alter
   *
   * Ensures that all sort options have a set sorting type
   */
  public function sort_alter($options, $efq) {
    if ($options['content_type'] != 'event' || in_array($options['sort_by'], array('sort_event_desc', 'sort_event_asc'))) {
      return;
    }

    $this->sort_event_asc($options, $efq);
  }

  /**
   * @function entities_alter
   *
   * Events have been loaded, but repeated events are grouped under a single node.
   * Split them into several nodes, then re-sort and slice them.
   */
  public function entities_alter($options, &$entities) {
    if ($options['content_type'] != 'event') {
      return;
    }

    // The EntityFieldQuery has the raw DB data on it's ordered_results property
    // We earlier modified this query to give a separate row for each nid-delta
    // So we need to add a value to the entities array for each row in the raw
    // DB Every entity we need should already be in the entities array.
    $output = array();
    $raw = $this->query->ordered_results;

    // use this instead of a foreach because this query can have more results
    // than expected.
    for ($i=0; $i<$options['number_of_items']; $i++) {
      if (empty($raw[$i])) {
        continue;
      }

      if ($row = $raw[$i]) {
        if (empty($entities[$row->entity_id])) {
          continue;
        }
        $entity = clone $entities[$row->entity_id];
        $entity->date_id = implode('.', array('date', $entity->nid, 'field_date', $row->delta));
        $output[] = $entity;
      }

      if ($i >= $options['number_of_items'] - 1) {
        break;
      }
    }

    $entities = $output;
  }

  /**
   * @function render
   *
   * The default entity_view() render function loads new copies of the entities.
   * We can't allow that because it'll invalidate entities_alter().  Instead,
   * just call node_view on each of them.
   */
  public function render($options, &$entities) {
    if ($options['content_type'] != 'event') {
      return;
    }

    $render = array();
    foreach ($entities as $node) {

      //Set as non-page node
      $new_entity = clone $node;
      unset($new_entity->page);
      $new_entity->sv_list = TRUE;

      $render[] = node_view($new_entity, $options['display']);
      //Garbage Collection
      unset($new_entity);
    }

    return $render;
  }

  /**
   * Set time cache based on options by a time string.
   *
   * @param $options
   *   Box options.
   * @param $cache_time
   *   Cache time string.
   */
  public function set_cache_time($options, &$cache_time) {
    if ($options['content_type'] == 'event') {
      $cache_time = "5 minutes";
    }
  }

  /**
   * Get the number of query results for the plugin.
   *
   * In case of events we need this number of results to determine if the "more"
   * link should be displayed. This is because repeated events are counted only
   * once when counting node IDs, and this can cause the link to not be showed
   * when it actually should.
   *
   * @return int
   *   Number of query results.
   */
  public function get_number_of_results() {
    if (isset($this->query)) {
      return count($this->query->ordered_results);
    }
  }
}
