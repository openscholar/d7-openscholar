<?php

/**
 * @file
 * Contains RestfulEntityUser__1_0.
 */

class OsRestfulUser extends \RestfulEntityBaseUser {

  public static function controllersInfo() {
    // check needs to be before the catch all from the parent or we don't get the right method
    return array(
      'authenticate' => array(
        \RestfulInterface::POST => 'authenticateUser',
      ),
      'check' => array(
        \RestfulInterface::POST => 'validateField'
      )
    ) + parent::controllersInfo();
  }

  public function publicFieldsInfo() {
    $public_fields = parent::publicFieldsInfo();

    $public_fields['name'] = array(
      'property' => 'name',
    );

    $public_fields['password'] = array(
      'property' => 'pass',
      'callback' => array($this, 'getPassword')
    );

    $public_fields['status'] = array(
      'property' => 'status',
    );

    $public_fields['role'] = array(
      'property' => 'roles',
      'process_callbacks' => array(
        array($this, 'getRoles'),
      ),
    );

    $public_fields['created'] = array(
      'property' => 'created',
    );

    $public_fields['access'] = array(
      'property' => 'uid',
      'process_callbacks' => array(
        array($this, 'getAccess'),
      ),
    );

    $public_fields['login'] = array(
      'property' => 'uid',
      'process_callbacks' => array(
        array($this, 'getLogin'),
      ),
    );

    $public_fields['create_access'] = array(
      'callback' => array($this, 'getCreateAccess')
    );

    $ga_field = og_get_group_audience_fields('user','user','node');
    unset($ga_field['vsite_support_expire']);

    if(count($ga_field)) {
      $public_fields['og_user_node'] = array(
        'property' => key($ga_field),
        'process_callbacks' => array(
          array($this, 'vsiteFieldDisplay'),
        ),
      );
    }

    // Add all non-interal use data fields
    $instances = field_info_instances('user');
    foreach ($instances['user'] as $field_name => $info) {
      if (strpos($field_name, 'field_') === 0 && $field_name != 'field_grouper_path') {
        $public_fields[str_replace('field_','',$field_name)] = array(
          'property' => $field_name,
        );
      }
    }

    $public_fields['permissions'] = array(
      'callback' => array($this, 'getPermissions')
    );

    return $public_fields;
  }

  /**
   * Filter files by vsite
   */
  protected function queryForListFilter(EntityFieldQuery $query) {
    if (!empty($this->request['vsiteid'])) {
      $query->fieldCondition(OG_AUDIENCE_FIELD, 'target_id', $this->request['vsiteid'], 'IN');
    }
  }

  /**
   * Get user password.
   */
  protected function getPassword($user_wrapper) {
    if (!empty($_GET['pass'])) {
      global $user;
      // Load password for users with top level access only.
      $current_roles = $user->roles;
      if ($user->uid == 1 || in_array('administrator', $current_roles)) {
        return $user_wrapper->value()->pass;
      }
    }

    return NULL;
  }

  /**
   * Get user login timestamp.
   */
  protected function getLogin($uid) {
    $load_user = user_load($uid);
    return $load_user->login;
  }

  /**
   * Get user access timestamp.
   */
  protected function getAccess($uid) {
    $load_user = user_load($uid);
    return $load_user->access;
  }

  /**
   * Hide the field value.
   *
   * @return null
   */
  protected function hideField() {
    return NULL;
  }

  /**
   * @inheritdoc
   *
   * Override to allow 0 values through (so we can get permissions for anonymous users)
   */
  public function setPath($path = '') {
    $this->path = implode(',', array_unique(array_filter(explode(',', $path), 'strlen')));
  }

  /**
   * {@inheritdoc}
   *
   * Override to allow 0 values through
   */
  public function viewEntities($ids_string) {
    $ids = array_unique(array_filter(explode(',', $ids_string), 'strlen'));
    $output = array();

    foreach ($ids as $id) {
      $output[] = $this->viewEntity($id);
    }
    return $output;
  }

  /**
   * Override this so anonymous users can create accounts.
   * Restful's rate-limiting should protect us
   */
  public function checkEntityAccess($op, $entity_type, $entity) {
    $account = $this->getAccount();
    if ($op == 'create' && $account->uid == 0) {
      return TRUE;
    }
    return parent::checkEntityAccess($op, $entity_type, $entity);
  }

  /**
   * Overriding the create entity method in order to load the password.inc file.
   */
  public function createEntity() {
    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
    try {
      $account = $this->getAccount();
    }
    catch (\RestfulBadRequestException $e) {
      $account = user_load(0);
      $this->setAccount($account);
    }
    $output = parent::createEntity();
    if ($output[0] != null) {
      return $output;
    }
    // we were denied access to view the entity because the active user is still anonymous
    $user = null;
    if (!empty($this->request['mail'])) {
      $user = user_load_by_mail($this->request['mail']);
    }
    elseif (!empty($this->request['name'])) {
      $user = user_load_by_name($this->request['name']);
    }
    else {
      foreach (user_load_multiple(FALSE, array('created' => REQUEST_TIME)) as $obj) {
        $user = $obj;
        break;
      }
    }

    if ($user) {
      $user = user_save($user, array('status' => 1));
      $this->setAccount($user);
      drupal_save_session(true);
      user_login_finalize();
      // TODO: Remove this with saml
      // lmao, && has higher precedence than = so this was setting $huid to true.
      if (module_exists('pinserver_authenticate') && ($huid = pinserver_get_user_huid ()) && !pinserver_authenticate_get_uid_from_huid ()) {
        pinserver_authenticate_set_user_huid ($user->uid, $huid);
      }
      return $this->viewEntity($user->uid);
    }
  }

  /**
   * Refactor the roles property with rid-name format.
   */
  public function getRoles($roles) {
    $return = array();
    foreach ($roles as $role) {
      $info = user_role_load($role);
      $return[$info->rid] = $info->name;
    }
    return $return;
  }

  /**
   * Returns whether a user can create new sites or not
   * Used in a property
   */
  public function getCreateAccess() {
    if (module_exists('vsite')) {
      global $user;
      if (module_exists('pinserver') && $user->uid != 1) {
        if (!pinserver_user_has_associated_pin ($user->uid)) {
          return false;
        }
      }
      return _vsite_user_access_create_vsite();
    }
  }

  /**
   * Display the id and the title of the group.
   */
  public function vsiteFieldDisplay($values) {
    $account = $this->getAccount();
    ctools_include('subsite', 'vsite');

    $groups = array();
    // Obtaining associative array of custom domains, keyed by space id
    $custom_domains = $this->getCustomDomains($values);
    $purl_base_domain = variable_get('purl_base_domain');
    foreach ($values as $value) {

      $base_site_url = url('', array(
        'absolute' => true,
        'purl' => array(
          'id' => $value->nid,
          'provider' => isset($custom_domains[$value->nid]) ? 'vsite_domain' : 'spaces_og'
        )
      ));
      $base_site_url = rtrim($base_site_url, '/') . '/';  // enforce single trailing slash
      $groups[] = array(
        'title' => $value->title,
        'id' => $value->nid,
        'purl' => $value->purl,
        'delete_base_url' => $base_site_url,
        'owner' => ($value->uid == $account->uid),
        'subsite_access' => vsite_subsite_access('create', $value),
        'delete_access' => node_access('delete', $value),
      );
    }
    return $groups;
  }

  /**
   * Returns associative array of custom domains, keyed by space id
   */
  protected function getCustomDomains($vsites) {
    $space_ids = array();
    foreach ($vsites as $vsite) {
      $space_ids[] = $vsite->nid;
    }
    $result = db_select('purl', 'p')
      ->fields('p', array('id', 'value'))
      ->condition('provider', 'vsite_domain', '=')
      ->condition('id', $space_ids, 'IN')
      ->execute()
      ->fetchAllKeyed(0, 1);
    return $result;
  }

  /**
   * Returns subset of permissions this user holds
   */
  protected function getPermissions(EntityDrupalWrapper $wrapper) {
    $permissions = array();

    $vicariousUser = (user_is_anonymous () && variable_get('os_site_creation_allow_anonymous', false));
    if (module_exists('pinserver')) {
      if ($huid = pinserver_get_user_huid()) {
        if (!$uid = pinserver_authenticate_get_uid_from_huid($huid)) {
          $vicariousUser = true;
        }
      }
    }
    $account = user_load($wrapper->getIdentifier ());

    if ($vicariousUser && user_is_anonymous ()) {
      $account->roles[2] = 'authenticated user';
    }

    $group_bundles = og_get_all_group_bundle('node');
    foreach ($group_bundles as $type => $label) {
      $permissions[] = 'create ' . $type . ' content';
    }

    $output = array();
    foreach ($permissions as $p) {
      $output[$p] = user_access($p, $account);
    }

    return $output;
  }

  /**
   * Validates all fields passed to this function
   */
  protected function validateField() {
    $output = array(
      'pass' => true,
      'errors' => array()
    );
    if (!empty($this->request) && is_array($this->request)) {
      foreach ($this->request as $field => $value) {
        switch ($field) {
          case 'email':
            if ($error = user_validate_mail($value)) {
              $output['pass'] = false;
              $output['errors'][] = $error.' <!--emailPattern-->';
            }
            else {
              $q = db_select('users', 'u')
                ->condition('mail', $value)
                ->countQuery()
                ->execute()
                ->fetchField();

              if ($q > 0) {
                $output['pass'] = false;
                $output['errors'][] = t('This e-mail is already in use. Do you have an account already? <!--emailTaken-->');
              }
            }
            break;
          case 'username':
            if ($error = user_validate_name($value)) {
              $output['pass'] = false;
              $output['errors'][] = $error.' <!--userNameInvalid-->';
            }
            else {
              $q = db_select('users', 'u')
                ->condition('name', $value)
                ->countQuery()
                ->execute()
                ->fetchField();

              if ($q > 0) {
                $output['pass'] = false;
                $output['errors'][] = t('This username is unavailable. <!--userNameTaken-->');
              }
            }
            break;
        }
      }
    }

    return $output;
  }

  /**
   * Authorizes a user given their username and password
   *
   * Copied heavily from user_login_authenticate_validate and user_login_final_validate
   */
  function authenticateUser() {
    $output = array(
      'success' => false,
      'errors' => array(),
    );
    if (empty($this->request['username']) || empty($this->request['password'])) {
      $output['errors'][] = t('Username or password not given.');
    }
    elseif (!flood_is_allowed('failed_login_attempt_ip', variable_get('user_failed_login_ip_limit', 50), variable_get('user_failed_login_ip_window', 3600))) {
      flood_register_event('failed_login_attempt_ip', variable_get('user_failed_login_ip_window', 3600));
      $output['errors'][] = t('Sorry, too many failed login attempts from your IP address. This IP address is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url('user/password')));
    }
    else {
      $account = db_query("SELECT * FROM {users} WHERE name = :name OR mail = :name AND status = 1", array(':name' => $this->request['username']))->fetchObject();
      if ($account) {
        if (variable_get('user_failed_login_identifier_uid_only', FALSE)) {
          // Register flood events based on the uid only, so they apply for any
          // IP address. This is the most secure option.
          $identifier = $account->uid;
        }
        else {
          // The default identifier is a combination of uid and IP address. This
          // is less secure but more resistant to denial-of-service attacks that
          // could lock out all users with public user names.
          $identifier = $account->uid . '-' . ip_address();
        }

        // Don't allow login if the limit for this user has been reached.
        // Default is to allow 5 failed attempts every 6 hours.
        if (!flood_is_allowed('failed_login_attempt_user', variable_get('user_failed_login_user_limit', 5), variable_get('user_failed_login_user_window', 21600), $identifier)) {
          flood_register_event('failed_login_attempt_user', variable_get('user_failed_login_user_window', 21600), $form_state['flood_control_user_identifier']);
          $output['errors'][] = format_plural(variable_get('user_failed_login_user_limit', 5), 'Sorry, there has been more than one failed login attempt for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', 'Sorry, there have been more than @count failed login attempts for this account. It is temporarily blocked. Try again later or <a href="@url">request a new password</a>.', array('@url' => url('user/password')));
        }
        else {
          $uid = user_authenticate($account->name, $this->request['password']);
          if (empty($uid)) {
            $output['errors'][] = t('Sorry, unrecognized username or password. <a href="@password">Have you forgotten your password?</a>', array('@password' => url('user/password')));
          }
          else {
            // Clear past failures for this user so as not to block a user who might
            // log in and out more than once in an hour.
            flood_clear_event('failed_login_attempt_user', $identifier);
            global $user;
            $user = user_load($uid);
            user_login_finalize();
            $output['success'] = true;
            $output['user'] = array(
              'uid' => $account->uid,
              'name' => $account->name,
              'mail' => $account->mail,
              'realName' => (!empty($user->field_first_name[LANGUAGE_NONE][0]['value']) ? $user->field_first_name[LANGUAGE_NONE][0]['value'] : '').' '.(!empty($user->field_last_name[LANGUAGE_NONE][0]['value']) ? $user->field_last_name[LANGUAGE_NONE][0]['value'] : ''),
            );
          }
        }
      }
      else {
        $output['errors'][] = t('Sorry, unrecognized username or password. <a href="@password">Have you forgotten your password?</a>', array('@password' => url('user/password')));
      }
    }

    return $output;
  }

  /**
   * Overrides \RestfulEntityBase::getQueryForList().
   *
   * Fetch updated users in the listing.
   */
  public function getQueryForList() {
    $query = parent::getQueryForList();
    if (!empty($this->request['changed'])) {
      $query->propertyCondition('changed', $this->request['changed'], '>=');
    }
    if (!empty($this->request['vsiteid'])) {
      $query->fieldCondition('og_user_node', 'target_id', $this->request['vsiteid'], 'IN');
    }
    return $query;
  }

  /**
   * Overrides \RestfulEntityBase::getQueryCount().
   *
   * Fetch updated user count.
   */
  public function getQueryCount() {
    $query = parent::getQueryCount();
    if (!empty($this->request['changed'])) {
      $query->propertyCondition('changed', $this->request['changed'], '>=');
    }
    if (!empty($this->request['vsiteid'])) {
      $query->fieldCondition('og_user_node', 'target_id', $this->request['vsiteid'], 'IN');
    }

    return $query->count();
  }

  /**
   * Get the pager range.
   *
   * @return int
   *  The range.
   */
  public function getRange() {
    return 500;
  }
}
