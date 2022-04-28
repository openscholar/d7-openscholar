<?php

class Grouper extends \RestfulBase implements \RestfulDataProviderInterface {

  /**
   * Overrides \RestfulBase::controllersInfo().
   */
  public static function controllersInfo() {
    return array(
      '' => array(
        // If they don't pass a menu-id then display nothing.
        \RestfulInterface::GET => 'getAllGroups',
        \RestfulInterface::HEAD => 'getAllGroups',
        \RestfulInterface::PUT => 'saveGroupToSite'
      ),
    );
  }

  public function publicFieldsInfo() {
    return array();
  }

  public function getAllGroups() {
    $huid = $this->getHUIDForUser();
    $cid = 'grouper:'.$huid;

    if ($cache = cache_get($cid)) {
      return $cache->data;
    }
    else {
      $grouper_url = variable_get('grouper_url', 'https://{user}:{pass}@grouper-ws-prod-pub.gslb.harvard.edu/grouper-ws-facade/v1/user/huid:{huid}/{accessLevel}');
      $replacements = array(
        '{user}' => variable_get('grouper_user'),
        '{pass}' => variable_get('grouper_pass'),
        '{huid}' => $huid,
        '{accessLevel}' => 'viewablegroups',
      );
      $grouper_url = strtr($grouper_url, $replacements);
      watchdog('Grouper', "Grouper URL: $grouper_url");

      $response = drupal_http_request($grouper_url);
      if ($response->code == 200) {
        $raw_data = $response->data;
      }
      else {
        switch ($response->code) {
          case 403:
            throw new RestfulForbiddenException("Cannot access Grouper restful interface.");
            break;
          default:
            throw new RestfulException("Error fetching grouper response: $response->code: $response->error");
        }
      }

      $cleaned = json_decode($raw_data);
      return $cleaned;
    }
  }

  private function getHUIDForUser() {
    // TODO: Replace this with actual function
    $account = $this->getAccount();
    return pinserver_authenticate_get_user_huid($account->uid);
  }
}