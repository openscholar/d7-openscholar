<?php

/**
 * @file
 * Contains \OsRestfulUserReport
 */
class OsRestfulUserReport extends \OsRestfulReports {

  /**
   * @var string
   *
   * The type of users to show
   */
  protected $userRoles = '';

  /**
   * {@inheritdoc}
   */
  public function publicFieldsInfo() {
    return array(
      'username' => array(
        'property' => 'name',
      ),
      'mail' => array(
        'property' => 'mail',
      ),
    );
  }

  public function runReport() {
    $request = $this->getRequest();
    $this->userRoles = (isset($request['vsite']) && isset($request['roles'])) ? $request['roles'] : "all";
    $results = $this->getQueryForList()->execute();
    $return = array();

    foreach ($results as $result) {
      $return[] = $this->mapDbRowToPublicFields($result);
    }

    return $return;
  }

  /**
   * {@inheritdoc}
   *
   * add additional fields and table joins
   */
  public function getQueryForList() {
    $query = $this->getQuery();
    $fields = $this->getPublicFields();

    // if site association is included in requested report, get vsite (and maybe role) data
    if (isset($fields['vsite'])) {
      $query->addField('n', 'title', 'title');
      $query->addField('n', 'uid', 'owner');
      $query->addField('n', 'nid', 'vsite_id');
      $query->innerJoin("og_membership", "ogm", "ogm.entity_type = 'user' AND ogm.etid = users.uid");

      if ($this->userRoles != "owners") {
        $query->innerJoin("node", "n", "ogm.entity_type = 'user' AND ogm.etid = users.uid AND ogm.gid = n.nid");
      }
      else {
        $query->innerJoin("node", "n", "ogm.entity_type = 'user' AND ogm.etid = users.uid AND ogm.gid = n.nid AND users.uid = n.uid");
      }

      $query->innerJoin("og_users_roles", "ogur", "ogur.uid = users.uid AND ogur.gid = ogm.gid");
      $query->groupBy("users.uid, n.nid");
      if(isset($fields['role_name'])) {
        $query->addField('ogr', 'name', 'role_name');
        $query->innerJoin("og_role", "ogr", "ogr.group_bundle = n.type AND ogr.group_type = 'node' AND ogur.rid = ogr.rid");
      }
    }

    if(isset($fields['fname'])) {
      $query->addField('ffname', 'field_first_name_value', 'fname');
      $query->leftJoin("field_data_field_first_name", "ffname", "ffname.entity_type = 'user' AND ffname.entity_id = users.uid");
    }
    if(isset($fields['lname'])) {
      $query->addField('flname', 'field_last_name_value', 'lname');
      $query->leftJoin("field_data_field_last_name", "flname", "flname.entity_type = 'user' AND flname.entity_id = users.uid");
    }
    if(isset($fields['latest_content'])) {
      $query->addExpression('0', "latest_content");
    }

    $this->queryForListSort($query);
    $this->queryForListFilter($query);
    $this->queryForListPagination($query);
    $this->addExtraInfoToQuery($query);

    return $query;
  }

  /**
   * Overriding the query list filter method
   */
  protected function queryForListFilter(\SelectQuery $query) {
    parent::queryForListFilter($query);

    // limit by role, if needed
    // radio buttons: only site owners, site owners/admins, all users
    if ($this->userRoles == "admins") {
      $query->condition("ogr.name", '%' . db_like("vsite") . '%', "LIKE");
    }
  }

  /**
   * {@inheritdoc}
   *
   * adds logic to handle site roles and latest updated content, if needed
   */
  public function mapDbRowToPublicFields($row) {
    global $base_url;
    $new_row = parent::mapDbRowToPublicFields($row);

    // if site association is included in requested report, include title and url
    if (isset($row->vsite_id)) {
      $new_row['vsite_name'] = $row->title;
      $new_row['vsite_url'] = $base_url . "/node/" . $row->vsite_id;
      unset($new_row['vsite']);

      // if role is requested, differentiate between owner and admin
      if (isset($row->role_name)) {
        if (strpos($row->role_name, "vsite") !== FALSE) {
          if ($row->uid == $row->owner) {
            $new_row['role_name'] = "Site Owner";
          }
          else {
            $new_row['role_name'] = "Administrator";
          }
        }
        else {
          $new_row['role_name'] = ucfirst($row->role_name);
        }
      }

      // if latest modified content is requested
      if (isset($row->latest_content)) {
        $query = db_select("node", "n")
                 ->condition('n.uid', $row->uid, '=');
        $query->innerJoin('og_membership', 'ogm', "ogm.etid = n.nid AND ogm.gid = '" . $row->vsite_id . "'");
        $query->addExpression('MAX(n.changed)');
        $date = $query->execute()->fetchField();
        if ($date) {
          $new_row['latest_content'] = date('M j, Y h:ia', $date);
        }
        else {
          $new_row['latest_content'] = "";
        }
      }
    }
    return $new_row;
  }
}
