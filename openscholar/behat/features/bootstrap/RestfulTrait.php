<?php

use Behat\Gherkin\Node\TableNode;
use Behat\Gherkin\Node\PyStringNode;
use GuzzleHttp\Message\ResponseInterface;

/**
 * Restful trait. This trait will hold all the methods and steps which relate
 * to the restful tests.
 */
trait RestfulTrait {

  /**
   * @var array
   *
   * Holds list of endpoints path.
   */
  private $endpoints = [
    'biblio' => 'api/biblio',
    'bio' => 'api/bio',
    'blog' => 'api/blog',
    'book' => 'api/book',
    'box' => 'api/boxes',
    'class' => 'api/class',
    'class_material' => 'api/class_material',
    'cv' => 'api/cv',
    'event' => 'api/event',
    'faq' => 'api/faq',
    'feed' => 'api/feed',
    'group' => 'api/group',
    'image_gallery' => 'api/media_gallery',
    'layout' => 'api/layouts',
    'news' => 'api/news',
    'og_vocab' => 'api/og_vocab',
    'page' => 'api/page',
    'person' => 'api/person',
    'presentation' => 'api/presentation',
    'slideshow_slide' => 'api/slideshow_slide',
    'software_project' => 'api/software_project',
    'software_release' => 'api/software_release',
    'taxonomy' => 'api/taxonomy',
    'variable' => 'api/variables',
    'vocabulary' => 'api/vocabulary',
  ];

  /**
   * @var array
   *
   * List of widgets and the machine name they represent.
   */
  private $widgets = [
    'Terms' => 'os_taxonomy_fbt',
    'Pub' => 'os_boxes_pub_year',
  ];

  /**
   * @var String
   *
   * Holds the access token for the user.
   */
  private $accessToken = array();

  /**
   * @var array
   *
   * Generic metadata from tests.
   */
  private $meta = array();

  /**
   * @var array
   *
   * List of operations.
   */
  private $operations = [
    'create' => 'post',
    'update' => 'put',
    'patch' => 'patch',
    'delete' => 'delete',
  ];

  /**
   * @var array
   *
   * Results from a JSON request.
   */
  private $results = array();

  /**
   * Alias for Guzzle client.
   *
   * @return \GuzzleHttp\Client
   */
  private function getClient() {
    return new GuzzleHttp\Client();
  }

  /**
   * Login via rest to get the user's access token.
   *
   * @param $user
   *   The user name.
   *
   * @return string
   *   The user access token.
   */
  private function restLogin($user) {
    if (isset($this->accessToken[$user])) {
      return $this->accessToken[$user]['access_token'];
    }

    if ($handler = new RestfulAccessTokenAuthentication(['entity_type' => 'restful_token_auth','bundle' => 'access_token'])) {
      if ($account = user_load_by_name ($user)) {
        $handler->setAccount ($account);
        $data = $handler->getOrCreateToken ();

        $this->accessToken[$user] = $data;
        return $data['access_token'];
      }
      else {
        throw new Exception("No user with name $user found");
      }
    }
    else {
      throw new Exception('No Restful Access handler found');
    }
  }

  /**
   * Handling non 200 http request.
   *
   * @param \GuzzleHttp\Exception\ClientException $e
   *   The client exception handler.
   *
   * @throws Exception
   */
  private function handleExceptions(\GuzzleHttp\Exception\ClientException $e, $return = FALSE) {
    $json = $e->getResponse()->json();

    $implode = array();
    if (!empty($json['errors'])) {
      foreach ($json['errors'] as $errors) {
        foreach ($errors as $error) {
          $implode[] = $error;
        }
      }
    }
    else {
      $implode[] = isset($json['title']) ? $json['title'] : '';
    }

    $errors = implode(', ', $implode);

    if ($return) {
      return $errors;
    }

    throw new Exception('Your request has failed: ' . $errors);
  }

  /**
   * Take the table head and the table body into one single array.
   *
   * todo: handle more then one line.
   *
   * @param TableNode $table
   *   The table object.
   * @param $table_to_fields
   *   Determine if the title of the fields from the table should be convert to
   *   field machine name. i.e: Taxonomy ref => field_taxonomy_ref.
   *
   * @return array
   */
  private function getValues(TableNode $table, $table_to_fields = FALSE) {
    $rows = $table->getRows();

    // Convert the titles to machine names.
    if ($table_to_fields) {

      foreach ($rows[0] as &$field) {
        $field = str_replace(' ', '_', strtolower($field));
      }
    }

    $return = array();

    foreach (array_slice($table->getRows(), 1) as $tbody) {
      $return[] = array_combine($rows[0], $tbody);
    }

    return count($table->getRows()) == 2 ? $return[0] : $return;
  }

  /**
   * Get the delta for the widget.
   *
   * @param $values
   *   The settings from the step definition.
   * @return int
   *   The delta of the widget. for a new widget the timestamp will be returned.
   */
  private function getDelta($values) {
    // Get the delta by specific conditions.
    if (!empty($values['Delta'])) {
      return $values['Delta'] == 'PREV' ? $this->meta['delta'] : $values['Delta'];
    }
    else {
      return time();
    }
  }

  /**
   * Verify the rest operation passed.
   *
   * @param $operation
   *   The type of the operation: PUT, DELETE or POST.
   *
   * @throws Exception
   */
  private function verifyOperationPassed($operation) {
    // Verify the request did what it suppose to do.
    $results = $this->results['data'];

    if (array_key_exists(0, $results)) {
      $results = $results[0];
    }

    if ($operation == 'delete') {
      if (!empty($results['value']) && $results['value']['description'] == $this->meta['widget']['description']) {
        throw new Exception('The box was not deleted.');
      }
    }
    else {
      if ($results['value']['description'] != $this->meta['widget']['description']) {
        throw new Exception('The results for the box not matching the settings you passed.');
      }
    }
  }

  /**
   * Return array with a needed variables to the rest operations.
   *
   * @param $type
   *   The type of the operation.
   * @param $account
   *   The user name.
   * @param TableNode $table
   *   The table settings from the step definition.
   * @param $table_to_fields
   *   Determine if the title of the fields from the table should be convert to
   *   field machine name. i.e: Taxonomy ref => field_taxonomy_ref.
   *
   * @return array
   */
  private function getVariables($type, $account, TableNode $table, $table_to_fields = FALSE) {
    return [
      $this->getValues($table, $table_to_fields),
      $this->restLogin($account),
      $this->locatePath($this->endpoints[$type]),
    ];
  }

  /**
   * Invoke a rest request.
   *
   * @param $method
   *   The method: POST, GET etc. etc.
   * @param $path
   *   The path of the request.
   * @param $headers
   *   The headers of the request.
   * @param $body
   *   The body of the request AKA the payload.
   * @param $return
   *   Determine if we need to return the request errors.
   *
   * @return ResponseInterface
   *   The request object.
   * @throws Exception
   */
  private function invokeRestRequest($method, $path, $headers, $body, $return = FALSE) {
    try {
      $response = $this->getClient()->{$method}($path, [
        'headers' => $headers,
        'json' => $body,
      ]);
    } catch (\GuzzleHttp\Exception\ClientException $e) {
      return $this->handleExceptions($e, $return);
    }

    return $response;
  }

  /**
   * @Given /^I test the exposed resources:$/
   */
  public function iTestTheExposedResources(PyStringNode $resources) {
    foreach ($resources->getLines() as $line) {
      $this->getClient()->get($this->locatePath($line));
    }
  }

  /**
   * @Given /^I "([^"]*)" a box as "([^"]*)" with the settings:$/
   */
  public function iAAsWithTheSettings($operation, $account, TableNode $table) {
    list($values, $token, $path) = $this->getVariables('box', $account, $table);
    $delta = $this->getDelta($values);
    $viste = FeatureHelp::getNodeId($values['Site']);

    $request = $this->invokeRestRequest($this->operations[$operation], $path,
      ['access-token' => $token],
      [
        'vsite' => $viste,
        'delta' => $delta,
        'widget' => $this->widgets[$values['Widget']],
        'options' => [
          'description' => $values['Description'],
        ],
      ]
    );

    $this->meta['delta'] = $request->json()['data']['delta'];
    $this->meta['widget'] = $request->json()['data'];
    $headers = ['headers' => ['access-token' => $token]];
    $get = $this->getClient()->get($path . '/' . $delta . '?vsite=' . $viste, $headers);
    $this->results = $get->json();
    $this->verifyOperationPassed($operation);
  }

  /**
   * @Given /^I "([^"]*)" a layout as "([^"]*)" with the settings:$/
   */
  public function iALayoutAsWithTheSettings($operation, $account, TableNode $table) {
    list($values, $token, $path) = $this->getVariables('layout', $account, $table);
    $box_path = $this->locatePath($this->endpoints['box']);
    $op = $this->operations[$operation];
    $delta = $this->getDelta($values);

    if ($op == 'post') {
      $request = $this->invokeRestRequest($op, $box_path,
        ['access-token' => $token],
        [
          'vsite' => FeatureHelp::getNodeId($values['Site']),
          'delta' => $delta,
          'widget' => $this->widgets[$values['Box']],
          'options' => [
            'description' => 'Widget for testing a layout',
          ],
        ]
      );

      $this->meta['delta'] = $delta = $request->json()['data']['delta'];

      $blocks = [
        'boxes-' . $delta => [
          'module' => 'boxes',
          'delta' => $delta,
          'region' => 'sidebar_second',
          'weight' => 2,
          'status' => 0,
        ],
      ];
    }
    elseif ($op == 'put') {
      $blocks = [
        'boxes-' . $delta => [
          'region' => 'sidebar_first',
        ],
      ];
    }
    else {
      // Create the layout override.
      $this->invokeRestRequest($op, $path,
        ['access-token' => $token],
        [
          'vsite' => FeatureHelp::getNodeId($values['Site']),
          'object_id' => $values['Context'],
          'delta' => 'boxes-' . $delta,
        ]
      );
      return;
    }

    // Create the layout override.
    $this->invokeRestRequest($op, $path,
      ['access-token' => $token],
      [
        'vsite' => FeatureHelp::getNodeId($values['Site']),
        'object_id' => $values['Context'],
        'blocks' => $blocks,
      ]
    );
  }

  /**
   * @Given /^I "([^"]*)" the variable "([^"]*)" as "([^"]*)" with the value "([^"]*)" in "([^"]*)"$/
   */
  public function iTheVariableAsWithTheValue($operation, $name, $account, $value, $site) {
    $token = $this->restLogin($account);
    $path = $this->locatePath($this->endpoints['variable']);
    $op = $this->operations[$operation];

    $this->invokeRestRequest($op, $path,
      ['access-token' => $token],
      [
        'vsite' => FeatureHelp::getNodeId($site),
        'object_id' => $name,
        'value' => $value,
      ]
    );
  }

  /**
   * @Given /^I try to "([^"]*)" a box as "([^"]*)" in "([^"]*)"$/
   */
  public function iTryToABoxAsIn($operation, $account, $site) {
    $token = $this->restLogin($account);
    $path = $this->locatePath($this->endpoints['box']);
    $delta = $this->getDelta(array());

    $request = $this->invokeRestRequest($this->operations[$operation], $path,
      ['access-token' => $token],
      [
        'vsite' => FeatureHelp::getNodeId($site),
        'delta' => $delta,
        'widget' => $this->widgets['Terms'],
        'options' => [
          'description' => 'Dummy one',
        ],
      ],
      TRUE
    );

    if ($request != "Access denied.") {
      throw new Exception('The user did not got the expected message.');
    }
  }

  /**
   * @Given /^I create a new node of "([^"]*)" as "([^"]*)" with the settings:$/
   */
  public function iCreateANewNodeOfAsWithTheSettings($type, $account, TableNode $table) {
    list($values, $token, $path) = $this->getVariables($type, $account, $table, TRUE);
    $values['vsite'] = FeatureHelp::getNodeId($values['vsite']);

    foreach (['parent', 'software_project'] as $key) {
      if (!empty($values[$key])) {
        $values[$key] = FeatureHelp::getNodeId($values[$key]);
      }
    }

    if (!empty($values['professional_title'])) {
      $values['professional_title'] = array($values['professional_title']);
    }

    if (!empty($values['files'])) {
      $values['files'] = FeatureHelp::getFilesIDs(explode(',', $values['files']));
    }

    if (!empty($values['package'])) {
      $file = $this->getClient()->get($this->locatePath('os-package-file'))->json();

      if (empty($file['file']['fid'])) {
        throw new Exception('An error occured with the file. You ca the value above.');
      }
      $values['package'] = $file['file']['fid'];
    }

    $this->invokeRestRequest('post', $path,
      ['access-token' => $token],
      $values
    );
  }

  /**
   * @Given /^I "([^"]*)" a term as "([^"]*)" with the settings:$/
   */
  public function iATermAsWithTheSettings($operation, $account, TableNode $table) {
    list($values, $token, $path) = $this->getVariables('taxonomy', $account, $table, TRUE);
    $method = $this->operations[$operation];

    if ($method != 'post') {
      $path .= '/' . $this->meta['id'];
    }

    $request = $this->invokeRestRequest($method, $path, ['access-token' => $token], $values);
    if ($method == 'delete') {
      if (!empty($request->json()['data'])) {
        throw new \Exception('The delete of the taxonomy term did not occurred.');
      }
    }
    else {
      $this->meta = $request->json()['data'][0];
      if ($this->meta['label'] != $values['label']) {
        throw new Exception("The label of the entity is {$this->meta['label']} and not {$values['label']}");
      }
    }
  }

  /**
   * @Given /^I "([^"]*)" a group as "([^"]*)":$/
   */
  public function iAGroup($operation, $account, TableNode $table) {
    list($groups, $token, $path) = $this->getVariables('group', $account, $table);
    $op = $this->operations[$operation];

    if ($operation == 'create') {
      foreach ($groups as $group) {
        $this->invokeRestRequest($op, $path,
          ['access-token' => $token],
          $group
        );
      }
    }
  }

  /**
   * @Given /^I verify vsite content:$/
   */
  public function iVerifyVsiteContent(TableNode $table) {
    $values = $this->getValues($table);

    foreach ($values as $value) {
      $this->visit($value['purl']);
      $this->assertPageContainsText($value['text']);
    }
  }

  /**
   * @Given /^I "([^"]*)" a vocabulary as "([^"]*)" with the settings:$/
   */
  public function iAVocabularyAsWithTheSettings($operation, $account, TableNode $table) {
    list($values, $token, $path) = $this->getVariables('vocabulary', $account, $table, TRUE);

    $method = $this->operations[$operation];

    if ($method != 'post') {
      $path .= '/' . $this->meta['id'];
    }
    else {
      $values['vsite'] = FeatureHelp::getNodeId($values['vsite']);
    }

    $request = $this->invokeRestRequest($method, $path, ['access-token' => $token], $values);
    if ($method == 'delete') {
      if (!empty($request->json()['data'])) {
        throw new \Exception('The delete of the vocabulary did not occurred.');
      }
    }
    else {
      $this->meta = $request->json()['data'][0];
      if ($this->meta['label'] != $values['label']) {
        throw new Exception("The label of the entity is {$this->meta['label']} and not {$values['label']}");
      }
    }
  }

  /**
   * @Given /^I "([^"]*)" OG vocabulary as "([^"]*)" with the settings:$/
   */
  public function iOGVocabularyAsWithTheSettings($operation, $account, TableNode $table) {
    list($values, $token, $path) = $this->getVariables('og_vocab', $account, $table, TRUE);
    $method = $this->operations[$operation];

    if (!empty($values['vocabulary'])) {
      $values['vid'] = taxonomy_vocabulary_machine_name_load($values['vocabulary'])->vid;
      unset($values['vocabulary']);
    }

    if ($method != 'post') {
      $path .= '/' . $this->meta['id'];
    }

    $request = $this->invokeRestRequest($method, $path, ['access-token' => $token], $values);

    if ($method == 'delete') {
      if (!empty($request->json()['data'])) {
        throw new \Exception('The delete of the vocabulary did not occurred.');
      }
    }
    else {
      $this->meta = $request->json()['data'][0];
      if ($this->meta['bundle'] != $values['bundle']) {
        throw new Exception("The bundle of the entity is {$this->meta['bundle']} and not {$values['bundle']}");
      }
    }
  }

  /**
   * @Given /^I consume "([^"]*)" as "([^"]*)"$/
   */
  public function iConsumeAs($enpdoint, $account) {
    $token = $this->restLogin($account);
    $this->results = $this->invokeRestRequest('get', $this->locatePath($enpdoint), ['access-token' => $token], [], TRUE);
  }

  /**
   * @Given /^I verify the request "([^"]*)"$/
   */
  public function iVerifyTheRequest($status) {
    if ($status == 'passed') {
      if (!($this->results instanceof ResponseInterface) || $this->results->getStatusCode() != 200) {
        throw new Exception('The last REST request did not passed');
      }
    }
    else {
      if (!is_string($this->results)) {
        throw new Exception('The last REST request did not failed');
      }
    }
  }

  /**
   * @Given /^I define "([^"]*)" as a "([^"]*)" group$/
   */
  public function iDefineAsA($group, $access_level) {
    $nid = FeatureHelp::getNodeId($group);
    $level = $access_level == 'private' ? VSITE_ACCESS_PRIVATE : VSITE_ACCESS_PUBLIC;
    $wrapper = entity_metadata_wrapper('node', $nid);
    $wrapper->{VSITE_ACCESS_FIELD}->set($level);
    $wrapper->save();
  }

  /**
   * @Given /^I try to post a "([^"]*)" as "([^"]*)" to "([^"]*)"$/
   */
  public function iTryToPostAAsTo($type, $account, $group) {
    $gid = FeatureHelp::getNodeId($group);
    $values = [
      'label' => 'Test',
      'body' => 'Test ' . $type,
      'vsite' => $gid,
    ];
    try {
      $this->invokeRestRequest('post', $this->locatePath($this->endpoints[$type]), ['access-token' => $this->restLogin($account)], $values);
      $this->meta['passed'] = TRUE;
    } catch (\Exception $e) {
      $this->meta['passed'] = FALSE;
    }
  }

  /**
   * @Given /^I verify it "([^"]*)"$/
   */
  public function iVerifyIt($status) {
    if (($status == 'passed' && !$this->meta['passed']) || $status == 'failed' && $this->meta['passed']) {
      throw new Exception('The last request has failed');
    }
  }

  private function jsonContent() {
    if (!empty($this->results) && method_exists($this->results, 'json')) {
      return $this->results->json ()['data'];
    }
    return '';
  }

  /**
   * @Given /^I should get empty json$/
   */
  public function iShouldGetEmptyJson() {
    $json = $this->jsonContent();
    if (!empty($json)) {
      throw new \Exception('The json is not empty.');
    }
  }

  /**
   * @Given /^I should not get empty json$/
   */
  public function iShouldNotGetEmptyJson() {
    $json = $this->jsonContent();
    if (empty($json)) {
      throw new \Exception('The json is empty.');
    }
  }
}
