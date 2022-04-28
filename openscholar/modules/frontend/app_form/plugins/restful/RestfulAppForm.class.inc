<?php

class RestfulAppform extends RestfulBase implements \RestfulDataProviderInterface {

  /**
   * Overrides \RestfulBase::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        \RestfulInterface::GET => 'getAppStates',
        \RestfulInterface::PATCH => 'setAppStates'
      ),
    );
  }

  /**
   * Overrides \RestfulInterface::publicFieldsInfo().
   *
   * Empty function to fulfill interface requirements.
   */
  public function publicFieldsInfo() {
    return array();
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

  public function getAppStates() {
    $this->activateSpace();

    $os_help = array( 'Blog' => 'https://help.theopenscholar.com/blogs',
                      'Classes' => 'https://help.theopenscholar.com/classes-app',
                      'FAQs' => 'https://help.theopenscholar.com/faq-app',
                      'Links' => 'https://help.theopenscholar.com/links-app',
                      'News' => 'https://help.theopenscholar.com/news-app',
                      'Basic Pages' => 'https://help.theopenscholar.com/basic-page',
                      'Software' => 'https://help.theopenscholar.com/software-app',
                      'Private Files' => 'https://help.theopenscholar.com/private-files',
                      'Booklets' => 'https://help.theopenscholar.com/booklet',
                      'Events' => 'https://help.theopenscholar.com/events-app',
                      'Media Gallery' => 'https://help.theopenscholar.com/media-gallery',
                      'Presentations' => 'https://help.theopenscholar.com/presentations-app',
                      'Profiles' => 'https://help.theopenscholar.com/people-profiles',
                      'Publications' => 'https://help.theopenscholar.com/creating-publications',
                      'Reader' => 'https://help.theopenscholar.com/rss-feed'
                    );
    $features = variable_get('spaces_features', array());
    $info = spaces_features('og');

    $output = array();
    foreach ($info as $name => $i) {
      $output[] = array(
        'title' => $i->info['name'],
        'machine_name' => $name,
        'description' => $i->info['description'],
        'enabled' => !empty($features[$name]) ? 1 : 0,
        'private' => !empty($features[$name]) ? $features[$name] - 1 : 0,
        'help' => $os_help[$i->info['name']]
      );
    }

    return $output;
  }

  public function setAppStates() {
    $this->activateSpace();

    $original = variable_get('spaces_features', array());
    foreach ($this->request as $key => $val) {
      if (is_numeric($key)) {
        $name = $val['machine_name'];
        if ($val['enabled']) {
          if ($val['private']) {
            $original[$name] = 2;
          }
          else {
            $original[$name] = 1;
          }
        }
        else {
          $original[$name] = 0;
        }
      }
    }

    if (module_exists('vsite') && $vsite = vsite_get_vsite()) {
      $vsite->controllers->variable->set('spaces_features', $original);
    }
    else {
      variable_set('spaces_features', $original);
    }
  }
}
