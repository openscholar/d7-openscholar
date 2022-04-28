<?php

class OsRestfulCPSettings extends \RestfulBase implements \RestfulDataProviderInterface {

  /**
   * Overrides \RestfulBase::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        // If they don't pass a menu-id then display nothing.
        \RestfulInterface::GET => 'getAllForms',
        \RestfulInterface::HEAD => 'getAllForms',
        \RestfulInterface::PUT => 'saveSettings'
      ),
      // We don't know what the ID looks like, assume that everything is the ID.
      '^.*$' => array(
        \RestfulInterface::GET => 'getForms',
        \RestfulInterface::HEAD => 'getForms'
      ),
    );
  }

  public function publicFieldsInfo() {
    return array();
  }

  public function getAllForms() {
    $this->activateSpace();
    return $this->processForms(cp_get_setting_forms());
  }

  public function getForms($args) {
    if ($this->activateSpace()) {
      $forms = explode(',', $args);
      $all = $this->processForms(cp_get_setting_forms());

      $output = array();
      foreach ($all as $f) {
        if (in_array($f['group']['#id'], $forms)) {
          $output = $f;
        }
      }

      return $output;
    }

    throw new RestfulForbiddenException("Vsite ID is required.");
  }

  public function processForms($form) {
    $access = spaces_access_admin();
    foreach ($form as $var => &$elem) {
      if (!isset($elem['form']['#id'])) {
        $elem['form']['#id'] = drupal_html_id('edit-' . $var);
      }

      if (!isset($elem['form']['#access'])) {
        $elem['form']['#access'] = $access;
      }
      if (!empty($elem['form']['#states'])) {
        drupal_process_states($elem['form']);
      }
    }

    return $form;
  }

  public function saveSettings() {
    if ($this->activateSpace()) {
      $forms = cp_get_setting_forms();

      $valid = array();

      // output back to the angular app
      $flags = array();

      foreach ($this->request as $var => $value) {
        if (!isset($forms[$var])) continue;

        // validation
        if (!isset($valid[$var])) {
          if (!empty($forms[$var]['group']['#group_validate'])) {
            $values = array();
            foreach ($this->request as $v2 => $val2) {
              if ($forms[$var]['group']['#id'] == $forms[$v2]['group']['#id']) {
                $values[$v2] = $val2;
              }
            }
            $result = $forms[$var]['group']['#group_validate']($values);
            foreach ($values as $k => $t) {
              $valid[$k] = $result;
            }
          }
          else if (!empty($forms[$var]['rest_validate']) && is_callable($forms[$var]['rest_validate'])) {
            $valid[$var] = $forms[$var]['rest_validate']($value);
          }
        }

        // submission
        if (!isset($valid[$var]) || $valid[$var]) {
          if (!empty($forms[$var]['rest_submit']) && is_callable($forms[$var]['rest_submit'])) {
            if (!empty($forms[$var]['submit_full_request']) && $forms[$var]['submit_full_request']) {
              $forms[$var]['rest_submit']($this->request);
            } else {
              $forms[$var]['rest_submit']($value, $var);
            }
          }
          elseif (!empty($forms[$var]['rest_trigger']) && is_callable($forms[$var]['rest_trigger'])) {
            if ($value) {
              if ($f = $forms[$var]['rest_trigger']()) {
                $flags = array_merge($flags, $f);
              };
            }
          }
          else {
            $this->saveVariable($var, $value);
          }

          if (!empty($forms[$var]['rest_after_submit'])) {
            if (is_array($forms[$var]['rest_after_submit']) && !is_callable($forms[$var]['rest_after_submit'])) {
              foreach ($forms[$var]['rest_after_submit'] as $func) {
                if (is_callable($func)) {
                  if ($f = $func($value, $var)) {
                    $flags = array_merge($flags, $f);
                  }
                }
              }
            }
            elseif (is_callable($forms[$var]['rest_after_submit'])) {
              if ($f = $forms[$var]['rest_after_submit']($value, $var)) {
                $flags = array_merge($flags, $f);
              }
            }
          }
        }
        else {
          // something about an error?
          watchdog("REST", "The value \"$value\" is not valid for \"$var\".");
          $flags[] = array(
            'type' => 'validation',
            'var' => $var,
          );
        }
      }

      return $flags;
    }

    throw new RestfulForbiddenException("Vsite ID is required.");
  }

  private function saveVariable($var, $val) {
    if (!empty($_GET['vsite'])) {
      if ($vsite = vsite_get_vsite($_GET['vsite'])) {
        $vsite->controllers->variable->set($var, $val);
      }
    }
    else {
      variable_set($var, $val);
    }

  }

  /**
   * Handle activating the space for access and variable override purposes
   * @return bool - TRUE if the space activated
   */
  protected function activateSpace() {
    if (!empty($_GET['vsite']) && $vsite = vsite_get_vsite($_GET['vsite'])) {
      // Make sure the Drupal $user account is the account Restful authenticated
      $account = $this->getAccount();
      spaces_set_space($vsite);
      $vsite->activate_user_roles();
      $vsite->init_overrides();
      return true;
    }
    return false;
  }


  public function additionalHateoas() {
    return array(
      'messages' => drupal_get_messages()
    );
  }
}
