<?php

class GroupNodeRestfulBase extends OsNodeRestfulBase {

  public static function controllersInfo() {
    return parent::controllersInfo() + array(
      'check' => array(
        \RestfulInterface::POST => 'check',
      ),
      'themes' => array(
        \RestfulInterface::GET => 'themes',
      )
    );
  }

  /**
   * Validates the fields passed in via POST against a variety of tests
   * Supports the following fields:
   *    url
   */
  protected function check() {
    $output = array();
    foreach ($this->request as $k => $val) {
      switch ($k) {
        case 'url':
          if (!preg_match('|^[a-zA-Z0-9-]*$|', $val)) {
            $output[$k] = array(
              'pass' => false,
              'errors' => array(
                'urlPattern' => t('Url can only contain lowercase alphanumberics and hyphen.'),
              )
            );
          }
          else {
            $q = db_select('purl', 'p')
              ->condition('value', $val)
              ->condition('provider', 'spaces_og')
              ->countQuery()
              ->execute()
              ->fetchField();

            if ($q > 0) {
              $output[$k] = array(
                'pass' => false,
                'errors' => array(
                  'urlTaken' => t('Url is already in use. Do you have a site already?'),
                )
              );
            }
          }
          break;
      }
      if (!isset($output[$k])) {
        $output[$k] = array(
          'pass' => true,
        );
      }
    }

    return $output;
  }

  /**
   * Returns a list of available themes a site administrator may choose
   *
   */
  protected function themes() {
    ctools_include('themes', 'os');
    $flavors = os_theme_get_flavors();
    $approved_themes = array(
      'Ivy Accents',
      'Lemon Sand',
      'Blue Sky',
      'Teal Accents',
      'Slate',
      'White',
      'Monochrome',
      'Sterling'
    );
    $output = array();
    foreach ($flavors as $k => $t) {
      if (in_array($t['name'], $approved_themes)) {
        $output[$k] = array(
          'id' => $k,
          'name' => $t['name'],
          'thumb' => url($t['path'].'/'.$t['screenshot'], array('absolute' => true, 'alias' => true)),
        );
      }
    }

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    unset($public_fields['body']);

    $public_fields['users'] = array(
      'property' => 'nid',
      'process_callbacks' => array(
        array($this, 'getGroupUsers'),
      ),
    );

    $public_fields['type'] = array(
      'property' => 'bundle',
      'callback' => array($this, 'getBundleForView'),
    );

    $public_fields['owner'] = array(
      'property' => 'author',
      'resource' => array(
        'user' => array(
          'name' => 'users',
          'full_view' => true,
        )
      ),
    );

    $public_fields['preset'] = array(
      'property' => 'preset',
    );

    $public_fields['site_description'] = array(
      'property' => 'field_site_description',
    );

    $public_fields['site_address'] = array(
      'property' => 'field_site_address',
    );

    $public_fields['site_logo'] = array(
      'property' => 'field_site_logo',
    );

    $public_fields['title'] = array(
      'property' => 'title',
    );

    $public_fields['purl'] = array(
      'property' => 'domain',
    );

    $public_fields['url'] = array(
      'callback' => array($this, 'getExternalUrl'),
    );

    $public_fields['theme'] = array(
      'callback' => array($this, 'getTheme'),
    );

    if (module_exists('vsite_access')) {
      $public_fields['privacy'] = array (
        'property' => VSITE_ACCESS_FIELD
      );
    }

    $public_fields['parent'] = array(
      'property' => 'field_group_parent',
      'callback' => Closure::bind(function ($wrapper) { return $this->optionalPropertyCallback ('field_group_parent', $wrapper); }, $this)
    );

    return $public_fields;
  }

  /**
   * Overrides RestfulEntityBase::getQueryForList().
   *
   * Expose only published nodes.
   */
  public function getQueryForList() {
    $query = parent::getQueryForList();
    $vsite_id = $_GET['vsite'];
    if ($vsite_id) {
      $query->propertyCondition('nid', check_plain($vsite_id));
    }
    $site_logo = $_GET['site_logo'];
    if (isset($site_logo) && $site_logo == 'true') {
      $query->fieldCondition('field_site_logo', 'fid', '0','<>');
    }
    return $query;
  }

  public function optionalPropertyCallback($property, EntityDrupalWrapper $wrapper) {
    $info = $wrapper->getPropertyInfo ();
    if (isset($info[$property])) {
      return $wrapper->{$property}->value();
    }
    return null;
  }

  /**
   * @param EntityFieldQuery $query
   *
   * Overriding the query list filter. Since this is a group a handler we need
   * to select nodes of 3 types: personal, project, department AKA group.
   */
  public function queryForListFilter(\EntityFieldQuery $query) {
    parent::queryForListFilter($query);

    $query->propertyCondition('type', array('personal', 'project', 'department'), 'IN');
  }

  /**
   * Return all the users for this group.
   */
  public function getGroupUsers($value) {
    $query = new EntityFieldQuery();
    $results = $query
      ->entityCondition('entity_type', 'user')
      ->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $value)
      ->execute();

    $list = array();

    if (empty($results['user'])) {
      return $list;
    }

    $accounts = user_load_multiple(array_keys($results['user']));

    $vsite = vsite_get_vsite($value);
    foreach ($accounts as $account) {
      $role = og_get_user_roles($vsite->group_type, $value, $account->uid, FALSE);
      $list[] = array(
        'uid' => $account->uid,
        'name' => $account->name,
        'roles' => $role,
      );
    }

    return $list;
  }

  /**
   * Returns the currently active theme for the site
   */
  public function getTheme(EntityDrupalWrapper $wrapper) {
    $id = $wrapper->getIdentifier();

    if ($vsite = vsite_get_vsite($id)) {
      $theme = $vsite->controllers->variable->get('theme_default');
      $current_flavor = variable_get('os_appearance_'.$theme.'_flavor','default');

      return $current_flavor;
    }
  }

  public function getBundleForView(EntityDrupalWrapper $wrapper) {
    list(,,$bundle) = entity_extract_ids('node', $wrapper->value());

    return $bundle;
  }

  public function getExternalUrl($wrapper) {
    $id = $wrapper->getIdentifier();
    return url('<front>', array(
      'absolute' => true,
      'purl' => array(
        'provider' => 'spaces_og',
        'id' => $id,
      )
    ));
  }

  public function getPrivacyLevel($wrapper) {
    return $wrapper->{VSITE_ACCESS_FIELD}->value();
  }

  /**
   * {@inheritdoc}
   */
  public function createEntity() {
    $fields = $this->getPublicFields();
    foreach ($fields as $k => $f) {
      if ($f['property'] == 'bundle') {
        if (!empty($this->request[$k])) {
          $this->bundle = $this->request[$k];
        }
        unset($this->request[$k]);
      }
    }

    return parent::createEntity();
  }

  /**
   * {@inheritdoc}
   */
  protected function setPropertyValues(EntityMetadataWrapper $wrapper, $null_missing_fields = FALSE) {
    $request = $this->getRequest();
    self::cleanRequest($request);
    unset($this->request['theme']);

    parent::setPropertyValues($wrapper, $null_missing_fields);
    $id = $wrapper->getIdentifier();

    if (!$space = vsite_get_vsite($id)) {
      return;
    }

    // Set the preset on the object.
    if ($request['preset']) {
      $space->controllers->variable->set('spaces_preset_og', $request['preset']);
    }

    if ($purl = $wrapper->domain->value()) {
      $modifier = array(
        'provider' => 'spaces_og',
        'id' => $id,
        'value' => $purl,
      );
      purl_save($modifier);
    }

    if ($request['theme']) {
      // this value will be a flavor, most likely
      ctools_include('themes', 'os');
      list($theme,,$flavor) = explode('-', $request['theme']);
      if (!empty($flavor)) {
        $space->controllers->variable->set('theme_default', $theme);
        $flavor_key = 'os_appearance_' . $theme . '_flavor';
        $space->controllers->variable->set($flavor_key, $flavor);
      }
      else {
        $themes = os_get_themes ();
        foreach ($themes as $name => $info) {
          $flavors = os_theme_get_flavors ($name);
          if (isset($flavors[$request['theme']])) {
            $space->controllers->variable->set ('theme_default', $name);
            $flavor_key = 'os_appearance_' . $name . '_flavor';
            $space->controllers->variable->set ($flavor_key, $request['theme']);
          }
        }
      }
    }
  }

  public function cleanUser($value) {
    $safe_values = array(

    );
    return $value;
  }

  public function additionalHateoas () {
    $hateoas = array();
    if (is_callable('parent::additionalHateoas')) {
      $hateoas = parent::additionalHateoas();
    }

    if ($batch = &batch_get()) {
      $redirectUrl = array(
        '',
        array()
      );
      batch_process($redirectUrl, 'batch', 'os_restful_output_batch_finished');
      $id = $batch['id'];
      $hateoas += array(
        'batch-id' => $id
      );
    }

    return $hateoas;
  }
}
