<?php

/**
 * @file
 * Contains \RestfulDataProviderDbQuery
 */

class OSRestfulCPMenu extends \RestfulBase implements \RestfulDataProviderInterface {

  /**
   * Overrides \RestfulBase::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        // If they don't pass a menu-id then display nothing.
        \RestfulInterface::GET => 'index',
        \RestfulInterface::HEAD => 'index',
        // POST
        \RestfulInterface::POST => 'create',
      ),
      // We don't know what the ID looks like, assume that everything is the ID.
      '^.*$' => array(
        \RestfulInterface::GET => 'getMenu',
        \RestfulInterface::HEAD => 'getMenu',
        \RestfulInterface::PUT => 'replace',
        \RestfulInterface::PATCH => 'update',
        \RestfulInterface::DELETE => 'remove',
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function publicFieldsInfo() {
    return array(
      'type' => array(
        'property' => 'type',
      ),
      'label' => array(
        'property' => 'label',
      ),
      'weight' => array(
        'property' => 'weight',
      ),
      'children' => array(
        'property' => 'children',
      ),
    );
  }

  /**
   * Activate user roles
   */
  public function activateRoles() {
    static $vsiteActivated = false;
    if (!$vsiteActivated) {
      if (module_exists('vsite') && $vsite = vsite_get_vsite($this->request['vsite'])) {
        spaces_set_space($vsite);
        $vsite->activate_user_roles();
        $vsite->init_overrides();
      }
      $vsiteActivated = true;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function access() {
    return true;  // access on each link is checked individually. we don't need to allow/deny the whole menu anymore.
    $account = $this->getAccount();
    return user_access('adminsiter site configuration', $account) || $this->checkGroupAccess();;
  }

  /**
   * Verify the user's request has access CRUD in the current group.
   */
  public function checkGroupAccess() {
    $account = $this->getAccount();

    $vsite = null;
    if (!empty($this->request['vsite'])) {
      $vsite = $this->request['vsite'];
    }

    if ($vsite) {
      return user_access('administer spaces', $account) || og_is_member('node', $vsite, 'user', $account);
    } else {
      return false;
    }

    return false;
  }

  /**
   * Check access for the given user against a single menu path
   */
  public function menuAccess(&$menuItem) {

    $access = false;
    if (isset($menuItem['access'])) {
      // The menu item item already have an access property. Return that one.
      $access = $menuItem['access'];
    }

    $this->activateRoles();

    if (!empty($menuItem['children'])) {
      foreach ($menuItem['children'] as &$c) {
        $access = $this->menuAccess($c) || $access;
      }
    }
    $urlParams = array();
    switch ($menuItem['type']) {
      case 'link':
        $item = menu_get_item($menuItem['href']);
        $menuItem['access'] = $item['access'];
        break;
      case 'heading':
        $menuItem['access'] = $access;
        break;
      case 'directive':
        // ?????????
        if (!isset($menuItem['access'])) {
          $menuItem['access'] = spaces_access_admin(); // what do we even do here?
        }
        break;
    }

    return $menuItem['access'];
  }

  /**
   * {@inheritdoc}
   */
  public function index() {

    $this->throwException('You must provide the id of the menu.');

    return $return;
  }

  /**
   * Throw exception.
   *
   * @param string $message
   *   Message to be thrown with exception.
   */
  protected function throwException($message) {
    throw new Exception($message);
  }

  /**
   * View a menu.
   *
   * @param string $name_string
   *  the name of the menu you would like to retrieve.
   */
  public function getMenu($name_string) {
    // Maintain order or not.
    $dmo = $this->request['dmo'];

    $output = array();
    $this->activateRoles();

    $function = "get_$name_string";
    if (method_exists($this, $function)) {
      $output = $this->$function();
      $user = $this->getAccount();

      drupal_alter('os_restful_cp_menu_'.$name_string, $output, $user);

      if (!$dmo) {
        //Set the menu order, so angular does not re-order
        $output = $this->maintainOrder($output);
      }
      $this->alterURLs($output);
    }
    else {
      $vsite = $this->request['vsite'];

      if ($vsite) {
        $controller = new spaces_controller_menus('menus', 'og', $vsite);
        $menu = $controller->get_menu_tree($name_string);
        $data = $this->normalize_menu($menu);

        return $data;
      }
    }

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function viewMultiple(array $ids) {
    $this->notImplementedCrudOperation(__FUNCTION__);
  }

  /**
   * {@inheritdoc}
   */
  public function update($id, $full_replace = FALSE) {
    $this->notImplementedCrudOperation(__FUNCTION__);
  }


  /**
   * {@inheritdoc}
   */
  public function create() {
    $this->notImplementedCrudOperation(__FUNCTION__);
  }


  /**
   * {@inheritdoc}
   */
  public function remove($id) {
    $this->notImplementedCrudOperation(__FUNCTION__);
  }

  /**
   * Returns primary menu information for a given vsite.
   */
  protected function get_primary_menu() {
    $vsite = $this->request['vsite'];

    if ($vsite) {
      $controller = new spaces_controller_menus('menus', 'og', $vsite);
      $menu = $controller->get_menu_tree('primary-menu');
      $data = $this->normalize_menu($menu);

      return $data;
    }

    $this->throwException('Vsite id mandatory');
  }

  /**
   * Returns list of custom menu for a vsite.
   */
  protected function get_menu_list() {
    $vsite = $this->request['vsite'];
    $include_default_menus = $this->request['default_menu'];
    $include_default_menus = $include_default_menus ? $include_default_menus : 0;

    if ($vsite) {
      $query = db_query("SELECT id, object_id from {spaces_overrides} where object_type = 'menus' AND id = :vsid", array(':vsid' => $vsite));
      $data = [];
      $i = 0;
      $controller = new spaces_controller_menus('menus', 'og', $vsite);

      while ($menu = $query->fetchObject()) {
        $menu_object_id = $menu->object_id;
        if (($include_default_menus == 0) && ($menu_object_id == 'primary-menu' || $menu_object_id == 'secondary-menu')) {
          continue;
        }

        $menu_tree_data = $controller->get_menu_tree($menu_object_id);

        $data[$i]['id'] = $menu->id;
        $data[$i]['object_id'] = $menu_object_id;
        $data[$i]['data'] = $this->normalize_menu($menu_tree_data);
        $i++;
      }

      return $data;
    }

    $this->throwException('Vsite id mandatory');
  }

  /**
   * Flatten array keys.
   *
   * @param array $menu
   *   Menu array to have proper keys.
   */
  protected function normalize_menu($menu = []) {
    foreach ($menu as $key => $value) {
      if (!empty($value['below'])) {
        $menu[$key]['below'] = $this->normalize_menu($value['below']);
      }
      else {
        unset($menu[$key]['below']);
      }
    }

    return array_values($menu);
  }

  /**
   * Returns secondary menu information for a given vsite.
   */
  protected function get_secondary_menu() {
    $vsite = $this->request['vsite'];

    if ($vsite) {
      $controller = new spaces_controller_menus('menus', 'og', $vsite);
      $menu = $controller->get_menu_tree('secondary-menu');
      $data = $this->normalize_menu($menu);

      return $data;
    }

    $this->throwException('Vsite id mandatory');
  }

  /**
   * Builds the admin menu as a structured array ready for drupal_render().
   *
   * @return Array of links and settings relating to the admin menu.
   */
  protected function get_admin_panel() {

    $user = $this->getAccount();
    $vsite = $this->request['vsite'];
    $vsite_object = vsite_get_vsite($vsite);
    spaces_set_space($vsite_object);

    $bundles = os_app_info_declared_node_bundle();
    $type_info = node_type_get_types();
    $variable_controller = $this->getVariableController($vsite);
    $spaces_features = $variable_controller->get('spaces_features');

    foreach ($bundles as $bundle) {
      if (!og_user_access('node', $vsite, 'create ' . $bundle . ' content')) {
        continue;
      }

      //Check that the feature is enabled, and not hidden from menus.
      $feature = os_get_app_by_bundle($bundle);
      $info = os_app_info($feature);
      if (empty($spaces_features[$feature]) || (!empty($info['hide from toolbar']) && is_array($info['hide from toolbar']) && in_array($bundle, $info['hide from toolbar']))) {
        continue;
      }

      $add_label = $type_info[$bundle]->name;
      if (strcasecmp($add_label, 'biblio') == 0) {
        $add_label = 'Publication';
      }
      $type_url_str = str_replace('_', '-', $bundle);
      $add_links["{$bundle}"] = array(
        'label' => $add_label,
        'type' => 'link',
        'href' => "node/add/{$type_url_str}",
        'alt' => $type_info[$bundle]->description,
      );

      if (os_importer_importable_content($bundle) && in_array($bundle, array('news', 'blog'))) {
        $import_links["{$bundle}"] = array(
         'label' => os_importer_importer_title($bundle),
         'type' => 'link',
         'href' => 'cp/os-importer/' . $bundle,
         'alt' => t("One time bulk import of @type content.",array('@type' => $type_info[$bundle]->name)),
        );
      }
    }

    // Files are a separate entity class, on the same level as nodes. Hard-code
    // the file links in.
    $add_links["{files}"] = array(
      'label' => "Files",
      'type' => 'link',
      'href' => 'cp/content/files',
      'alt' => t("One time bulk import of @type content.", array('@type' => "file")),
      'options' => array(
        'fragment' => 'open'
      )
    );
    $add_links["{os_private_files}"] = array(
      'label' => 'Private Files',
      'type' => 'link',
      'href' => 'cp/content/files-private',
      'alt' => t("One time bulk import of @type content.", array('@type' => "private files")),
      'options' => array(
        'fragment' => 'open'
      )
    );

    $feature_settings = array();
    if (spaces_access_admin($user, $vsite_object)) {
      foreach (array_keys(array_filter($spaces_features)) as $feature) {
        $item = menu_get_item("cp/build/features/{$feature}");
        if ($item && $item['href'] == "cp/build/features/{$feature}") {
          $feature_object = feature_load($feature);
          $feature_settings["feature_{$feature}"] = array(
            'label' => features_get_feature_title($feature_object),
            'type' => 'link',
            'href' => $item['href'],
          );
        }
      }
    }

    $private_files = !empty($spaces_features['os_files_private'])? array('files_private' => array(
        'label' => 'Private Files',
        'type' => 'link',
        'href' => 'cp/content/files-private'
    )) : array();

    $settings_forms = cp_get_setting_forms();

    $settings_links = array();
    $space_access = spaces_access_admin();
    foreach ($settings_forms as $f) {
      $group = $f['group']['#title'];
      $id = $f['group']['#id'];

      $access = !isset($f['form']['#access']) ? $space_access : $f['form']['#access'];

      if ($access) {
        $settings_links[$id] = array(
          'label' => $group,
          'type' => 'directive',
          'directive' => array(
            'ap-settings-form',
            'form' => $f['group']['#id'],
          ),
          'parent' => !empty($f['group']['#menu_parent']) ? $f['group']['#menu_parent'] : 'advanced',
          'access' => true,
        );
      }
    }

    // Order alphabetically.
    $labelcmp = function ($a, $b) {
        return strnatcmp($a['label'], $b['label']);
    };
    uasort($add_links, $labelcmp);
    uasort($feature_settings, $labelcmp);

    $structure = array(
      'content' => array(
        'label' => 'Site Content',
        'type' => 'heading',
        'default_state' => 'collapsed',
        'children' => array(
          'browse' => array(
            'label' => 'Browse',
            'type' => 'heading',
            'default_state' => 'collapsed',
            'children' => array(
              'content' => array(
                'label' => 'Content',
                'type' => 'link',
                'href' => 'cp/content'
              ),
              'files' => array(
                'label' => 'Files',
                'type' => 'link',
                'href' => 'cp/content/files'
              )) + $private_files
//              @tbd v2
//              'widgets' => array(
//                'label' => 'Widgets',
//                'type' => 'link',
//                'href' => '/cp/content'
//              ),
          ),
          'add' => array(
            'label' => 'Add',
            'type' => 'heading',
            'default_state' => 'collapsed',
            'children' => $add_links,
          ),
          'import' => array(
            'label' => 'Import',
            'type' => 'heading',
            'default_state' => 'collapsed',
            'children' => $import_links,
          ),
        ),
      ),
      'menus' => array(
        'label' => 'Menus',
        'type' => 'link',
        'href' => 'cp/build/menu',
      ),
      'appearance' => array(
        'label' => 'Appearance',
        'type' => 'heading',
        'default_state' => 'collapsed',
        'children' => array(
            'layout' => array(
                'label' => 'Layout',
                'type' => 'link',
                'href' => 'cp/build/layout'
            ),
            'themes' => array(
              'label' => 'Themes',
              'type' => 'link',
              'href' => 'cp/appearance'
            ),
//            'theme_settings' => array(
//              'label' => 'Theme Settings',
//              'type' => 'link',
//              'href' => 'dev/null'
//            ),
          ),
      ),
      'tagging' => array(
        'label' => 'Taxonomy',
        'type' => 'link',
        'href' => 'cp/build/taxonomy',
      ),
      'settings' => array(
        'label' => 'Settings',
        'type' => 'heading',
        'default_state' => 'collapsed',
        'children' => array(
          'app' => array(
            'label' => 'Enable / Disable Apps',
            'type' => 'directive',
            'directive' => array(
              'app-form-modal'
            ),
          ),
          'app_settings' => array(
            'label' => 'App Settings',
            'type' => 'heading',
            'default_state' => 'collapsed',
            'children' => $feature_settings,
          ),
          'advanced' => array(
            'label' => 'Global Settings',
            'type' => 'heading',
            'default_state' => 'collapsed',
          )
        ),
      ),
      'users_roles' => array(
        'label' => 'Members & Roles',
        'type' => 'link',
        'href' => 'cp/users',
      ),
      'help' => array(
        'label' => 'Help',
        'type' => 'heading',
        'default_state' => 'collapsed',
        'children' => array(),
        // Grant access and by pass access checking.
        'access' => TRUE,
      ),
    );

    foreach ($settings_links as $k => $sl) {
      $elem = &$this->findMenuElement($structure, (array)$sl['parent']);
      unset($sl['parent']);
      $elem['children'][$k] = $sl;
      uasort($elem['children'], $labelcmp);
    }

    // Should we show this user the admin links?
    if (user_access('access toolbar',$user)) {
      $admin_menu = menu_tree_all_data('management', NULL, 2);
      $admin_menu = current($admin_menu);
      $admin_links = array();

      foreach ($admin_menu['below'] as $mi) {
        $link = $mi['link'];
        if ($link['hidden'] != 0) continue;
        $key = str_replace(" ", "_", strtolower($link['title']));
        $admin_links[$key] = array(
          'label' => $link['title'],
          'type' => 'link',
          'href' => "{$link['href']}",
        );
      }

      $structure['admin'] = array(
        'label' => 'Admin',
        'type' => 'heading',
        'default_state' => 'collapsed',
        'children' => $admin_links,

      );
    }

    foreach ($structure as &$link) {
      $this->menuAccess($link);
    }

    return $structure;
  }

  /**
   * @param $menu
   *   The menu to search
   * @param $args
   *   An array of keys in the menu array that will lead to the target element
   *   Ex.
   *      findMenuParent('browse', 'content')
   *      findMenuParent('appearance')
   */
  private function &findMenuElement(&$menu, $args) {
    $element = $args[0];
    if (isset($menu[$element])) {
      if (count($args) == 1) {
        return $menu[$element];
      }
      else if (empty($element['children'])) {
        return false;
      }
      else {
        array_shift($args);
        $target = &$this->findMenuElement($menu[$element]['children'], $args);
        return $target;
      }
    }
    else {
      foreach ($menu as &$m) {
        if (empty($m['children'])) {
          continue;
        }
        else {
          if ($target = &$this->findMenuElement($m['children'], $args)) {
            return $target;
          }
        }
      }
    }
  }

  protected function getVariableController($vsite) {

    $controller = FALSE;
    ctools_include('plugins');

    $plugin = ctools_get_plugins('spaces', 'plugins', 'spaces_controller_variable');
    if ($plugin && $class = ctools_plugin_get_class($plugin, 'handler')) {
      $controller = new $class('variable', 'og', $vsite);
    }

    return $controller;
  }

  /**
   * Change the array keys so that angular does not re-order them. It
   * automatically re-orders keys so that they are alphabetical.
   *
   * @param $menu
   */
  protected function maintainOrder($menu) {
    $final = array();
    $i = 0;
    foreach ($menu as $key => $value) {
      if (is_array($value)) {
        $value = $this->maintainOrder($value);
      }

      if(is_array($value) && !empty($value['type'])) {
    	  $final["{$i}_{$key}"] = $value;
    	  $i++;
      } else {
        $final["{$key}"] = $value;
      }
    }

    return $final;
  }

  /**
   * Alter the URL's so that they are vsite specific.
   * @param $menu
   */
  protected function alterURLs(&$menu) {

    if (!empty($this->request['vsite'])) {
      $vsite = $this->request['vsite'];
      $vsite_object = vsite_get_vsite($vsite);

      foreach ($menu as $key => $value) {
        if (!empty($value['children'])) {
          $this->alterURLs($menu[$key]['children']);
        }

        if (!empty($value['href']) && $value['href'] != '#') {
          if ($vsite_object) {
            $menu[$key]['href'] = $vsite_object->get_absolute_url($value['href'], !empty($value['options']) ? $value['options'] : array());
          } else {
            $menu[$key]['href'] = url($value['href'], !empty($value['options']) ? $value['options'] : array());
          }
        }
      }
    }
  }
}
