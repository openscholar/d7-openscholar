<?php

class FeatureHelp {

  /**
   * @var array
   * Mapping for all the pages.
   */
  static $page_mapping = array(
    'News' => 'news_news',
    'Blog' => 'blog_blog',
    'Link' => 'links_links',
    'Reader' => 'reader_reader',
    'Calendar' => 'events_events',
    'Classes' => 'classes_classes',
    'People' => 'profiles_profiles',
    'Data' => 'dataverse_dataverse',
    'Galleries' => 'gallery_gallery',
    'FAQ' => 'faq_faq',
    'Software' => 'software_software',
    'Documents' => 'booklets_booklets',
    'Publications' => 'publications_publications',
    'Presentations' => 'presentations_presentations',
  );

  /**
   * @var array
   * Mapping widgets we want to tests. The array build in the format of:
   *  - The key hold the mapped name for the widget.
   *  - If this is a block, the value will be MODULE-BLOCK_DELTA-BLOCK.
   */
  static $boxes_mapping = array(
    'Filter by term' => 'os_taxonomy_fbt',
    'Simple view list' => 'os_sv_list_box',
    'Files view list' => 'os_sv_list_file',
    'Search' => 'os_search_db-site-search',
    'Active book TOC' => 'os_boxes_booktoc',
    'RSS feed' => 'os_boxes_rss',
    'Image gallery' => 'os_boxes_slideshow',
    'Faceted taxonomy' => 'os_boxes_facetapi_vocabulary',
    'List of posts' => 'os_sv_list_box',
    'List of publications' => 'os_sv_list_box',
    'Upcoming events' => 'os_sv_list_box',
    'All Posts' => 'os_sv_list_box',
    'Cache time test' => 'os_boxes_cache_test',
    'Solr search' => 'os_search_solr_search_box',
  );

  /**
   * Get the entity ID.
   *
   * @param $entity_type
   *   'node', 'user', etc.
   * @param $title
   *   Title of the entity.
   * @param $bundle
   *   Optional; The bundle of the entity.
   *
   * @return mixed
   *   The entity ID (in case of $return).
   */
  static public function getEntityID($entity_type, $title, $bundle = NULL) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type);
    if ($entity_type == 'node') {
      $query->propertyCondition('title', $title);
    }
    else if ($entity_type == 'file') {
      $query->propertyCondition('filename', $title);
    }

    if ($bundle) {
      $query->entityCondition('bundle', $bundle);
    }

    $result = $query->execute();

    // Currently only support file and node.
    if ($entity_type == 'node') {
      $identifier = 'nid';
    }
    else if ($entity_type == 'file') {
      $identifier = 'fid';
    }

    if (empty($result[$entity_type])) {
      return NULL;
    }

    return reset($result[$entity_type])->{$identifier};
  }

  /**
   * Get the node ID.
   *
   * @param $title
   *   The node title.
   *
   * @return integer
   *   The node ID.
   */
  static public function getNodeId($title) {
    return static::getEntityID('node', $title);
  }

  /**
   * Delete a node with a specific title.
   *
   * @param $title
   *   The title of the node.
   */
  static public function deleteNode($title) {
    node_delete_multiple(array(static::getNodeId($title)));
  }

  /**
   * Enable the harvard courses.
   */
  static public function define_harvard_courses() {
    // Enable Harvard courses module.
    module_enable(array('harvard_courses'));

    return t('Harvard courses enabled successfully.');
  }

  /**
   * Change the spaces overrides domain easily.
   *
   *  @param $type
   *    The space type.
   *  @param $id
   *    The space ID.
   *  @param $domain
   *    The domain address.
   */
  static public function spacesOverrides($type, $id, $domain) {
    $query = db_select('spaces_overrides', 'space')
      ->fields('space', array('value'))
      ->condition('space.type', $type)
      ->condition('space.id', $id)
      ->execute()
      ->fetchAssoc();

    if (empty($query)) {
      // Check first if there is no PURL for the site.
      db_insert('spaces_overrides')
        ->fields(array(
          'value' => serialize($domain),
          'type' => $type,
          'id' => $id,
          'object_type' => 'variable',
          'object_id' => 'vsite_domain_name',
        ))
        ->execute();
    }
    else {
      if ($query['value'] == serialize($domain)) {
        // This is the same value, return early.
        return;
      }
      db_update('spaces_overrides')
        ->fields(array(
          'value' => serialize($domain),
        ))
        ->condition('type', $type)
        ->condition('id', $id)
        ->execute();
    }
  }

  /**
   * Import the courses not via batch API for the tests. The batch API not work
   * in CLI mode, there for we will use the import functions directly.
   */
  static public function importCourses() {
    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('type', 'harvard_api_importer')
      ->propertyCondition('title', 'Department & school importer')
      ->execute();

    if (!empty($result['node'])) {
      foreach (array_keys($result['node']) as $nid) {
        feeds_source('course', $nid)->import();
      }
    }
  }

  /**
   * Create a dummy feed importer and import its items.
   *
   * @param $url
   *  RSS url from which to import the items.
   * @param $vsite_id
   *  VSite ID to which to associate the imported feed.
   * @param $type
   *  The node importer type.
   */
  static public function importFeedItems($url, $vsite_id, $type = 'news') {
    $node_type = $type == 'news' ? 'feed_importer' : 'blog_import';
    $node = node_load($vsite_id);
    $entity = entity_create('node', array(
      'title' => $node->title . ' ' . $type . ' importer',
      'language' => LANGUAGE_NONE,
      'type' => $node_type,
      'uid' => 1,
    ));

    $wrapper = entity_metadata_wrapper('node', $entity);
    $wrapper->field_rss_url->set($url);
    $wrapper->{OG_AUDIENCE_FIELD}->set(array($vsite_id));
    $wrapper->save();

    feeds_source('os_reader', $wrapper->getIdentifier())->import();
  }

  /**
   * Attach a block to region on OS.
   *
   *  @param $nid
   *    The node id of the site.
   *  @param $box
   *    The human name of the widget.
   *  @param $page
   *    The human name for the page for attaching the widget to.
   *  @param $region
   *    The machine name of the region, optional.
   *
   *  @return
   *    A CSV string contain the widget plugin, the delta of the box/block, and
   *    the context page mapping.
   *    If the function called from CLI the function will print the string.
   */
  static public function setBoxInRegion($nid, $box, $page, $region = 'sidebar_second', $delta_suffix = '') {
    $box_name = $box;
    ctools_include('layout', 'os');
    $contexts = array(
      self::$page_mapping[$page],
      'os_public',
    );

    $blocks = os_layout_get_multiple($contexts, FALSE, TRUE);

    $options = array(
      'delta' => time(),
    );

    // Changing the region differently for box and block.
    if (empty($blocks[self::$boxes_mapping[$box]])) {
      // Define the box.
      $delta = 'box-' . strtolower(str_replace(" ", "-", $box));
      if ($delta_suffix) {
        $delta .= '-' . $delta_suffix;
      }
      $options = array(
        'delta' => $delta,
        'title' => $box,
        'description' => $box,
      );

      // Create the box.
      if (!$box = boxes_box::factory(self::$boxes_mapping[$box], $options)) {
        throw new Exception(sprintf('The box %s failed to saved', $box_name));
      }
      $box->save();

      $blocks['boxes-' . $box->delta]['region'] = $region;

      // Initialize the module ad the delta.
      if (!array_key_exists($blocks['boxes-' . $box->delta]['region'], array('module', 'delta'))) {
        $blocks['boxes-' . $box->delta]['delta'] = $options['delta'];
        $blocks['boxes-' . $box->delta]['module'] = 'boxes';
      }

      $stringToPrint = $box->plugin_key . ',' . $options['delta'] . ',' . self::$page_mapping[$page];
    }
    else {
      $blocks[self::$boxes_mapping[$box]]['region'] = $region;
      $stringToPrint = self::$boxes_mapping[$box] . ',' . $options['delta'] . ',' . self::$page_mapping[$page];
    }

    // Save the widget in the region.
    $vsite = spaces_load('og', $nid);
    $vsite->controllers->context->set($contexts[0] . ":reaction:block", array(
      'blocks' => $blocks,
    ));

    // Data that relate to the box we added and used in the end of the scenario.
    return $stringToPrint;
  }

  /**
   * Hide the box. If the box is not exported, the function will delete the box.
   *
   *  @param $nid
   *    The node id of the site.
   *  @param $plugin
   *    The box plugin name from the mapped boxes.
   *  @param $delta
   *    The delta of the box.
   *  @param $page
   *    The machine name page from the mapped pages.
   *
   *  @see static::SetBoxInRegion().
   */
  static public function hideBox($nid, $plugin, $delta, $page) {
    // Hide the box.
    ctools_include('layout', 'os');
    $contexts = array(
      $page,
      'os_public',
    );
    $blocks = os_layout_get_multiple($contexts, FALSE, TRUE);

    if (empty($blocks[$plugin])) {
      $blocks['boxes-' . $delta]['region'] = FALSE;
      // Delete the box.
      if ($box = boxes_box::factory($plugin, array())) {
        $box->delete();
      }
    }
    else {
      $blocks[$plugin]['region'] = FALSE;
    }

    $vsite = spaces_load('og', $nid);
    $vsite->controllers->context->set($page . ":reaction:block", array(
      'blocks' => $blocks,
    ));
  }

  /**
   * Tag node to term.
   *
   *  @param $title
   *    The title of the node.
   *  @param $name
   *    The name of the term.
   *  @param $type
   *    The type of the node. Optional, default is class.
   */
  static public function assignNodeToTerm($title, $name, $type = 'class') {
    $entities = entity_load('node', NULL, array('title' => $title, 'type' => $type));

    if (!reset($entities)) {
      return;
    }

    $nid = reset($entities)->nid;

    $names = explode(",", $name);
    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('name', $names, is_array($names) ? 'IN' : '=')
      ->execute();

    $tid = reset($result['taxonomy_term']);

    $wrapper = entity_metadata_wrapper('node', $nid);
    $wrapper->{OG_VOCAB_FIELD}[] = $tid;
    $wrapper->save();
  }

  /**
   * Remove term tag from node.
   *
   *  @param $title
   *    The title of the node.
   *  @param $name
   *    The name of the term.
   *  @param $type
   *    The type of the node. Optional, default is class.
   */
  static public function unassignNodeFromTerm($title, $name, $type = 'class') {
    $entities = entity_load('node', NULL, array('title' => $title, 'type' => $type));

    if (!$entities) {
      return;
    }

    $nid = reset($entities)->nid;

    $wrapper = entity_metadata_wrapper('node', $nid);

    $terms = $wrapper->{OG_VOCAB_FIELD}->value();
    foreach ($terms as $key => $term) {
      if ($term->name == $name) {
        unset($terms[$key]);
      }
    }

    $wrapper->{OG_VOCAB_FIELD}->set($terms);
    $wrapper->save();
  }

  /**
   * Set a value for variable.
   */
  static public function variableSet($name, $value) {
    $vsite = spaces_preset_load('os_scholar', 'og');
    $vsite->value['variable'][$name] = $value;
    spaces_preset_save($vsite);

    variable_set($name, $value);
  }

  /**
   * Get vsite id for purl path
   */
  static public function idFromPath($path) {
    $q = db_select('purl', 'p')
      ->fields('p', array('id'))
      ->condition('value', $path)
      ->condition('provider', 'spaces_og')
      ->execute();

    return $q->fetchField();
  }

  /**
   * Set variable for vsite only
   * @param $vsite - the purl id of the vsite
   */
  static public function variableSetSpace($name, $value, $vsite) {
    $id = self::idFromPath($vsite);

    if ($vs = vsite_get_vsite($id)) {
      $vs->controllers->variable->set($name, $value);
      $conf[$name] = $value;
    }
    else {
      throw new Exception("No vsite $vsite found.");
    }
  }

  /**
   * Set term under a term.
   */
  static public function setTermUnderTerm($child, $parent) {
    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('name', $child)
      ->execute();

    $child_tid = reset($result['taxonomy_term'])->tid;

    $term = taxonomy_term_load($child_tid);

    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('name', $parent)
      ->execute();

    $parent_tid = reset($result['taxonomy_term'])->tid;

    $term->parent = array($parent_tid);
    taxonomy_term_save($term);
  }

  /**
   * Get the term ID.
   *
   * @param $term
   *   The term name.
   */
  static public function getTermId($term) {
    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('name', $term)
      ->execute();

    return reset($result['taxonomy_term'])->tid;
  }

  static public function getTermVsitePurl($tid) {
    // Get vid.
    $term = taxonomy_term_load($tid);
    $vid = $term->vid;

    // Get og_vocab relation.
    $relation = og_vocab_relation_get($vid);

    if (empty($relation)) {
      // Vocabulary is not related to a group.
      return '';
    }

    // Get vsite.
    $vsite = node_load($relation->gid);

    return $vsite->purl;
  }

  static public function getFileVsitePurl($fid) {
    // Get vid.
    $file = file_load($fid);
    $vid = $file->vid;

    // Get og_vocab relation.
    $relation = og_vocab_relation_get($vid);

    if (empty($relation)) {
      // Vocabulary is not related to a group.
      return '';
    }

    // Get vsite.
    return node_load($relation->gid)->purl;
  }

  /**
   * Get vsite PURL.
   *
   * @param $nid
   *   The vsite ID.
   * @return mixed
   *   The PURL.
   */
  static public function getVsitePurl($nid) {
    return node_load($nid)->purl;
  }

  /**
   * Get the node ID by title in a given VSite.
   */
  static public function getNodeIdInVsite($title, $vsite) {
    $gid = self::GetNodeId($vsite, TRUE);
    $query = new \entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('title', $title)
      ->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $gid)
      ->execute();

    return reset($result['node'])->nid;
  }

  /**
   * Get the role of a user in a group.
   *
   *  @return
   *    0 - if user is not a member of the group.
   *    1 - user has a role which is not the role given.
   *    2 - user has the role given
   */
  static public function checkUserRoleInGroup($name, $role, $group) {
    drupal_static_reset();
    $gid = self::idFromPath($group);
    $user = user_load_by_name($name);

    if (!og_is_member('node', $gid, 'user', $user)) {
      return 1;
    }

    $user_roles = og_get_user_roles('node', $gid, $user->uid);

    return !in_array($role, $user_roles) ? 1 : 2;
  }

  /**
   * Get a user id by name.
   *
   * @param $name
   *    The user's name.
   */
  static public function GetUserByName($name) {
    drupal_static_reset();
    return user_load_by_name($name)->uid;
  }

  /**
   * Get a role id.
   *
   * @param $name
   * @param $gid
   */
  static public function GetRoleByName($name, $gid) {
    drupal_static_reset();
    $roles = og_roles('node', NULL, $gid);
    return array_search($name, $roles);
  }

  /**
   * Make a node sticky.
   */
  static public function MakeNodeSticky($nid) {
    db_update('node')
      ->fields(array(
        'sticky' => 1,
      ))
      ->condition('nid', $nid)
      ->execute();
  }

  /**
   * Get the node alias.
   */
  static public function GetNodeAlias($nid) {
    $result = db_select('url_alias')
      ->fields('url_alias', array('alias'))
      ->condition('source', 'node/' . $nid)
      ->execute()
      ->fetchAssoc();

    return $result['alias'];
  }

  /**
   * Get the entity vsite purl.
   */
  static public function GetEntityVsitePurl($entity_type, $entity_id) {
    $wrapper = entity_metadata_wrapper($entity_type, $entity_id);

    if (empty($wrapper->{OG_AUDIENCE_FIELD})) {
      // Not group content.
      return '';
    }
    $vsite = $wrapper->{OG_AUDIENCE_FIELD}->get(0)->value();

    return $vsite->purl;
  }

  /**
   * Get the node vsite purl.
   */
  static public function GetNodeVsitePurl($nid) {
    return static::GetEntityVsitePurl('node', $nid);
  }

  /**
   * Get the term alias.
   */
  static public function GetTermAlias($tid) {
    $result = db_select('url_alias')
      ->fields('url_alias', array('alias'))
      ->condition('source', 'taxonomy/term/' . $tid)
      ->execute()
      ->fetchAssoc();

    return $result['alias'];
  }

  /**
   * Create a term in a given vocabulary.
   *
   * @param $term_name
   *   The name for the new term.
   *
   * @param $vocab_name
   *   The name of the vocabulary.
   *
   * @throws Exception
   *   When vocab not found or not mapped correctly.
   */
  static public function CreateTerm($term_name, $vocab_name) {
    // A mapping for existing vocabularies.
    $vocabs = array(
      'authors' => 'authors_personal1',
      'biology' => 'biology_personal1',
      'math' => 'math_personal1',
      'science' => 'science_personal1',
    );

    $params = array(
      '@vocab-name' => $vocab_name,
    );

    if (!array_key_exists($vocab_name, $vocabs)) {
      throw new Exception(t('The vocabulary "@vocab-name" is not mapped to any existing vocabularies.', $params));
    }

    // Load the vocabulary.
    $machine_name = $vocabs[$vocab_name];
    if (!$vocab = taxonomy_vocabulary_machine_name_load($machine_name)) {
      throw new Exception(t('No vocabulary with the name "@vocab-name" was found.', $params));
    }

    // Create and save the new term.
    $term = new stdClass();
    $term->name = $term_name;
    $term->vid = $vocab->vid;
    taxonomy_term_save($term);
  }

  /**
   * Delete a term in a given vocabulary.
   *
   * @param $term_name
   *  The name of the term.
   */
  static public function DeleteTerm($term_name) {
    $taxonomies = taxonomy_get_term_by_name($term_name);
    $term = reset($taxonomies);
    taxonomy_term_delete($term->tid);
  }

  /**
   * Adding a subtheme to a site.
   *
   * @param $subtheme
   *  The name of the subtheme(a theme flavor) which located in the behat folder.
   * @param $vsite
   *  The name of the vsite.
   */
  static public function AddSubtheme($subtheme, $vsite) {
    $path = 'profiles/openscholar/behat/' . $subtheme;

    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('title', $vsite)
      ->range(0, 1)
      ->execute();

    if (empty($result['node'])) {
      return;
    }

    $nid = array_keys($result['node']);
    $vsite = vsite_get_vsite(reset($nid));

    $flavors = $vsite->controllers->variable->get('flavors');

    $flavors[$subtheme] = array(
      'path' => $path,
      'name' => $subtheme,
    );

    $vsite->controllers->variable->set('flavors', $flavors);
  }

  /**
   * Define a subtheme for a vsite.
   *
   * @param $theme
   *  The name of the theme which the new subtheme is her flavor.
   * @param $subtheme
   *  The name of the subtheme which located in the behat folder.
   * @param $vsite
   *  The name of the vsite.
   */
  static public function DefineSubtheme($theme, $subtheme, $vsite) {
    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('title', $vsite)
      ->range(0, 1)
      ->execute();

    if (empty($result['node'])) {
      return;
    }

    $nid = array_keys($result['node']);
    $vsite = vsite_get_vsite(reset($nid));
    $flavor_key = 'os_appearance_cleanblue_flavor';

    $vsite->controllers->variable->set('theme_default', $theme);
    $vsite->controllers->variable->set('os_appearance_cleanblue_flavor', $subtheme);
  }

  /**
   * Importing nodes from a demo CSV.
   *
   * @pram $import
   *  The file name(without .csv)
   */
  static public function ImportCsv($type) {
    global $base_url;

    $importers = array(
      'blog' => 'os_blog_csv',
      'news' => 'os_news',
      'event' => 'os_ical',
      'page' => 'os_pages',
      'class' => 'os_classes',
      'faq' => 'os_faq',
      'presentation' => 'os_presentation',
      'software_project' => 'os_software',
      'person' => 'os_people',
      'link' => 'os_links',
      'media_gallery' => 'os_gallery',
    );

    // Specify a specific encoding of importers files. This meant for testing the
    // import of files in various encodings.
    $encodes = array(
      'blog' => 'WINDOWS-1255',
      'news' => 'WINDOWS-1254',
    );

    $ending = $type == 'event' ? 'ics' : 'csv';
    $url = $base_url . '/' . drupal_get_path('module', 'os_migrate_demo') . '/includes/import_csv/' . $type . '.' . $ending;
    // todo: Use system_retrieve_file().
    $file = system_retrieve_file($url, 'public://', TRUE);

    $source = feeds_source($importers[$type]);
    $fetcher_config = $source->getConfigFor($source->importer->fetcher);
    $fetcher_config['source'] = $file->uri;
    $fetcher_config['file'] = $file;

    // Add encoding for specific files.
    if (isset($encodes[$type])) {
      $fetcher_config['encode'] = $encodes[$type];
    }
    $source->setConfigFor($source->importer->fetcher, $fetcher_config);
    $source->save();

    feeds_source($importers[$type], 0)->import();
  }

  /**
   * @param $bundle
   *  The content type.
   * @param $vocabulary
   *  The vocabulary name.
   */
  static public function BindContentToVocab($bundle, $vocabulary) {
    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_vocabulary')
      ->propertyCondition('name', $vocabulary)
      ->execute();

    if (empty($result['taxonomy_vocabulary'])) {
      return;
    }

    $vid = reset($result['taxonomy_vocabulary'])->vid;
    og_vocab_create_og_vocab($vid, 'node', $bundle)->save();
  }

  /**
   * Remove the domain we added to a vsite.
   *
   * @param $vsite
   *  The vsite name.
   */
  static public function remove_vsite_domain($vsite) {
    $id = static::GetNodeId($vsite, TRUE);

    db_delete('spaces_overrides')
      ->condition('id', $id)
      ->condition('object_id', 'vsite_domain_name')
      ->execute();
  }

  /**
   * Set the display of field_event_registration to be Registration Form.
   */
  static public function EventRegistrationForm() {
    $instance = field_info_instance('node', 'field_event_registration', 'event');
    $instance['display']['default']['type'] = 'registration_form';
    field_update_instance($instance);
  }

  /**
   * Set the display of field_event_registration to be Registration Link.
   */
  static public function EventRegistrationLink() {
    $instance = field_info_instance('node', 'field_event_registration', 'event');
    $instance['display']['default']['type'] = 'registration_link';
    $instance['display']['default']['settings']['label'] = 'Sign up for this event';
    field_update_instance($instance);
  }

  /**
   * Set the variable $name to $value for the site $vsite.
   */
  static public function VsiteSetVariable($vsite, $name, $value) {
    $query = new entityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('title', $vsite)
      ->range(0, 1)
      ->execute();

    if (empty($result['node'])) {
      return;
    }

    $nid = array_keys($result['node']);
    $vsite = vsite_get_vsite(reset($nid));
    $vsite->controllers->variable->set($name, $value);
  }

  /**
   * Get the variable $name for the site $vsite
   */
  static public function VsiteGetVariable($vsite, $name) {
    $nid = db_select('purl', 'p')
      ->fields('p', array('id'))
      ->condition('value', $vsite)
      ->condition('provider', 'spaces_og')
      ->execute()
      ->fetchField();

    $vsite = vsite_get_vsite($nid);
    $vsite->controllers->variable->init_overrides();
    return $vsite->controllers->variable->get($name);
  }

  static public function SetReadOnly($value) {
    variable_set('os_readonly_mode', $value);
  }

  static public function AddToWhiteList($domain) {
    $domains = variable_get('media_embed_whitelist', array());
    $domains[] = $domain;

    variable_set('media_embed_whitelist', array_values($domains));
  }

  /**
   * Return the number of times the feed item exists.
   *
   * @param $title
   *  The title of the feed item.
   * @param $vsite
   *  The vsite name.
   */
  static public function CountNodeInstances($title, $vsite) {
    $nid = static::GetNodeId($vsite);
    $query = new EntityFieldQuery();

    $results = $query
      ->entityCondition('entity_type', 'os_feed_item')
      ->propertyCondition('title', $title)
      ->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $nid)
      ->count()
      ->execute();

    return $results;
  }

  /**
   * Get the uid of the owner of a group.
   *
   * @param $group
   *   The group for which to get the owner's uid.
   */
  static public function GetVsiteOwnerUid($group) {
    drupal_static_reset();
    $nid = static::GetNodeId($group);
    $space = spaces_load('og', $nid);
    return $space->group->uid;
  }

  /**
   * Get the nid of the child site of a profile.
   *
   *  Prints 0 if user the profile has no child site.
   *  Otherwise, print the node id of the profile's child site.
   *
   * @param $profile_title
   *  The profile title for which to get the child site's id.
   */
  static public function GetChildSiteNid($profile_title) {
    $nid = static::GetEntityId('node', $profile_title, 'person');
    $wrapper = entity_metadata_wrapper('node', $nid);

    if (!$wrapper->__isset('field_child_site')) {
      return 0;
    }

    if (!$child_site_from_profile = $wrapper->field_child_site->value(array('identifier' => TRUE))) {
      return 0;
    }

    return $child_site_from_profile;
  }

  /**
   * Remove the courses for s given values.
   */
  static public function RemoveCourses() {
    static::__RemoveCourses(FALSE);
  }

  /**
   * Add the courses for s given values.
   */
  static public function AddCourses() {
    static::__RemoveCourses();
  }

  /**
   * Remove/add the courses for s given values.
   *
   * @param $group
   *   Determine if we need to group or un group the courses.
   */
  static public function __RemoveCourses($group = TRUE) {
    $fields = array(
      'field_faculty',
      'field_department_id',
    );

    $search_values = array(
      'field_faculty' => "Harvard Graduate School of Design",
      'field_department_id' => 'Architecture',
    );

    // Un-grouping removed courses from the group.
    $courses = harvard_courses_related_nodes($fields, $search_values);
    if (!empty($courses)) {
      foreach ($courses as $course_id) {
        if ($group) {
          og_group('node', 2, array('entity_type' => 'node', 'entity' => $course_id));
        }
        else {
          og_ungroup('node', 2, 'node', $course_id);
        }
      }
    }
  }

  /**
   * Remove the domain we added to a vsite.
   *
   * @param $vsite
   *  The vsite name.
   */
  static public function RemoveVsiteDomain($vsite) {
    $id = static::getNodeId($vsite);

    db_delete('spaces_overrides')
      ->condition('id', $id)
      ->condition('object_id', 'vsite_domain_name')
      ->execute();
  }

  /**
   * Get file ID's by their name.
   *
   * @param array $files
   *   List of file names.
   */
  static public function getFilesIDs($files) {
    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'file')
      ->propertyCondition('filename', $files, 'IN')
      ->execute();

    if (empty($result['file'])) {
      return;
    }

    return array_keys($result['file']);
  }

  /**
   * Return list of watchdog messages.
   */
  public static function DisplayWatchdogs() {
    $query = db_select('watchdog', 'w');
    $result = $query
      ->fields('w', array('wid', 'uid', 'severity', 'type', 'timestamp', 'message', 'variables', 'link'))
      ->orderBy('w.timestamp', 'DESC')
      ->range(0, 200)
      ->execute();

    $messages = [];
    foreach ($result as $dblog) {
      $params = unserialize($dblog->variables);

      if (!is_array($params)) {
        $params = [];
      }

      $string = format_string($dblog->message, $params);
      $messages[] = strip_tags($string);
    }

    return $messages;
  }

}
