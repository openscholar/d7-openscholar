<?php

/**
 * @file
 * Contains \OsRestfulLayout
 */

class OsRestfulVariables extends OsRestfulSpaces {

  protected $validateHandler = 'variables';
  protected $objectType = 'variable';

  /**
   * Verify the user have access to the manage layout.
   */
  public function checkGroupAccess() {
    $account = $this->getAccount();

    if ($account->uid == 1 || in_array('administrator', $account->roles)) {
      return TRUE;
    }

    parent::checkGroupAccess();

    if (user_access('administer group', $account)) {
      return TRUE;
    }

    if ($this->group->author->getIdentifier() != $account->uid) {
      // The current user can't manage boxes.
      $this->throwException('You are not authorised to manage the variables of the group.');
    }

    return true;
  }

  /**
   * Updating a given space override.
   *
   * type: PUT
   * values: {
   *  vsite: 2,
   *  object_id: vsite_head_link_rel_author,
   *  value: 1
   * }
   */
  public function updateSpace() {
    return $this->createUpdateVariable();
  }

  /**
   * Creating a space override.
   *
   * type: POST
   * values: {
   *  vsite: 2,
   *  object_id: vsite_head_link_rel_author,
   *  value: 1
   * }
   */
  public function createSpace() {
    return $this->createUpdateVariable();
  }

  /**
   * Create and update are the same actions. Invoke them in a single method
   * that will be invoked by the rest call type.
   */
  private function createUpdateVariable() {
    // Check group access.
    $this->checkGroupAccess();

    $controller = $this->space->controllers->{$this->objectType};
    if (!empty($this->object->object_id)) {
      $controller->set($this->object->object_id, $this->object->value);
      return array(
        'name' => $this->object->object_id,
        'value' => $controller->get($this->object->object_id),
      );
    }
    else {
      $output = array();
      foreach ($this->request as $var => $val) {
        $controller->set($var, $val);
        $output[$var] = $val;
      }
      return $output;
    }
  }

  /**
   * In order to delete a widget from the layout your REST call should be:
   * type: DELETE
   *
   * values: {
   *  vsite: 2,
   *  object_id: vsite_head_link_rel_author,
   * }
   */
  public function deleteSpace() {
    // Check group access.
    $this->checkGroupAccess();

    $controller = $this->space->controllers->{$this->objectType};
    $controller->del($this->object->object_id);
  }

  /**
   * Keep a list of the object we don't want to show.
   *
   * @return array
   *   Array of variables names.
   */
  private function whiteList() {
    $white_list = array(
      'in_fields' => array(
        'cp_admin_menu', 'cp_clear_cache', 'cp_redirect_max', 'disqus_domain',
        'disqus_nodetypes', 'enable_responsive', 'field_meta_description', 'flavors',
        'login_via_pin_by_default', 'os_appearance_flavor',
        'os_booklets_toc_position', 'os_breadcrumbs_show_breadcrumbs',
        'os_files_private_pinserver', 'os_ga_google_analytics_id',
        'os_layout_has_cleaned_up', 'os_mailchimp_vsite_api_key', 'os_menus',
        'os_profiles_default_image_file',
        'os_profiles_disable_default_image', 'os_profiles_display_type',
        'os_publications_external_link_name', 'os_publications_filter_publication_types',
        'os_publications_note_in_teaser', 'os_publications_shorten_citations',
        'os_search_solr_display_mode', 'os_shields_shield',
        'os_taxonomy_term_page_options',
        'privacy_policy', 'robotstxt', 'rowsperpage', 'scholar_front_feature_nodes',
        'scholar_front_frontpage_nid', 'scholar_front_frontpage_path',
        'scholar_front_site_title',
        'scholar_publications_filter_publication_types',
        'scholar_publication_note_in_teaser', 'scholar_reader_twitter_username',
        'show_email', 'site_favicon_fid', 'site_frontpage', 'spaces_features',
        'spaces_preset_og', 'space_menu_items', 'themedirectory', 'themesettings',
        'vsite', 'theme_default', 'os_appearance_hwpi_onepage_flavor', 'os_appearance_hwpi_classic_flavor', 'os_appearance_hwpi_college_flavor', 'os_appearance_hwpi_modern_flavor', 'os_appearance_hwpi_sterling_flavor', 'os_appearance_hwpi_themeone_bentley_flavor', 'os_appearance_hwpi_themethree_bentley_flavor', 'os_appearance_hwpi_themetwo_bentley_flavor', 'os_appearance_hwpi_vibrant_flavor',
      ),
      'wildecard_fields' => array(
        'biblio_%', 'citation_distribute_%', 'comment_%',
      ),
    );

    drupal_alter('os_restful_variable_white_list', $white_list);

    return $white_list;
  }

  /**
   * Filtering the variables we don't want to show.
   */
  protected function queryForListFilter(\SelectQuery $query) {
    parent::queryForListFilter($query);
    $whitelist = $this->whiteList();

    $db_or = db_or();
    foreach ($whitelist['wildecard_fields'] as $variable) {
      $db_or->condition('object_id', $variable, 'LIKE');
    }

    $db_or->condition('object_id', $whitelist['in_fields'], 'IN');

    $query->condition($db_or);

    if (!empty($this->path)) {
      $query->condition('object_id', $this->path);
    }
  }

  /**
   * Don't show all the variables but only the one who passed in the address.
   */
  public function getSpace() {
    if (empty($this->path)) {
      $this->throwException('You must provide the id of the variable.');
    }

    return parent::getSpace();
  }

}
