<?php

/**
 * Filter by node_access records and vsite privacy.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_vsite_access extends views_handler_filter_node_access {

  /**
   * For a non-views query based implementation,
   * @see _node_access_where_sql()
   */
  function query() {
    // Add the node access grants if there are any
    parent::query();

    // @todo If you have 'administer nodes' allow full access
    //!user_access('administer nodes')
    if ($group_table = $this->query->ensure_table('node', 'node_og_membership')) {

      $public_nid = 1;
      $group_cond = db_and()->condition($group_table . '.nid', $public_nid);

      $this->query->add_where('AND', $group_cond);
    }
  }
}

