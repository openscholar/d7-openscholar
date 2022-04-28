<?php

use Behat\Behat\Event\StepEvent;
use Behat\Mink\Driver\Selenium2Driver;
use Drupal\DrupalExtension\Context\DrupalContext;
use Behat\Behat\Context\Step\Given;
use Behat\Gherkin\Node\TableNode;
use Behat\Gherkin\Node\PyStringNode;
use Behat\Behat\Context\Step;
use Behat\Mink\Exception\ElementNotFoundException;

require 'vendor/autoload.php';
require_once 'RestfulTrait.php';

// prevent behat from failing on PHP notification or warning
define("BEHAT_ERROR_REPORTING", E_ALL ^ E_NOTICE ^ E_WARNING);

class FeatureContext extends DrupalContext {

  use RestfulTrait;

  public function beforeScenario($event) {
    // Set up the browser width.

    if ($this->getSession() instanceof Selenium2Driver) {
      $this->getSession()->resizeWindow(1440, 1200, 'current');
    }

    // turn off Mollom CAPTCHA verification
    variable_set('mollom_testing_mode', 1);
    variable_del('mollom_cmp_enabled');

    parent::beforeScenario($event);
  }

  private $currentUrl;

  /**
   * @BeforeStep @javascript
   */
  public function urlChange(StepEvent $e) {
    $this->currentUrl = $this->getSession()->getCurrentUrl();
  }

  /**
   * @AfterStep @javascript
   */
  public function urlChangeHandler(StepEvent $e) {
    if ($this->currentUrl != $this->getSession()->getCurrentUrl()) {
      $this->_captureJavaScriptConsoleErrors();
    }
    $this->_printJavaScriptConsoleErrors();
  }

  private function _captureJavaScriptConsoleErrors() {
    $script = "
    (function () {
      if (!window.BehatScriptRun) {
        window.BehatScriptRun = true;
        window.BehatConsoleErrors = [];

        window.onerror = function (error, url, line) {
          BehatConsoleErrors.push({error: error, url: url, line: line});
        }
      }
    })();
    ";
    $this->getSession()->executeScript($script);
  }

  private function _printJavaScriptConsoleErrors() {
    if ($jserrors = $this->getSession()->evaluateScript("return window.BehatConsoleErrors")) {
      print_r($jserrors);
    }
  }

  /**
   * Variable for storing the random string we used in the text.
   */
  private $randomText;

  /**
   * The box delta we need to hide.
   */
  private $box = array();

  /**
   * Save for later the list of domain we need to remove after a scenario is
   * completed.
   */
  private $domains = array();

  /**
   * Hold the user name and password for the selenium tests for log in.
   */
  private $users;

  /**
   * Hold the NID of the vsite.
   */
  private $nid;

  /**
   * Initializes context.
   *
   * Every scenario gets its own context object.
   *
   * @param array $parameters .
   *   Context parameters (set them up through behat.yml or behat.local.yml).
   */
  public function __construct(array $parameters) {
    if (isset($parameters['drupal_users'])) {
      $this->users = $parameters['drupal_users'];
    }
    else {
      throw new Exception('behat.yml should include "drupal_users" property.');
    }

    if (isset($parameters['vsite'])) {
      $this->nid = $parameters['vsite'];
    }
    else {
      throw new Exception('behat.yml should include "vsite" property.');
    }
  }

  /**
   * Override, since the admin panel screwes this check up
   */
  public function loggedIn() {
    $session = $this->getSession();
    $session->visit($this->locatePath('/'));

    // If a logout link is found, we are logged in. While not perfect, this is
    // how Drupal SimpleTests currently work as well.
    $element = $session->getPage();
    return !$element->findLink('Admin Login');
  }

  /**
   * Authenticates a user with password from configuration.
   *
   * @Given /^I am logging in as "([^"]*)"$/
   */
  public function iAmLoggingInAs($username) {

    try {
      $password = $this->users[$username];
    }
    catch (Exception $e) {
      throw new Exception("Password not found for '$username'.");
    }

    if ($this->loggedIn()) {
      error_log('were logged in. log us out please.');
      $this->logout();
      usleep(500000);
    }

    $element = $this->getSession()->getPage();
    $this->getSession()->visit($this->locatePath('/user'));
    $element->fillField('Username', $username);
    $element->fillField('Password', $password);
    $submit = $element->findButton('Log in');
    $submit->click();
    $this->user = user_load_by_name($username);
    sleep(3);
  }

  /**
   * Authenticates a user with password from configuration.
   *
   * @Given /^I am logging in as "([^"]*)" in the domain "([^"]*)"$/
   */
  public function iAmLoggingInAsInDomain($username, $domain) {

    try {
      $password = $this->users[$username];
    }
    catch (Exception $e) {
      throw new Exception("Password not found for '$username'.");
    }

    if ($this->loggedIn()) {
      $this->logout();
    }

    // Log in.
    // Go to the user page.
    $element = $this->getSession()->getPage();
    $this->getSession()->visit("http://{$domain}/user");
    $element->fillField('Username', $username);
    $element->fillField('Password', $password);
    $submit = $element->findButton('Log in');
    $submit->click();
  }

  /**
   * @Given /^I am logging in as a user who "(can[^"]*)" "([^"]*)"$/
   */
  public function iAmLoggingInAsAUserWho($can, $permission) {
    $can_flag = ($can == "can") ? true : false;

    if ($this->loggedIn() && $can_flag && $this->assertLoggedInWithPermissions($permission)) {
      return true;
    }
    else {
      if ($can_flag) {
        $operator = '=';
      }
      else {
        $operator = '<>';
      }
      $query = db_select('og_users_roles', 'ogur');
      $query->addField('u', 'uid');
      $query->innerJoin('users', 'u', 'u.uid = ogur.uid and u.uid <> 1 and name <> :empty', array(':empty' => "''"));
      $query->innerJoin('og_role_permission', 'ogrp', 'ogrp.rid = ogur.rid and permission ' . $operator . ' :permission', array(':permission' => $permission));
      $query->innerJoin('og_role', 'ogr', 'ogur.rid = ogr.rid');
      $uid = $query->range(0,1)->execute()->fetchField();

      if($uid && $user = user_load($uid)) {
        new Step\When('I am logging in as "' . $user->name . '"');
      }
      else {
        if (!$this->assertAuthenticatedByRole("os report admin")) {
          throw new Exception("Unable to log in as a user who $can $permission.");
        }
      }
    }
  }

  /**
   * @Given /^I create a new "([^"]*)" with title "([^"]*)" in the "([^"]*)" site$/
   */
  public function iCreateANewWithTitleInTheSite ($content_type, $title, $vsite_name) {
    $content_type = str_replace(" ", "_", $content_type);
    $query = new EntityFieldQuery();
    $results = $query->entityCondition('entity_type', 'node')
                    ->propertyCondition('title', $title)
                    ->propertyCondition('type', str_replace('-', '_', $content_type))
                    ->execute();

    if (!empty($results['node'])) {
      FeatureHelp::deleteNode($title);
    }
    $entity = $this->createEntity($content_type, $title);

    // Set the group ref
    $vid = FeatureHelp::getNodeId($vsite_name);
    $wrapper = entity_metadata_wrapper('node', $entity);
    $wrapper->{OG_AUDIENCE_FIELD}->set(array($vid));
    entity_save('node', $entity);
  }

  /**
   * @Given /^I create a revision of "([^"]*)" where I change the "([^"]*)" to "([^"]*)"$/
   */
  public function iCreateARevisionOfWhereIChangeTheTo ($node_title, $field_name, $new_field_value) {
    // grab first row returns when querying DB for node with the passed title
    $query = db_select('node', 'n')
            ->fields('n', array('nid', 'vid'))
            ->condition('n.title', $node_title, '=');
    $results = $query->execute()->fetchAllAssoc('nid');
    $row = array_pop($results);

    $node = node_load($row->nid, $row->vid);

    $node->revision = TRUE;
    $node->is_current = TRUE;
    $node->status = 1;
    $node->log = "setting $field_name to '$new_field_value'";
    $node->revision_moderation = FALSE;
    $node->{$field_name} = $new_field_value;
    $node = node_submit($node);
    node_save($node);
  }

  /**
   * @Then /^I should not be permitted to "([^"]*)" revisions for "([^"]*)"$/
   */
  public function iShouldNotBePermittedToRevisionsFor($action, $node_title) {
    if (!in_array($action, ['Delete', 'Revert'])) {
      throw new Exception('The action ' . $action . ' does not supported by the step.');
    }

    if (node_access('update', FeatureHelp::getNodeId($node_title), $this->user)) {
      throw new Exception('The user is not forbidden from revision page of the node');
    }
  }

  /**
   * @Given /^I revert "([^"]*)" to revision "(\d+)"$/
   */
  public function iRevertToRevision($node_title, $revision_num) {
    $query = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->fields('p', array('id','value'))
      ->condition('n.title', $node_title, '=');
    $query->innerJoin('og_membership', 'ogm', 'ogm.etid = n.nid');
    $query->innerJoin('purl', 'p', 'p.id = ogm.gid');
    $node_rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (!count($node_rows)) {
      throw new Exception(sprintf("Could not find node with title of '%s'.", $node_title));
    }
    $node_row = array_pop($node_rows);
    $url = "/". $node_row['value'] . "/node/" . $node_row['nid'] . "/revisions";
    $this->visit($url);

    $xpath = "//div[@id='content']//table/tbody/tr/td/a[text()='Revert']";
    $elements = $this->getSession()->getPage()->findAll('xpath', $xpath);
    $link = $elements[$revision_num - 1];

    if (!$link) {
      throw new Exception(sprintf("Could not find revision %d for '%s'.", $revision_num, $node_title));
    }
    $link->press();

    // lastly, click the submit button on the confirm modal
    return array(
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @Given /^I delete revision "(\d+)" of "([^"]*)"$/
   */
  public function iDeleteRevisionof($revision_num, $node_title) {
    $query = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->fields('p', array('id','value'))
      ->condition('n.title', $node_title, '=');
    $query->innerJoin('og_membership', 'ogm', 'ogm.etid = n.nid');
    $query->innerJoin('purl', 'p', 'p.id = ogm.gid');
    $node_rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (!count($node_rows)) {
      throw new Exception(sprintf("Could not find node with title of '%s'.", $node_title));
    }
    $node_row = array_pop($node_rows);
    $url = "/". $node_row['value'] . "/node/" . $node_row['nid'] . "/revisions";
    $this->visit($url);

    $xpath = "//div[@id='content']//table/tbody/tr/td/a[text()='Delete']";
    $elements = $this->getSession()->getPage()->findAll('xpath', $xpath);
    $link = $elements[$revision_num - 1];

    if (!$link) {
      throw new Exception(sprintf("Could not find revision %d for '%s'.", $revision_num, $node_title));
    }
    $link->press();

    // lastly, click the submit button on the confirm modal
    return array(
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @Then /^I should not be able to see the "([^"]*)" contextual link for "([^"]*)"$/
   */
  public function iShouldNotBeAbleToSeeTheContextualLink($linktext, $node_title) {
    $query = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->fields('p', array('id','value'))
      ->condition('n.title', $node_title, '=');
    $query->innerJoin('og_membership', 'ogm', 'ogm.etid = n.nid');
    $query->innerJoin('purl', 'p', 'p.id = ogm.gid');
    $node_rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (!count($node_rows)) {
      throw new Exception(sprintf("Could not find node with title of '%s'.", $node_title));
    }
    $node_row = array_pop($node_rows);
    $url = "/". $node_row['value'] . "/node/" . $node_row['nid'];
    $this->visit($url);
    $xpath = "//div[@id='columns']//ul[contains(@class, 'contextual-links')]//a[text()='$linktext']";
    $elements = $this->getSession()->getPage()->findAll('xpath', $xpath);
    if (count($elements)) {
      throw new Exception(sprintf("%s node page contains the '%s' contextual link.", $node_title, $linktext));
    }
  }

  /**
   * @Then /^I should be able to see "(\d+)" revisions for "([^"]*)"$/
   */
  public function iShouldBeAbleToSeeRevisionsFor($number_of_revisions, $node_title) {
    $query = db_select('node', 'n')
      ->fields('n', array('nid'))
      ->fields('p', array('id','value'))
      ->condition('n.title', $node_title, '=');
    $query->innerJoin('og_membership', 'ogm', 'ogm.etid = n.nid');
    $query->innerJoin('purl', 'p', 'p.id = ogm.gid');
    $node_rows = $query->execute()->fetchAll(PDO::FETCH_ASSOC);

    if (!count($node_rows)) {
      throw new Exception(sprintf("Could not find node with title of '%s'.", $node_title));
    }
    $node_row = array_pop($node_rows);

    // check to make sure the "revisions" link is available on the node view page
    $url = "/". $node_row['value'] . "/node/" . $node_row['nid'];
    $this->visit($url);
    $revisions_url = $url . "/revisions";

    $xpath = "//div[@id='columns']//ul[contains(@class, 'contextual-links')]//a[text()='Revisions']";
    $elements = $this->getSession()->getPage()->findAll('xpath', $xpath);
    if (!count($elements)) {
      throw new Exception(sprintf("%s node page does not contain the 'revisions' contextual link.", $node_title, $url));
    }

    // check to see if the proper number of revision rows show up on the revisions page
    $this->visit($revisions_url);
    $action_xpath = "//div[@id='content']//table/tbody/tr/td/a[text()='Revert']";
    $revert_elements = $this->getSession()->getPage()->findAll('xpath', $action_xpath);
    $actual_number_of_revisions = count(node_revision_list(node_load($node_row['nid']))) - 1;

    if ((count($revert_elements) != $actual_number_of_revisions) || ($number_of_revisions != $actual_number_of_revisions)) {
      throw new Exception(sprintf("%s has %d revisions instead of %d.", $node_title, $actual_number_of_revisions, $number_of_revisions));
    }
  }

  /**
   * @Given /^I am on a "([^"]*)" page titled "([^"]*)"(?:, in the tab "([^"]*)"|)$/
   */
  public function iAmOnAPageTitled($page_type, $title, $subpage = NULL) {
    $query = new EntityFieldQuery();
    $results = $query
      ->entityCondition('entity_type', 'node')
      ->propertyCondition('title', $title)
      ->propertyCondition('type', str_replace('-', '_', $page_type))
      ->execute();

    if (empty($results['node'])) {
      throw new \Exception("No $page_type with title '$title' was found.");
    }

    $ids = array_keys($results['node']);
    $id = reset($ids);

    return new Given("I am at \"node/$id\"");
  }

  /**
   * @Then /^I should see "([^"]*)" element with the class "([^"]*)"$/
   */
  public function iShouldSeeElementWithTheClass($tag, $class) {
    $page = $this->getSession()->getPage();

    $element = $page->find('css', "$tag.$class");
    if (!$element) {
      throw new Exception(sprintf("%s element with the class %s was not found.", $tag, $class));
    }
  }

  /**
   * @Then /^I should not see "([^"]*)" element with the class "([^"]*)"$/
   */
  public function iShouldNotSeeElementWithTheClass($tag, $class) {
    $page = $this->getSession()->getPage();

    $element = $page->find('css', "$tag.$class");
    if ($element) {
      throw new Exception(sprintf("%s element with the class %s was found.", $tag, $class));
    }
  }

  /**
   * @Given /^I should see the "([^"]*)" table with the following <contents>:$/
   */
  public function iShouldSeeTheTableWithTheFollowingContents($class, TableNode $table) {
    $page = $this->getSession()->getPage();
    $table_element = $page->find('css', "table.$class");
    if (!$table_element) {
      throw new Exception("A table with the class $class wasn't found");
    }

    $hash = $table->getRows();
    // Iterate over each row, just so if there's an error we can supply
    // the row number, or empty values.
    foreach ($hash as $vals) {
      $xpath_fragments = array();
      foreach ($vals as $v) {
        $xpath_fragments[] = 'td//text()[contains(.,"'.$v.'") and not(ancestor::*[contains(@class, "ng-hide")])]';
      }
      $xpath = '//tr['.implode(' and ', $xpath_fragments).']';
      if (!$table_element->findAll('xpath', $xpath)) {
        error_log($xpath);
        throw new Exception("Row with the following values not found: ".implode(', ', $vals));
      }
    }
  }

  /**
   * @Then /^I should get:$/
   */
  public function iShouldGet(PyStringNode $string) {
    $page = $this->getSession()->getPage();
    $compare_string = $string->getRaw();
    $page_string = $page->getContent();

    if (strpos($compare_string, '{{*}}')) {
      // Attributes that may changed in different environments.
      foreach (array('sourceUrl', 'id', 'value', 'href', 'os_version') as $attribute) {
        $page_string = preg_replace('/ '. $attribute . '=".+?"/', '', $page_string);
        $compare_string = preg_replace('/ '. $attribute . '=".+?"/', '', $compare_string);

        // Dealing with JSON.
        $page_string = preg_replace('/"'. $attribute . '":".+?"/', '', $page_string);
        $compare_string = preg_replace('/"'. $attribute . '":".+?"/', '', $compare_string);
      }

      if ($page_string != $compare_string) {
        $output = "The strings are not matching.\n";
        $output .= "Page: {$page_string}\n";
        $output .= "Search: {$compare_string}\n";
        throw new Exception($output);
      }
    }
    else {
      // Normal compare.
      foreach (explode("\n", $compare_string) as $text) {
        if (strpos($page_string, $text) === FALSE) {
          throw new Exception(sprintf('The text "%s" was not found.', $text));
        }
      }
    }
  }

  /**
   * @When /^I clear the cache$/
   */
  public function iClearTheCache() {
    drupal_flush_all_caches();
  }

  /**
   * @Then /^I should print page$/
   */
  public function iShouldPrintPage() {
    $element = $this->getSession()->getPage();
    $url = $this->createGist($element->getContent());
    print_r('You asked to see the page content. Here is a gist contain the html: ' . $url . "\n");

    // Make sure the temp directory exists and is writable before using it
    $tmpdir = sys_get_temp_dir() . DIRECTORY_SEPARATOR . 'screenshots';
    if (! file_exists($tmpdir)) {
      mkdir($tmpdir, "0777", true);
    }

    $this->iShouldPrintPageTo($tmpdir . DIRECTORY_SEPARATOR . time() . '.txt');
    $driver = $this->getSession()->getDriver();
    $screenshot = $driver->getScreenshot();
    $gistUrl = $this->createGist('<img src="data:image/png;base64,'.base64_encode($screenshot).'">');
    $this->iPrintPageScreenShot();
    print_r("Here is a screenshot of the page: $gistUrl\n");
  }

  /**
   * Creating public gist.
   *
   * @param array $file
   *   List of files and the content.
   */
  public function createGist($file) {
    $request = $this->invokeRestRequest('post', 'https://api.github.com/gists', [], [
      'description' => 'http log',
      'public' => TRUE,
      'files' => ['file.html' => ['content' => $file]]]
    );
    $json = $request->json();

    error_log($json['files']['file.html']['raw_url']);
    return $json['files']['file.html']['raw_url'];
  }

  /**
   * @Then /^I should print page to "([^"]*)"$/
   */
  public function iShouldPrintPageTo($file) {
    $element = $this->getSession()->getPage();
    file_put_contents($file, $element->getContent());
  }

  /**
   * @Then /^I should see the images:$/
   */
  public function iShouldSeeTheImages(TableNode $table) {
    $page = $this->getSession()->getPage();
    $table_rows = $table->getRows();
    foreach ($table_rows as $rows) {
      $image = $page->find('xpath', "//img[contains(@src, '{$rows[0]}')]");
      if (!$image) {
        throw new Exception(sprintf('The image "%s" was not found in the page.', $rows[0]));
      }
    }
  }

  /**
   * @Given /^I drag&drop "([^"]*)" to "([^"]*)"$/
   */
  public function iDragDropTo($element, $destination) {
    $selenium = $this->getSession()->getDriver();
    $selenium->evaluateScript("jQuery('#{$element}').detach().prependTo('#{$destination}');");
  }

  /**
   * @Given /^I verify the element "([^"]*)" under "([^"]*)"$/
   */
  public function iVerifyTheElementUnder($element, $container) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//*[contains(@id, '{$container}')]//div[contains(@id, '{$element}')]");

    if (!$element) {
      throw new Exception(sprintf("The element with %s wasn't found in %s", $element, $container));
    }
  }

  /**
   * @Given /^I should see that the "([^"]*)" in the "([^"]*)" are collapsed$/
   */
  public function iShouldSeeTheItemsInTheAre($type, $location) {
    switch ($location) {
      case 'LOP':
        $id = 'block-boxes-os-' . $type . '-sv-list';
        break;
    }

    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//*[contains(@id, '{$id}')]//*[contains(@class, 'expanded')]");

    if ($element) {
      throw new Exception(sprintf("Some elements with %s are not collapsed", $location));
    }
  }

  /**
   * @Given /^a node of type "([^"]*)" with the title "([^"]*)" exists in site "([^"]*)"$/
   */
  public function assertNodeTypeTitleVsite($type, $title, $site = 'john') {
    return array(
      new Step\When('I visit "' . $site . '/node/add/' . $type . '"'),
      new Step\When('I fill in "Title" with "'. $title . '"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @Given /^I start creating a post of type "([^"]*)" in site "([^"]*)"$/
   */
  public function iStartCreatingPostOfType($type, $site) {
    return array(
      new Step\When('I visit "'. $site . '/node/add/' . $type . '"')
    );
  }

  /**
   * @Given /^I create a new publication$/
   */
  public function iCreateANewPublication() {
    return array(
      new Step\When('I visit "john/node/add/biblio"'),
      new Step\When('I select "Book" from "Publication Type"'),
      new Step\When('I press "edit-biblio-next"'),
      new Step\When('I fill in "Title" with "'. time() . '"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @When /^I create a new "([^"]*)" with title "([^"]*)" in the site "([^"]*)"$/
   */
  public function iCreateANewEventWithTitle($type, $title, $vsite) {
    $tomorrow = time() + (24 * 60 * 60);
    $entity = $this->createEntity($type, $title);
    $entity->field_date['und'][0]['value'] = date('Y-m-d H:i:s');
    $entity->field_date['und'][0]['value2'] = date('Y-m-d H:i:s', $tomorrow);
    $wrapper = entity_metadata_wrapper('node', $entity);

    // Set the group ref
    $nid = FeatureHelp::getNodeId($vsite);
    $wrapper->{OG_AUDIENCE_FIELD}->set(array($nid));
    entity_save('node', $entity);
  }

  /**
   * @When /^I create an upcoming event with title "([^"]*)" in the site "([^"]*)"$/
   */
  public function iCreateAnUpcomingEventWithTitle($title, $vsite) {
    $tomorrow = time() + (24 * 60 * 60);
    $entity = $this->createEntity("event", $title);
    $entity->field_date['und'][0]['value'] = date('Y-m-d H:i:s', $tomorrow);
    $entity->field_date['und'][0]['value2'] = date('Y-m-d H:i:s', $tomorrow + 360);
    $wrapper = entity_metadata_wrapper('node', $entity);

    // Set the group ref
    $nid = FeatureHelp::getNodeId($vsite);
    $wrapper->{OG_AUDIENCE_FIELD}->set(array($nid));
    entity_save('node', $entity);
  }

  /**
   * @When /^I create a new repeating event with title "([^"]*)" that repeats "([^"]*)" times$/
   */
  public function iCreateANewRepeatingEventWithTitle($title, $times) {
    $tomorrow = time() + (24 * 60 * 60);
    return array(
      new Step\When('I visit "john/node/add/event"'),
      new Step\When('I fill in "Title" with "' . $title . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value-datepicker-popup-0" with "' . date('M j Y', $tomorrow) . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value2-datepicker-popup-0" with "' . date('M j Y', $tomorrow) . '"'),
      new Step\When('I check the box "edit-field-date-und-0-all-day"'),
      new Step\When('I check the box "edit-field-date-und-0-show-repeat-settings"'),
      new Step\When('I fill in "edit-field-date-und-0-rrule-count-child" with "' . $times . '"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @When /^I create a new repeating event with title "([^"]*)" on "([^"]*)" that repeats "([^"]*)" times and repeats "([^"]*)" with Repeat on "([^"]*)"$/
   */
  public function iCreateANewRepeatingEventWithTitleThatOnDateRepeatsTimesAndRepeatsWithRepeatOn($title, $date, $times, $frequently, $onDay) {
    switch ($frequently) {
      case 'Daily':
        $frequentlyValue = 'DAILY';
        break;
      case 'Yearly':
        $frequentlyValue = 'YEARLY';
        break;
      case 'Monthly':
        $frequentlyValue = 'MONTHLY';
        break;
      case 'Weekly':
      default:
        $frequentlyValue = 'WEEKLY';
        break;
    }
    $onDayCheckbox = '';
    switch ($onDay) {
      case 'Sun':
        $onDayCheckbox = 'edit-field-date-und-0-rrule-weekly-byday-su';
        break;
      case 'Mon':
        $onDayCheckbox = 'edit-field-date-und-0-rrule-weekly-byday-mo';
        break;
      case 'Tue':
        $onDayCheckbox = 'edit-field-date-und-0-rrule-weekly-byday-tu';
        break;
      case 'Wed':
        $onDayCheckbox = 'edit-field-date-und-0-rrule-weekly-byday-we';
        break;
      case 'Thu':
        $onDayCheckbox = 'edit-field-date-und-0-rrule-weekly-byday-th';
        break;
      case 'Fri':
        $onDayCheckbox = 'edit-field-date-und-0-rrule-weekly-byday-fr';
        break;
      case 'Sat':
        $onDayCheckbox = 'edit-field-date-und-0-rrule-weekly-byday-sa';
        break;
    }
    $steps = array(
      new Step\When('I visit "john/node/add/event"'),
      new Step\When('I fill in "Title" with "' . $title . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value-datepicker-popup-0" with "' . date('M j Y', strtotime($date)) . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value2-datepicker-popup-0" with "' . date('M j Y', strtotime($date)) . '"'),
      new Step\When('I check the box "edit-field-date-und-0-all-day"'),
      new Step\When('I check the box "edit-field-date-und-0-show-repeat-settings"'),
      new Step\When('I check the box "Signup"'),
      new Step\When('I fill in "edit-field-date-und-0-rrule-freq" with "' . $frequentlyValue . '"'),
      new Step\When('I fill in "edit-field-date-und-0-rrule-count-child" with "' . $times . '"'),
    );
    if (!empty($onDayCheckbox)) {
      $steps[] = new Step\When('I check the box "' . $onDayCheckbox . '"');
    }
    $steps[] = new Step\When('I press "edit-submit"');
    return $steps;
  }

  /**
   * @When /^I create a new registration event with title "([^"]*)"$/
   */
  public function iCreateANewRegistrationEventWithTitle($title) {
    $tomorrow = time() + (24 * 60 * 60);
    return array(
      new Step\When('I visit "john/node/add/event"'),
      new Step\When('I fill in "Title" with "' . $title . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value-datepicker-popup-0" with "' . date('M j Y', $tomorrow) . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value2-datepicker-popup-0" with "' . date('M j Y', $tomorrow) . '"'),
      new Step\When('I check the box "edit-field-date-und-0-all-day"'),
      new Step\When('I check the box "field_event_registration[und][0][registration_type]"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @When /^I create a new repeating registration event in site "([^"]*)" with title "([^"]*)" that repeats "([^"]*)" times$/
   */
  public function iCreateANewRepeatingRegistrationEventWithTitle($site, $title, $times) {
    $tomorrow = time() + (24 * 60 * 60);
    return array(
      new Step\When('I visit "'.$site.'/node/add/event"'),
      new Step\When('I fill in "Title" with "' . $title . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value-datepicker-popup-0" with "' . date('M j Y', $tomorrow) . '"'),
      new Step\When('I fill in "edit-field-date-und-0-value2-datepicker-popup-0" with "' . date('M j Y', $tomorrow) . '"'),
      new Step\When('I check the box "edit-field-date-und-0-all-day"'),
      new Step\When('I check the box "edit-field-date-und-0-show-repeat-settings"'),
      new Step\When('I fill in "edit-field-date-und-0-rrule-count-child" with "' . $times . '"'),
      new Step\When('I check the box "field_event_registration[und][0][registration_type]"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @When /^I should see the event "([^"]*)" in the LOP$/
   */
  public function iShouldSeeTheEventInTheLop($title) {
    $page = $this->getSession()->getPage()->getContent();

    $pattern = "/<div id='boxes-box-os_events_upcoming' class='boxes-box'>[\s\S]*" . $title . "[\s\S]*<\/div>/";
    if (!preg_match($pattern, $page)) {
      throw new Exception("The event '$title' was not found in the List of posts widget.");
    }
  }

  /**
   * @When /^I should see the date of the "([^"]*)" repeat of the event$/
   */
  public function iShouldSeeTheDateOfTheRepeatOfDate($repeat) {
    $two_weeks_from_tmr = time() + ($repeat * 7 + 1) * (24 * 60 * 60);
    return array(
      new Step\When('I should see "' . date('l, F j, Y', $two_weeks_from_tmr) .'"'),
    );
  }

  /**
   * @When /^I should see the date of "([^"]*)" on repeat of the event$/
   */
  public function iShouldSeeTheDateOfRepeatDate($date) {
    return array(
      new Step\When('I should see "' . date('l, F j, Y', strtotime($date)) .'"'),
    );
  }

  /**
   * @When /^I create a new "([^"]*)" entry with the name "([^"]*)"$/
   */
  public function iCreateANewEntryWithTheName($type, $name) {
    $node = new stdClass();
    $node->title = $name;
    $node->type = $type;
    node_save($node);
    $this->visit('john/node/' . $node->nid);
  }

  /**
   * @Given /^I create a sub page named "([^"]*)" under the page "([^"]*)"$/
   */
  public function iCreateSubPageUnderPage($child_title, $parent_title) {
    $nid = FeatureHelp::getNodeId($parent_title);
    return array(
      new Step\When('I visit "john/node/add/page?parent_node=' . $nid . '&destination=node/' . $nid . '"'),
      new Step\When('I fill in "Title" with "' . $child_title . '"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @When /^I create a new "([^"]*)" entry with the name "([^"]*)" in the group "([^"]*)"$/
   */
  public function iCreateANewEntryWithTheNameInGroup($type, $name, $group) {
    $node = new stdClass();
    $node->title = $name;
    $node->type = $type;
    $node->{OG_AUDIENCE_FIELD}[LANGUAGE_NONE][0]['target_id'] = FeatureHelp::getNodeId($group);
    node_save($node);
    $this->visit('john/node/' . $node->nid);
  }

  /**
   * @When /^I change privacy of the site "([^"]*)" to "([^"]*)"$/
   */
  public function iChangePrivacyTo($vsite, $visibility) {

    $privacy_level = array(
      'Public on the web.' => 0,
      'Anyone with the link.' => 2,
      'Site members only.' => 1,
    );

    return array(
      new Step\When('I visit "' . $vsite . '"'),
      new Step\When('I open the admin panel to "Settings"'),
      new Step\When('I open the admin panel to "Global Settings"'),
      new Step\When('I scroll in the ".menu-container .simplebar-scroll-content" element until I find "Site Visibility"'),
      new Step\When('I click on the "Site Visibility" control'),
      new Step\When('I click on the "' . trim($visibility) . '" control'),
      new Step\When('I press "Save"'),
      new Step\When('I wait for page actions to complete'),
    );
  }

  /**
   * @Then /^I should verify the node "([^"]*)" not exists$/
   */
  public function iShouldVerifyTheNodeNotExists($title) {
    $nid = FeatureHelp::getNodeId($title);
    FeatureHelp::deleteNode($title);

    $this->Visit('I visit "john/node/' . $nid . '"');

    return array(
      new Step\When('I should not get a "200" HTTP response'),
    );
  }

  /**
   * @Given /^I add a comment "([^"]*)" using the comment form$/
   */
  public function iAddACommentUsingTheCommentForm($comment) {
    return array(
      new Step\When('I fill in "Comment" with "' . $comment . '"'),
      new Step\When('I press "Save"'),
    );
  }

  /**
   * @Given /^I create a "([^"]*)" widget for the vsite "([^"]*)" with the following <settings>:$/
   */
  public function iCreateAWidgetWithTheFollowingSettingsForTheVsite($widget, $vsite, TableNode $table) {
    $metasteps = [];
    switch (strtolower($widget)) {
      case "custom html":
        $widgetType = "os_boxes_html";
        break;
      case "list of posts":
        $widgetType = "os_sv_list_box";
        break;
      case "embed media":
        $widgetType = "os_boxes_media";
        break;
      case "feed reader":
        $widgetType = "os_boxes_feedreader";
        break;
      case "slideshow":
        $widgetType = "os_slideshow_box";
        break;
      case "dataverse list":
        $widgetType = "os_boxes_dataverse_list";
        break;
      case "dataverse search box":
        $widgetType = "os_boxes_dataverse_search";
        break;
      case "dataverse dataset citation":
        $widgetType = "os_boxes_dataverse_dataset_citation";
        break;
      case "dataverse dataset":
        $widgetType = "os_boxes_dataverse_dataset";
        break;
    }
    $metasteps[] = new Step\When('I visit "/' . $vsite . '/os/widget/add/' . $widgetType . '/cp-layout"');
    $hash = $table->getRows();

    print "\n" . 'I visit "/' . $vsite . '#overlay=' . $vsite . '/os/widget/add/' . $widgetType . '/cp-layout"' . "\n";

    foreach ($hash as $form_elements) {
      switch ($form_elements[2]) {
        case 'select list':
          $values = explode(",", $form_elements[1]);

          if (count($values) > 1) {
            foreach ($values as $value) {
              // Select multiple values from the terms options.
              $this->getSession()
                ->getPage()
                ->selectFieldOption($form_elements[0], trim($value), TRUE);
            }
          }
          else {
            $metasteps[] = new Step\When('I select "' . $form_elements[1] . '" from "' . $form_elements[0] . '"');
          }
          break;
        case 'checkbox':
          $metasteps[] = new Step\When('I ' . $form_elements[1] . ' the box "' . $form_elements[0] . '"');
          break;
        case 'textfield':
          $metasteps[] = new Step\When('I fill in "' . $form_elements[0] . '" with "' . $form_elements[1] . '"');
          break;
        case 'radio':
          $metasteps[] = new Step\When('I select the radio button "' . $form_elements[0] . '" with the id "' . $form_elements[1] . '"');
          break;
      }
    }

    if ($widgetType != "os_slideshow_box") {
      // Skip Make Embeddable step for slide show widget
      $metasteps[] = new Step\When('I check the box "edit-make-embeddable"');
    }
    $metasteps[] = new Step\When('I make sure admin panel is closed');
    $metasteps[] = new Step\When('I press "Save"');
    return $metasteps;
  }

  /**
   * @Given /^I drag the "([^"]*)" widget to the "([^"]*)" region$/
   */
  public function iDragTheWidgetToTheRegion($widget_label, $region_name) {

    $widget_name_label_map = array(
      "Active book TOC"                        => "boxes-box-active-book-toc",
      "All Posts"                              => "boxes-box-all-posts",
      "Blog RSS Feed"                          => "boxes-blog_rss_feed",
      "Contact"                                => "boxes-hwp_personal_contact_html",
      "Filter by taxonomy for pages"           => "boxes-vocabulary_filter_pages",
      "Filter by term"                         => "boxes-box-filter-by-term",
      "Front page header text"                 => "boxes-iqss_scholars_fp_headertext",
      "HWP Option Info text"                   => "boxes-iqss_scholars_fp_hwp_option",
      "Latest News"                            => "boxes-os_news_latest",
      "Latest Publications"                    => "boxes-boxes-os_boxes_feedreader",
      "List of posts"                          => "boxes-box-list-of-posts",
      "Recent FAQs"                            => "boxes-os_faq_sv_list",
      "Recent Images"                          => "boxes-os_image_gallery_latest",
      "Recent Presentations"                   => "boxes-os_presentations_recent",
      "Recent Publications"                    => "boxes-os_publications_recent",
      "Scholars Info text with video link"     => "boxes-iqss_scholars_fp_infoblock",
      "Scholars Learn More Box"                => "boxes-iqss_scholars_fp_learnmore",
      "Scholars Learn More Toggle Page"        => "boxes-iqss_scholars_learnmore_toggle",
      "Scholars Logo"                          => "boxes-iqss_scholars_fp_logoblock",
      "Scholars fixed-position header."        => "boxes-iqss_scholars_fixed_header",
      "Search box"                             => "boxes-solr_search_box",
      "Site RSS Feed"                          => "boxes-os_rss",
      "Subscribe to MailChimp mailing list"    => "boxes-os_box_mailchimp",
      "Upcoming Events"                        => "boxes-os_events_upcoming",
      "Active Book's TOC"                      => "boxes-os_booktoc",
      "AddThis"                                => "boxes-os_addthis",
      "Blog Archive"                           => "views-os_blog-block",
      "Blog RSS Feed"                          => "boxes-blog_rss_feed",
      "Contact"                                => "boxes-hwp_personal_contact_html",
      "Filter News by Month"                   => "views-os_news-news_by_month_block",
      "Filter News by Year"                    => "views-os_news-news_by_year_block",
      "Filter Profiles by Alphabetical Groups" => "views-os_profiles-filter_by_alphabet",
      "Filter by taxonomy for pages"           => "boxes-vocabulary_filter_pages",
      "Front page header text"                 => "boxes-iqss_scholars_fp_headertext",
      "Google Translate"                       => "os_ga-google_translate",
      "HWP Option Info text"                   => "boxes-iqss_scholars_fp_hwp_option",
      "Latest Publications"                    => "boxes-boxes-os_boxes_feedreader",
      "Mini Calendar"                          => "views-os_events-block_1",
      "Primary Menu"                           => "os-primary-menu",
      "Recent Documents"                       => "boxes-os_booklets_recent_docs",
      "Recent FAQs"                            => "boxes-os_faq_sv_list",
      "Recent Images"                          => "boxes-os_image_gallery_latest",
      "Recent Presentations"                   => "boxes-os_presentations_recent",
      "Recent Publications"                    => "boxes-os_publications_recent",
      "Recent Software Releases"               => "views-os_software_releases-block_1",
      "Scholars Info text with video link"     => "boxes-iqss_scholars_fp_infoblock",
      "Scholars Learn More Box"                => "boxes-iqss_scholars_fp_learnmore",
      "Scholars Learn More Toggle Page"        => "boxes-iqss_scholars_learnmore_toggle",
      "Scholars Logo"                          => "boxes-iqss_scholars_fp_logoblock",
      "Scholars"                               => "boxes-iqss_scholars_fixed_header",
      "Search box"                             => "boxes-solr_search_box",
      "Site RSS Feed"                          => "boxes-os_rss",
      "Subscribe to MailChimp mailing list"    => "boxes-os_box_mailchimp",
      "Upcoming Events"                        => "boxes-os_events_upcoming",
    );

    $region_names = array(
      "header-first",
      "header-second",
      "header-third",
      "menu-bar",
      "sidebar-first",
      "content",
      "content-top",
      "content-first",
      "content-second",
      "content-bottom",
      "sidebar-second",
      "footer-first",
      "footer",
      "footer-third",
      "footer-bottom",
    );

    if (! in_array($region_name, $region_names)) {
      throw new Exception("I do not recognize the region name: $region_name.");
    }

    $css_selector = $widget_name_label_map[$widget_label];
    $widget_icon = $this->getSession()->getPage()->find('css', "div#$css_selector");
    if (! $widget_icon) {
      throw new Exception("I could not find a widget for '$widget_label'.");
    }

    $region_element = $this->getSession()->getPage()->find('css', "div#edit-layout-$region_name");
    if (! $region_element) {
      throw new Exception("I could not find a region for '$region_name'.");
    }
    $widget_icon->dragTo($region_element);

    $save_button = $this->getSession()->getPage()->find('css', "input#edit-submit");
    if (! $save_button) {
      throw new Exception("I could not find a save button using css selector 'input#edit-submit'.");
    }

    $save_button->click();
  }

  /**
   * @Given /^I click the big gear$/
   */
  public function iClickTheBigGear() {
    $big_gear = $this->getSession()->getPage()->find('css', "a.ctools-dropdown-link.ctools-dropdown-text-link");
    if (! $big_gear) {
      throw new Exception("I did not locate the big gear icon.");
    }
    $big_gear->click();
  }

  /**
   * @Given /^the widget "([^"]*)" is placed in the "([^"]*)" layout$/
   */
  public function theWidgetIsPlacedInTheLayout($widget, $page) {

    $page_mapping = array(
      'News' => 'news_news',
      'Blog' => 'blog_blog',
      'Link' => 'links_links',
      'Reader' => 'reader_reader',
      'Calendar' => 'events_events',
      'Classes' => 'classes_classes',
      'People' => 'profiles_profiles',
      'Galleries' => 'gallery_gallery',
      'FAQ' => 'faq_faq',
      'Software' => 'software_software',
      'Documents' => 'booklets_booklets',
      'Publications' => 'publications_publications',
      'Presentations' => 'presentations_presentations',
    );

    $q = db_select('spaces_overrides', 'so')
      ->fields('so', array('object_id', 'id'))
      ->condition('value', '%s:5:"title";s:' . strlen($widget) . ':"' . $widget . '";%', 'LIKE')
      ->condition('object_type', 'boxes', '=');
    $results = $q->execute()->fetchAll();
    $row = array_pop($results);
    $vsite = spaces_load('og', $row->id);

    $page_id = FeatureHelp::GetNodeId($page);

    if (array_key_exists($page, $page_mapping)) {
      $blocks = $vsite->controllers->context->get($page_mapping[$page] . ":reaction:block");
      $blocks['blocks']['boxes-' . $row->object_id] = array(
        'module' => 'boxes',
        'delta' => $row->object_id,
        'title' => $widget,
        'region' => 'sidebar_second',
        'status' => 0,
        'weight' => 0
      );
      $vsite->controllers->context->set($page_mapping[$page] . ":reaction:block", $blocks);
    } else {
      $blocks = $vsite->controllers->context->get('os_pages-page-' . $page_id . ":reaction:block");
      $blocks['blocks']['boxes-' . $row->object_id] = array(
        'module' => 'boxes',
        'delta' => $row->object_id,
        'title' => $widget,
        'region' => 'sidebar_second',
        'status' => 0,
        'weight' => 0
      );
      $vsite->controllers->context->set('os_pages-page-' . $page_id . ":reaction:block", $blocks);
    }
  }

  /**
   * @Given /^the dataverse widget "([^"]*)" is placed in the "([^"]*)" layout$/
   */
  public function theDataverseWidgetIsPlacedInTheLayout($widget, $page) {
    $q = db_select('spaces_overrides', 'so')
      ->fields('so', array('object_id', 'id'))
      ->condition('value', '%s:5:"title";s:' . strlen($widget) . ':"' . $widget . '";%', 'LIKE')
      ->condition('object_type', 'boxes', '=');
    $results = $q->execute()->fetchAll();
    $row = array_pop($results);

    $page_id = FeatureHelp::GetNodeId($page);

    $vsite = spaces_load('og', $row->id);
    $blocks = $vsite->controllers->context->get('os_pages-page-' . $page_id . ":reaction:block");
    $blocks['blocks']['boxes-' . $row->object_id] = array(
      'module' => 'boxes',
      'delta' => $row->object_id,
      'title' => $widget,
      'region' => 'content_first',
      'status' => 0,
      'weight' => 0
    );
    $vsite->controllers->context->set('os_pages-page-' . $page_id . ":reaction:block", $blocks);

  }

  /**
   * @Given /^the widget "([^"]*)" is set in the "([^"]*)" page with the following <settings>:$/
   */
  public function theWidgetIsSetInThePageWithSettings($widget, $page, TableNode $table) {
    return $this->theWidgetIsSetInThePageByTheNameWithSettings($widget, $page, '', $table);
  }

  /**
   * @Given /^the widget "([^"]*)" is set in the "([^"]*)" page by the name "([^"]*)" with the following <settings>:$/
   */
  public function theWidgetIsSetInThePageByTheNameWithSettings($widget, $page, $name, TableNode $table) {
    $this->box[] = FeatureHelp::setBoxInRegion($this->nid, $widget, $page, 'sidebar_second', $name);
    $hash = $table->getRows();

    list($box, $delta, $context) = explode(",", $this->box[0]);

    $metasteps = array();
    $this->visit(FeatureHelp::getVsitePurl($this->nid) . '/os/widget/boxes/' . $delta . '/edit');

    // @TODO: Use XPath to fill the form instead of giving the type of the in
    // the scenario input.
    foreach ($hash as $form_elements) {
      switch ($form_elements[2]) {
        case 'select list':
          $values = explode(",", $form_elements[1]);

          if (count($values) > 1) {
            foreach ($values as $value) {
              // Select multiple values from the terms options.
              $this->getSession()->getPage()->selectFieldOption($form_elements[0], trim($value), true);
            }
          }
          else {
            $metasteps[] = new Step\When('I select "' . $form_elements[1] . '" from "'. $form_elements[0] . '"');
          }
          break;
        case 'checkbox':
          $metasteps[] = new Step\When('I '. $form_elements[1] . ' the box "' . $form_elements[0] . '"');
          break;
        case 'textfield':
          $metasteps[] = new Step\When('I fill in "' . $form_elements[0] . '" with "1"');
          break;
        case 'radio':
          $metasteps[] = new Step\When('I select the radio button "' . $form_elements[0] . '" with the id "' . $form_elements[1] . '"');
          break;
      }
    }

    $metasteps[] = new Step\When('I press "Save"');

    return $metasteps;
  }

  /**
   * @Given /^the widget "([^"]*)" is set in the "([^"]*)" page$/
   */
  public function theWidgetIsSetInThePage($widget, $page) {
    $this->box[] = FeatureHelp::setBoxInRegion($this->nid, $widget, $page);
  }

  /**
   * @When /^I assign the node "([^"]*)" to the term "([^"]*)"$/
   */
  public function iAssignTheNodeToTheTerm($node, $term) {
    FeatureHelp::assignNodeToTerm($node, $term);
  }

  /**
   * @When /^I assign the page "([^"]*)" to the term "([^"]*)"$/
   */
  public function iAssignThePageToTheTerm($node, $term) {
    FeatureHelp::assignNodeToTerm($node, $term, 'page');
  }

  /**
   * @Given /^I unassign the node "([^"]*)" from the term "([^"]*)"$/
   */
  public function iUnassignTheNodeFromTheTerm($node, $term) {
    FeatureHelp::unassignNodeFromTerm($node, $term);
  }

  /**
   * @Given /^I unassign the node "([^"]*)" with the type "([^"]*)" from the term "([^"]*)"$/
   */
  public function iUnassignTheNodeWithTheTypeFromTheTerm($node, $type, $term) {
    FeatureHelp::unassignNodeFromTerm($node, $term, $type);
  }

  /**
   * @Given /^I assign the node "([^"]*)" with the type "([^"]*)" to the term "([^"]*)"$/
   */
  public function iAssignTheNodeWithTheTypeToTheTerm($node, $type, $term) {
    FeatureHelp::assignNodeToTerm($node, $term, $type);
  }

  /**
   * Hide the boxes we added during the scenario.
   *
   * @AfterScenario
   */
  public function afterScenario($event) {
    if (!empty($this->box)) {
      // Loop over the box we collected in the scenario, hide them and delete
      // them.
      foreach ($this->box as $box_handler) {
        $data = explode(',', $box_handler);
        foreach ($data as &$value) {
          $value = trim($value);
        }
        FeatureHelp::hideBox($this->nid, $data[0], $data[1], $data[2]);
      }
    }

    if (!empty($this->domains)) {
      // Remove domain we added to vsite.
      foreach ($this->domains as $domain) {
        FeatureHelp::RemoveVsiteDomain($domain);
      }
    }


    // make sure no support user has subscribed to a site after a scenario is over
    $original = variable_get('vsite_support_expire', '3 days');
    variable_set('vsite_support_expire', '1 sec');
    vsite_cron();
    variable_set('vsite_support_expire', $original);
  }

  /**
   * @Given /^cache is "([^"]*)" for anonymous users$/
   */
  public function cacheIsForAnonymousUsers($status) {
    variable_set('cache', $status == "enabled");
  }

  /**
   * @Then /^response header "([^"]*)" should be "([^"]*)"$/
   */
  public function responseHeaderShouldBe($key, $result) {
    $headers = $this->getSession()->getResponseHeaders();
    if (!empty($headers[$key]) && $headers[$key][0] == $result) {
      return;
    }
    // Depending on the HTTP server, keys might be lowercase even if they
    // where uppercase. To avoid failures we try to look for the same key
    // but lowercase before we decide it is missing.
    $lowercase_key = strtolower($key);
    if (!empty($headers[$lowercase_key]) && $headers[$lowercase_key][0] == $result) {
      return;
    }
    throw new Exception(sprintf('The "%s" key in the response header is "%s" instead of the expected "%s".', $key, $headers[$key][0], $result));
  }

  /**
   * @Then /^response header "([^"]*)" should not be "([^"]*)"$/
   */
  public function responseHeaderShouldNotBe($key, $result) {
    $headers = $this->getSession()->getResponseHeaders();
    if (!empty($headers[$key]) && $headers[$key][0] == $result) {
      throw new Exception(sprintf('The "%s" key in the response header should not be "%s", but it is.', $key, $headers[$key][0]));
    }
  }

  /**
   * @When /^I create the vocabulary "([^"]*)" in the group "([^"]*)" assigned to bundle "([^"]*)"$/
   */
  public function iCreateVocabInGroup($vocab_name, $group, $bundle) {
    return array(
      new Step\When('I visit "' . $group . '/cp/build/taxonomy/add"'),
      new Step\When('I fill in "Name" with "' . $vocab_name . '"'),
      new Step\When('I select "' . $bundle . '" from "Content types"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @Given /^I create the term "([^"]*)" in vocabulary "([^"]*)"$/
   */
  public function iCreateTheTermInVocab($term_name, $vocab_name) {
    FeatureHelp::CreateTerm($term_name ,$vocab_name);
  }

  /**
   * @Given /^I delete the term "([^"]*)"$/
   */
  public function iDeleteTheTermInVocab($term_name) {
    FeatureHelp::DeleteTerm($term_name);
  }

  /**
   * @Given /^I should see the following <links>$/
   */
  public function iShouldNotSeeTheFollowingLinks(TableNode $table) {
    $page = $this->getSession()->getPage();
    $hash = $table->getRows();

    foreach ($hash as $i => $table_row) {
      if (empty($table_row)) {
        continue;
      }
      $element = $page->find('xpath', "//a[.='{$table_row[0]}']");

      if (empty($element)) {
        throw new Exception(printf("The link %s wasn't found on the page", $table_row[0]));
      }
    }
  }

  /**
   * @Then /^I should see CKEDITOR in "([^"]*)"$/
   */
  public function iShouldSeeTinemceIn($field) {
    $page = $this->getSession()->getPage();
    $iframe = $page->find('xpath', "//label[contains(., '{$field}')]//..//iframe[contains(@class, 'cke')]");

    if (!$iframe) {
      throw new Exception("tinyMCE wysiwyg does not appear.");
    }
  }

  /**
   * @Given /^I sleep for "([^"]*)"$/
   */
  public function iSleepFor($sec) {
    sleep($sec);
  }

  /**
   * @Then /^I should see the following <json>:$/
   */
  public function iShouldSeeTheFollowingJson(TableNode $table) {
    // Get the json output and decode it.
    $json_output = $this->getSession()->getPage()->getContent();
    $json = json_decode($json_output);

    // Hasing table, and define variables for later.
    $hash = $table->getRows();

    // Run over the tale and start matching between the values of the JSON and
    // the user input.
    foreach ($hash as $i => $table_row) {
      if (isset($json->{$table_row[0]})) {
        if ($json->{$table_row[0]} != $table_row[1]) {
          $error['values'][$table_row[0]] = ' Not equal to ' . $table_row[1];
        }
      }
      else {
        $error['not_found'][$table_row[0]] = " Dosen't exists.";
      }
    }

    // Build the error string if needed.
    if (!empty($error)) {
      $string = array();

      if (!empty($error['values'])) {
        foreach ($error['values'] as $variable => $message) {
          $string[] = '  ' . $variable . $message;
        }
      }

      if (!empty($error['not_found'])) {
        foreach ($error['not_found'] as $variable => $message) {
          $string[] = '  ' . $variable . $message;
        }
      }

      throw new Exception("Some errors were found:\n" . implode("\n", $string));
    }
  }

  /**
   * @Then /^I should see the following message <json>:$/
   */
  public function iShouldSeeTheFollowingMessageJson(TableNode $table) {
    // Get the json output and decode it.
    $json_output = $this->getSession()->getPage()->getText();
    $json = json_decode($json_output);

    // Hashing table, and define variables for later.
    $hash = $table->getRows();

    if (isset($json->data)) {
      foreach ($json->data->messages as $message) {
        $error = array();
        foreach ($hash as $table_row) {
          if (isset($message->arguments->{$table_row[0]})) {
            if ($message->arguments->{$table_row[0]} != $table_row[1]) {
              $error['values'][$table_row[0]] = ' not equal to ' . $table_row[1];
            }
          }
          else {
            $error['not_found'][$table_row[0]] = " doesn't exist.";
          }
        }
        if (empty($error)) {
          break;
        }
      }
    }
    else {
      $error = "No messages were found.";
    }

    // Build the error string if needed.
    if (!empty($error)) {
      $string = array();

      if (!empty($error['values'])) {
        foreach ($error['values'] as $variable => $message) {
          $string[] = '  ' . $variable . $message;
        }
      }
      if (!empty($error['not_found'])) {
        foreach ($error['not_found'] as $variable => $message) {
          $string[] = '  ' . $variable . $message;
        }
      }

      if (is_string($error)) {
        $string[] = $error;
      }

      throw new Exception("Some errors were found:\n" . implode("\n", $string));
    }
  }

  /**
   * @Then /^I should not see the following message <json>:$/
   */
  public function iShouldNotSeeTheFollowingMessageJson(TableNode $table) {
    // Get the json output and decode it.
    $json_output = $this->getSession()->getPage()->getText();
    $json = json_decode($json_output);

    // Hashing table, and define variables for later.
    $hash = $table->getRows();

    if (isset($json->data)) {
      foreach ($json->data as $message) {
        $error = array();
        foreach ($hash as $table_row) {
          if (isset($message->arguments->{$table_row[0]})) {
            if ($message->arguments->{$table_row[0]} != $table_row[1]) {
              $error['values'][$table_row[0]] = ' not equal to ' . $table_row[1];
            }
          }
          else {
            $error['not_found'][$table_row[0]] = " doesn't exist.";
          }
        }
        if (empty($error)) {
          break;
        }
      }
    }
    else {
      $error = "No messages were found.";
    }

    if (empty($error)) {
      throw new Exception("Message with the given properties appear on the page when it shouldn't have");
    }
    elseif (is_string($error)) {
      throw new Exception("{$error}");
    }
  }

  /**
   * Generate random text.
   */
  private function randomizeMe($length = 10) {
    return $this->randomText = substr(str_shuffle("abcdefghijklmnopqrstuvwxyz"), 0, $length);
  }

  /**
   * @Given /^I fill "([^"]*)" with random text$/
   */
  public function iFillWithRandomText($elementId) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//input[@id='{$elementId}']");

    if (!$element) {
      throw new Exception(sprintf("Could not find the element with the id %s", $elementId));
    }

    $element->setValue($this->randomizeMe());
  }

  /**
   * @Given /^I visit the site "([^"]*)"$/
   */
  public function iVisitTheSite($site) {
    if ($site == "random") {
      $this->visit("/" . $this->randomText);
    }
    else {
      $this->visit("/" . $site);
    }
  }

  /**
   * @Given /^I execute vsite cron$/
   */
  public function iExecuteVsiteCron() {
    vsite_cron();
  }

  /**
   * @Given /^I set the term "([^"]*)" under the term "([^"]*)"$/
   */
  public function iSetTheTermUnderTheTerm($child, $parent) {
    FeatureHelp::setTermUnderTerm($child, $parent);
  }

  /**
   * @When /^I set the variable "([^"]*)" to "([^"]*)"$/
   */
  public function iSetTheVariableTo($variable, $value) {
    FeatureHelp::variableSet($variable, $value);
  }

  /**
   * @When /^I delete the variable "([^"]*)"$/
   */
  public function iDeleteVariable($variable) {
    variable_del($variable);
  }

  /**
   * @Then /^I should see a pager$/
   */
  public function iShouldSeeAPager() {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//div[@class='item-list']");

    if (!$element) {
      throw new Exception("The pager wasn't found.");
    }
  }

  /**
   * @Then /^I should see the options "([^"]*)" under "([^"]*)"$/
   */
  public function iShouldSeeOptions($options, $container) {
    $options = explode(',',$options);

    $element = FALSE;
    $page = $this->getSession()->getPage();
    foreach ($options as $option) {
      $element = $page->find('xpath', "//select[@name='{$container}']//option[contains(.,'{$option}')]");
      if (!$element) {
        break;
      }
    }

    if (!$element) {
      throw new Exception("The option {$option} is missing.");
    }
  }

  /**
   * @Given /^I set courses to import$/
   */
  public function iSetCoursesToImport() {
    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('name', 'Harvard Graduate School of Design')
      ->range(0, 1)
      ->execute();

    print_r($result);

    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->execute();

    print_r($result);

    $entity = entity_create('field_collection_item', array('field_name' => 'field_department_school'));
    $entity->setHostEntity('node', node_load(FeatureHelp::getNodeId('john')));
    $wrapper = entity_metadata_wrapper('field_collection_item', $entity);
    $wrapper->field_department_id->set('Architecture');
    $wrapper->field_school_name->set(reset($result['taxonomy_term'])->tid);
    $wrapper->save();

    $metasteps = array();
    $metasteps[] = new Step\When('I visit "admin"');
    $metasteps[] = new Step\When('I visit "admin/structure/feeds/course/settings/HarvardFetcher"');
    $metasteps[] = new Step\When('I check the box "Debug mode"');
    $metasteps[] = new Step\When('I press "Save"');
    return $metasteps;
  }

  /**
   * @When /^I enable harvard courses$/
   */
  public function iEnableHarvardCourses() {
    $this->visit('os-import-demo/enable/harvard');
  }

  /**
   * @Given /^I refresh courses$/
   */
  public function iRefreshCourses() {
    FeatureHelp::ImportCourses();
  }

  /**
   * @Given /^I remove harvard courses$/
   */
  public function iRemoveHarvardCourses() {
    FeatureHelp::RemoveCourses();
    $this->iSleepFor(2);
  }

  /**
   * @Given /^I add the courses$/
   */
  public function iAddTheCourses() {
    FeatureHelp::AddCourses();
    $this->iSleepFor(2);
  }

  /**
   * @Given /^I invalidate cache$/
   */
  public function iInvalidateCache() {
    cache_clear_all('*', 'cache_views_data', TRUE);
  }

  /**
   * @Given /^I populate in "([^"]*)" with "([^"]*)"$/
   */
  public function iPopulateInWith($field, $url) {
    $url = str_replace('LOCALHOST', $this->locatePath(''), $url);

    return array(
      new Step\When('I fill in "' . $field . '" with "' . $url . '"'),
    );
  }

  /**
   * @Given /^I should be redirected in the following <cases>:$/
   */
  public function iShouldBeRedirectedInTheFollowingCases(TableNode $table) {
    $rows = $table->getRows();
    $baseUrl = $this->locatePath('');

    if (count(reset($rows)) == 3) {
      foreach ($rows as $row) {
        $this->visit($row[0]);
        $url = $this->getSession()->getCurrentUrl();

        if ($url != $baseUrl . $row[2] && $url != 'http://lincoln.local/' . $row[2]) {
          throw new Exception("When visiting {$row[0]} we did not redirected to {$row[2]} but to {$url}.");
        }

        $john_response_code = $this->responseCode($baseUrl . $row[0]);
        $lincoln_response_code = $this->responseCode('http://lincoln.local/' . $row[0]);
        if ($john_response_code != $row[1] && $lincoln_response_code != $row[1]) {
          throw new Exception("When visiting {$row[0]} we did not get a {$row[1]} response code but the {$john_response_code}/{$lincoln_response_code} response code.");
        }
      }
    }
    else {
      foreach ($rows as $row) {
        $nid = FeatureHelp::GetNodeId($row[1]);

        if ($row[2] == 'No') {
          $VisitUrl = 'node/' . $nid;
        }
        else {
          $VisitUrl = drupal_get_path_alias('node/{$nid}');
        }

        if (!empty($row[0])) {
          $VisitUrl = $row[0] . $VisitUrl;
        }

        $this->visit($VisitUrl);
        $url = $this->getSession()->getCurrentUrl();

        if ($url != $baseUrl . $row[4]) {
          throw new Exception("When visiting {$VisitUrl} we did not redirected to {$row[4]} but to {$url}.");
        }

        $response_code = $this->responseCode($baseUrl . $VisitUrl);
        if ($response_code != $row[3]) {
          throw new Exception("When visiting {$VisitUrl} we did not get a {$row[3]} response code but the {$response_code} response code.");
        }
      }
    }
  }

  /**
   * Get the response code for a URL.
   *
   *  @param $address
   *    The URL address.
   *
   *  @return
   *    The response code for the URL address.
   */
  function responseCode($address) {
    $ch = curl_init($address);

    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HEADER, 1); // Return header.
    curl_setopt($ch, CURLOPT_NOBODY, 1); // Will not return the body.

    curl_exec($ch);
    $curlInfo = curl_getinfo($ch);
    curl_close($ch);

    return $curlInfo['http_code'];
  }

  /**
   * @Then /^I should see the random string$/
   */
  public function iShouldSeeTheRandomString() {
    $metasteps = array(new Step\When('I should see "' . $this->randomText . '"'));
    return $metasteps;
  }

  /**
   * @When /^I search for "([^"]*)"$/
   */
  public function iSearchFor($item) {
    return array(
      new Step\When('I fill in "search_block_form" with "'. $item . '"'),
      new Step\When('I press "Search"'),
    );
  }

  /**
   * @When /^I search for "([^"]*)" in the site "([^"]*)"$/
   */
  public function iSearchForInSite($item, $site) {
    return array(
      new Step\When('I visit "' . $site . '"'),
      new Step\When('I fill in "search_block_form" with "' . $item . '"'),
      new Step\When('I press "Search"'),
    );
  }

  /**
   * @When /^I add to the search results the sites "([^"]*)"$/
   */
  public function iAddToSearchResultsSites($sites) {
    $sites = explode(',', $sites);
    $site_ids = array();
    foreach ($sites as $site) {
      $site_ids[] = FeatureHelp::GetNodeId($site);
    }
    FeatureHelp::VariableSet('os_search_solr_search_sites', array(implode(',', $site_ids)));
  }

  /**
   * @When /^I add to the search results the site's subsites$/
   */
  public function iAddToSearchResultsSubsites() {
    FeatureHelp::VariableSet('os_search_solr_include_subsites', TRUE);
  }

  /**
   * @Then /^I verify the "([^"]*)" term link redirect to the original page$/
   */
  public function iVerifyTheTermLinkRedirectToTheOriginalPage($term) {
    $tid = FeatureHelp::GetTermId($term);

    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//a[contains(., '{$term}')]");

    if (strpos($element->getAttribute('href'), 'taxonomy/term/') !== FALSE) {
      throw new exception("The term {$term} linked us to his original path(taxonomy/term/{$tid})");
    }
  }

  /**
   * @Given /^I verify the "([^"]*)" term link doesn\'t redirect to the original page$/
   */
  public function iVerifyTheTermLinkDoesnTRedirectToTheOriginalPage($term) {
    $tid = FeatureHelp::GetTermId($term);

    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//a[contains(., '{$term}')]");

    if (strpos($element->getAttribute('href'), 'taxonomy/term/') === FALSE) {
      throw new exception("The term {$term} linked us to his original path(taxonomy/term/{$tid})");
    }
  }

  /**
   * @Given /^I should not see "([^"]*)" under "([^"]*)"$/
   */
  public function iShouldNotSeeUnder($text, $id) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//input[@id='{$id}']//*[contains(.,'{$text}')]");
    if ($element) {
      throw new Exception("The text {$text} found under #{$id}");
    }
  }

  /**
   * @Then /^I should verify i am at "([^"]*)"$/
   */
  public function iShouldVerifyIAmAt($given_url) {
    $url = $this->getSession()->getCurrentUrl();
    $base_url = $startUrl = rtrim($this->getMinkParameter('base_url'), '/') . '/';

    $path = str_replace($base_url, '', $url);

    $path_fragments = explode('?', $path);

    if ($given_url != $path_fragments[0]) {
      throw new Exception("The given url: '{$given_url}' is not equal to the current path {$path_fragments[0]}");
    }
  }

  /**
   * @Given /^I should see the meta tag "([^"]*)" with value "([^"]*)"$/
   */
  public function iShouldSeeTheMetaTag($tag, $value) {
    $page = $this->getSession()->getPage();
    if (!$text = $page->find('xpath', "//meta[(@property='{$tag}' or @name='{$tag}') and @content='{$value}']")) {
      throw new Exception("The meta tag {$tag} value is not {$value}");
    }
  }

  /**
   * @Given /^I should see the text "([^"]*)" under "([^"]*)"$/
   */
  public function iShouldSeeTheTextUnder($text, $container) {
    if (!$this->searchForTextUnderElement($text, $container)) {
      throw new Exception(sprintf("The element with %s wasn't found in %s", $text, $container));
    }
  }

  /**
   * @Then /^I should not see the text "([^"]*)" under "([^"]*)"$/
   */
  public function iShouldNotSeeTheTextUnder($text, $container) {
    if ($this->searchForTextUnderElement($text, $container)) {
      throw new Exception(sprintf("The element with %s was found in %s", $text, $container));
    }
  }

  /**
   * Searching text under an element with class
   */
  private function searchForTextUnderElement($text, $container) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//*[contains(@class, '{$container}')]//*[contains(., '{$text}')]");
    return $element;
  }

  /**
   * @Given /^I should see the link "([^"]*)" under "([^"]*)"$/
   */
  public function iShouldSeeTheLinkUnder($text, $container) {
    if (!$this->searchForLinkUnderElement($text, $container)) {
      throw new Exception(sprintf("The link %s wasn't found in %s", $text, $container));
    }
  }

  /**
   * @Then /^I should not see the link "([^"]*)" under "([^"]*)"$/
   */
  public function iShouldNotSeeTheLinkUnder($text, $container) {
    if ($this->searchForLinkUnderElement($text, $container)) {
      throw new Exception(sprintf("The link %s was found in %s", $text, $container));
    }
  }

  /**
   * Searching a link under an element with class
   */
  private function searchForLinkUnderElement($text, $container) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//*[contains(@class, '{$container}')]//a[.='{$text}']");

    return $element;
  }

  /**
   * @Given /^I give the user "([^"]*)" the role "([^"]*)" in the group "([^"]*)"$/
   */
  public function iGiveTheUserTheRoleInTheGroup($name, $role, $group) {
    $uid = FeatureHelp::GetUserByName($name);

    return array(
      new Step\When('I visit "' . $group . '/cp/users/add"'),
      new Step\When('I fill in "edit-name" with "' . $name . '"'),
      new Step\When('I press "Add member"'),
      new Step\When('I sleep for "5"'),
      new Step\When('I visit "' . $group . '/cp/users/edit_membership/' . $uid . '"'),
      new Step\When('I sleep for "5"'),
      new Step\When('I select the radio button named "edit_role" with value "' . $role . '"'),
      new Step\When('I press "Save"'),
    );
  }

  /**
   * @Given /^I give the role "([^"]*)" in the group "([^"]*)" the permission "([^"]*)"$/
   */
  public function iGiveTheRoleThePermissionInTheGroup($role, $group, $permission) {
    $nid = FeatureHelp::idFromPath($group);
    $rid = FeatureHelp::GetRoleByName($role, $nid);

    return array(
      new Step\When('I visit "' . $group . '/group/node/' . $nid . '/admin/permission/' . $rid . '/edit"'),
      new Step\When('I check the box "' . $permission . '"'),
      new Step\When('I press "Save permissions"'),
    );
  }

  /**
   * @Given /^I remove from the role "([^"]*)" in the group "([^"]*)" the permission "([^"]*)"$/
   */
  public function iRemoveTheRoleThePermissionInTheGroup($role, $group, $permission) {
    $nid = FeatureHelp::GetNodeId($group);
    $rid = FeatureHelp::GetRoleByName($role, $nid);

    return array(
      new Step\When('I visit "' . $group . '/group/node/' . $nid . '/admin/permission/' . $rid . '/edit"'),
      new Step\When('I uncheck the box "edit-' . $rid . '-' . $permission . '"'),
      new Step\When('I press "Save permissions"'),
    );
  }

  /**
   * @Then /^I should verify that the user "([^"]*)" has a role of "([^"]*)" in the group "([^"]*)"$/
   */
  public function iShouldVerifyThatTheUserHasRole($name, $role, $group) {
    $user_has_role = FeatureHelp::checkUserRoleInGroup($name, $role, $group);

    if ($user_has_role == 0) {
      throw new Exception("The user {$name} is not a member in the group {$group}");
    }
    elseif ($user_has_role == 1) {
      throw new Exception("The user {$name} doesn't have the role {$role} in the group {$group}");
    }
  }

  /**
   * @When /^I select the radio button named "([^"]*)" with value "([^"]*)"$/
   */
  public function iSelectRadioNamedWithValue($name, $value) {
    $page = $this->getSession()->getPage();
    $radiobutton = $page->find('xpath', "//*[@name='{$name}'][@value='{$value}']");
    if (!$radiobutton) {
      throw new Exception("A radio button with the name {$name} and value {$value} was not found on the page");
    }
    $radiobutton->selectOption($value, FALSE);
  }

  /**
   * @When /^I select the radio button under "([^"]*)" with a label containing "([^"]*)"$/
   */
  public function iSelectRadioButtonUnderWithALabelContaining($under_label, $label_containing) {
    $page = $this->getSession()->getPage();

    $radiobutton = $page->find('xpath', "//label[starts-with(text(), '$under_label')]/..//div[contains(text(), '$label_containing')]/input");

    if (!$radiobutton) {
      throw new Exception("A radio button with the name {$name} and value {$value} was not found on the page");
    }
    $radiobutton->selectOption(true, FALSE);
  }

  /**
   * @When /^I select the "([^"]*)" button with value "([^"]*)"$/
   */
  public function iSelectTypedButtonWithValueXyz($button_type, $button_value) {
    $page = $this->getSession()->getPage();
    $radiobutton = $page->find('xpath', "//input[@type='$button_type'][@value='$button_value']");
    if (!$radiobutton) {
      throw new Exception("A '$button_type' button with the value {$button_value} was not found on the page.");
    }
    $radiobutton->selectOption($button_value, FALSE);
  }



  /**
   * @When /^I choose the radio button named "([^"]*)" with value "([^"]*)" for the vsite "([^"]*)"$/
   */
  public function iSelectRadioNamedWithValueForVsite($name, $value, $vsite) {
    $page = $this->getSession()->getPage();
    $radiobutton = $page->find('xpath', "//*[@name='{$name}'][@value='{$value}']");
    if (!$radiobutton) {
      throw new Exception("A radio button with the name {$name} and value {$value} was not found on the page");
    }
    $radiobutton->selectOption($value, FALSE);
    $option = $radiobutton->getValue();
    FeatureHelp::VsiteSetVariable($vsite, $name, $option);
  }

  /**
   * @When /^I visit the original page for the term "([^"]*)"$/
   */
  public function iVisitTheOriginalPageForTheTerm($term) {
    $tid = FeatureHelp::GetTermId($term);
    $this->getSession()->visit($this->locatePath('taxonomy/term/' . $tid));
  }

  /**
   * @Given /^I wait for page actions to complete$/
   */
  public function waitForPageActionsToComplete() {
    // Waits 5 seconds i.e. for any javascript actions to complete.
    // @todo configure selenium for JS, see step 6 of the following link.
    // @see http://xavierriley.co.uk/blog/2012/10/12/test-driving-prestashop-with-behat/
    $duration = 5000;
    $this->getSession()->wait($duration);
  }

  /**
   * @Given /^I set the event capacity to "([^"]*)"$/
   */
  public function iSetTheEventCapacityTo($capacity) {
    return array(
      new Step\When('I click "Manage Registrations"'),
      new Step\When('I click on link "Settings" under "main-content-header"'),
      new Step\When('I fill in "edit-capacity" with "' . $capacity . '"'),
      new Step\When('I press "Save Settings"'),
    );
  }

  /**
   * @Given /^I click on link "([^"]*)" under "([^"]*)"$/
   */
  public function iClickOnLinkUnder($link, $container) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//*[contains(@id, '{$container}')]//a[contains(., '{$link}')]");
    $element->press();
  }

  /**
   * @Given /^I click on "([^"]*)" under facet "([^"]*)"$/
   */
  public function iClickOnLinkInFacet($option, $facet) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//h2[contains(., '{$facet}')]/following-sibling::div//a[contains(., '{$option}')]");

    if (!$element) {
      throw new Exception(sprintf("'%s' was not found under the facet '%s'", $option, $facet));
    }

    $element->press();
  }

  /**
   * @Then /^I delete "([^"]*)" registration$/
   */
  public function iDeleteRegistration($arg1) {
    return array(
      new Step\When('I am not logged in'),
      new Step\When('I am logging in as "john"'),
      new Step\When('I visit "john/event/halleys-comet"'),
      new Step\When('I click "Manage Registrations"'),
      new Step\When('I click "Delete"'),
      new Step\When('I press "Delete"'),
    );
  }

  /**
   * @Given /^I turn on event registration on "([^"]*)"$/
   */
  public function iTurnOnEventRegistrationOn($location) {
    return $this->eventRegistrationChangeStatus($location);
  }

  /**
   * @Given /^I turn off event registration on "([^"]*)"$/
   */
  public function iTurnOffEventRegistrationOn($location) {
    return $this->eventRegistrationChangeStatus($location);
  }

  /**
   * Change the event registration status.
   */
  private function eventRegistrationChangeStatus($title) {
    $nid = FeatureHelp::GetNodeId($title);
    return array(
      new Step\When('I visit "node/' . $nid . '/edit"'),
      new Step\When('I check the box "Signup"'),
      new Step\When('I press "Save"'),
    );
  }

  /**
   * @Given /^no boxes display outside the site context$/
   */
  function noBoxesDisplayOutsideTheSiteContext() {
    // Runs a test of loading all existing boxes and checking if they have
    // output.
    // @todo ideally we would actually create a box of each kind and test each.
    $error = _os_boxes_test_load_all_boxes_outside_vsite_context();
    if ($error) {
      throw new Exception(sprintf("At least one box returned output outside of a vsite: %s", $error));
    }
  }

  /**
   * @When /^I edit the node "([^"]*)"$/
   */
  public function iEditTheNode($title) {
    $nid = FeatureHelp::GetNodeId($title);

    $purl = FeatureHelp::GetNodeVsitePurl($nid);
    $purl = !empty($purl) ? $purl . '/' : '';

    return array(
      new Step\When('I visit "' . $purl . 'node/' . $nid . '/edit"'),
    );
  }

  /**
   * @When /^I edit the node "([^"]*)" in the group "([^"]*)"$/
   */
  public function iEditTheNodeInGroup($title, $group) {
    $nid = FeatureHelp::getNodeIdInVsite($title, $group);
    $purl = FeatureHelp::GetNodeVsitePurl($nid);
    $purl = !empty($purl) ? $purl . '/' : '';
    $page = $purl . 'node/' . $nid . '/edit';

    try {
      $this->visit($page);
    } catch (\Exception $e) {
      print_r('An error: ' . $e->getMessage());
      print_r('page: ' . $page);
    }
  }

  /**
   * @When /^I edit the node of type "([^"]*)" named "([^"]*)" using contextual link$/
   */
  public function iEditTheNodeOfTypeNamedUsingContextualLink($type, $title) {
    $nid = FeatureHelp::GetNodeId($title);
    return array(
      new Step\When('I visit "node/' . $nid . '/edit?destination=' . $type . '"'),
    );
  }

  /**
   * @When /^I delete the node of type "([^"]*)" named "([^"]*)"$/
   */
  public function iDeleteTheNodeOfTypeNamedUsingContextualLink($type, $title) {
    $nid = FeatureHelp::GetNodeId($title);
    return array(
      new Step\When('I visit "node/' . $nid . '/delete?destination=' . $type . '"'),
      new Step\When('I press "Delete"'),
    );
  }

  /**
   * @When /^I delete the node of type "([^"]*)" named "([^"]*)" in the group "([^"]*)"$/
   */
  public function iDeleteTheNodeOfTypeNamedInGroup($type, $title, $group) {
    $nid = FeatureHelp::GetNodeIdInVsite($title, $group);
    return array(
      new Step\When('I visit "' . $group . '/node/' . $nid . '/delete?destination=' . $type . '"'),
      new Step\When('I press "Delete"'),
    );
  }

  /**
   * @Then /^I verify the "([^"]*)" value is "([^"]*)"$/
   */
  public function iVerifyTheValueIs($label, $value) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//label[contains(.,'{$label}')]/following-sibling::input[@value='{$value}']");

    if (empty($element)) {
      throw new Exception(sprintf("The element '%s' did not has the value: %s", $label, $value));
    }
  }

  /**
   * @Given /^I am adding the subtheme "([^"]*)" in "([^"]*)"$/
   */
  public function iAmAddingTheSubthemeIn($subtheme, $vsite) {
    FeatureHelp::AddSubtheme($subtheme, $vsite);
  }

  /**
   * @When /^I defined the "([^"]*)" as the theme of "([^"]*)"$/
   */
  public function iDefinedTheAsTheThemeOf($subtheme, $vsite) {
    FeatureHelp::DefineSubtheme($subtheme, $vsite, 1);
  }

  /**
   * @Given /^I define the subtheme "([^"]*)" of the theme "([^"]*)" as the theme of "([^"]*)"$/
   */
  public function iDefineTheSubthemeOfTheThemeAsTheThemeOf($subtheme, $theme, $vsite) {
    FeatureHelp::DefineSubtheme($theme, $subtheme, $vsite);
  }

  /**
   * @Then /^I should verify the existence of the css "([^"]*)"$/
   */
  public function iShouldVerifyTheExistenceOfTheCss($asset) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//link[contains(@href, '{$asset}')]");

    if (!$element) {
      throw new Exception(sprintf("The CSS asset %s wasn't found.", $asset));
    }
  }

  /**
   * @Given /^I should verify the existence of the js "([^"]*)"$/
   */
  public function iShouldVerifyTheExistenceOfTheJs($asset) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//script[contains(@src, '{$asset}')]");

    if (!$element) {
      throw new Exception(sprintf("The JS asset %s wasn't found.", $asset));
    }
  }

  /**
   * @Given /^I set feed item to import$/
   */
  public function iSetFeedItemToImport() {
    return array(
      new Step\When('I visit "admin"'),
      new Step\When('I visit "admin/structure/feeds/os_reader/settings/OsFeedFetcher"'),
      new Step\When('I check the box "Debug mode"'),
      new Step\When('I press "Save"'),
    );
  }

  /**
   * @Given /^I import feed items for "([^"]*)"$/
   */
  public function iImportFeedItemsFor($vsite) {
    global $base_url;
    $nid = FeatureHelp::GetNodeId($vsite);
    $url = $base_url . '/' . drupal_get_path('module', 'os_migrate_demo') . '/includes/' . $vsite . '_dummy_rss.xml';
    FeatureHelp::ImportFeedItems($url, $nid);
  }

  /**
   * @Given /^I import "([^"]*)" feed items for "([^"]*)"$/
   */
  public function iImportVsiteFeedItemsForVsite($vsite_origin, $vsite_target) {
    $nid = FeatureHelp::GetNodeId($vsite_target);
    FeatureHelp::ImportFeedItems($this->locatePath('os-reader/' . $vsite_origin), $nid);
  }

  /**
   * @Given /^I import the feed item "([^"]*)"$/
   */
  public function iImportTheFeedItem($feed_item) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//td[contains(., '{$feed_item}')]//..//td//a[contains(., 'Import')]");

    if (!$element) {
      throw new Exception(sprintf("The feed item %s wasn't found or it's already imported.", $feed_item));
    }

    $element->click();
  }

  /**
   * @Given /^I go to the "([^"]*)" app settings in the vsite "([^"]*)"$/
   */
  public function iGoToTheAppSettingsInVsite($app_name, $vsite) {
    return array(
      new Step\When('I visit "' . $vsite . '/cp/build/features/' . $app_name . '"'),
    );
  }

  /**
   * @Then /^I change site title to "([^"]*)" in the site "([^"]*)"$/
   */
  public function iChangeSiteTitleTo($title, $vsite) {
    $nid = FeatureHelp::idFromPath($vsite);
    $node = node_load($nid);
    $node->title = $title;
    node_save($node);
  }

  /**
   * @Then /^I should see the feed item "([^"]*)" was imported$/
   */
  public function iShouldSeeTheFeedItemWasImported($feed_item) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//td[contains(., '{$feed_item}')]//..//td//a[contains(., 'Edit')]");

    if (!$element) {
      throw new Exception(sprintf("The feed item %s was not found or is already imported.", $feed_item));
    }

    $element->click();
  }

  /**
   * @Then /^I should see the news photo "([^"]*)"$/
   */
  public function iShouldSeeTheNewsPhoto($image_name) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//div[contains(@class, 'field-name-field-photo')]//figure//img[contains(@src, '{$image_name}')]");

    if (!$element) {
      $this->iShouldPrintPage();
      throw new Exception(sprintf("The feed item's image %s was not imported into field_photo.", $image_name));
    }
  }

  /**
   * @Given /^I display watchdog$/
   */
  public function iDisplayWatchdog() {
    $watchdog = FeatureHelp::DisplayWatchdogs();
    $url = $this->createGist(implode("\n", $watchdog));
    print_r('The watch dog url is: ' . $url . "\n");
  }

  /**
   * @When /^I login as "([^"]*)" in "([^"]*)"$/
   */
  public function iLoginAsIn($username, $site) {
    $nid = FeatureHelp::GetNodeId($site);

    try {
      $password = $this->users[$username];
    } catch (Exception $e) {
      throw new Exception("Password not found for '$username'.");
    }

    return array(
      new Step\When('I visit "node/' . $nid .'"'),
      new Step\When('I click "Admin Login"'),
      new Step\When('I fill in "Username" with "' . $username . '"'),
      new Step\When('I fill in "Password" with "' . $password . '"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @Given /^I set the Share domain name to "([^"]*)"$/
   */
  public function iSetTheShareDomainNameTo($value) {
    $action = $value ? 'I checked "edit-vsite-domain-shared"' : 'I uncheck "edit-vsite-domain-shared"';
    return array(
      new Step\When('I open the admin panel to "Settings"'),
      new Step\When('I open the admin panel to "Global Settings"'),
      new Step\When('I click on the "Custom Domain" control'),
      new Step\When($action),
      new Step\When('I press "Save"'),
    );
  }

  /**
   * @Given /^I import the blog for "([^"]*)"$/
   */
  public function iImportTheBlogFor($vsite) {
    $nid = FeatureHelp::GetNodeId($vsite);
    FeatureHelp::ImportFeedItems($this->locatePath('os-reader/' . $vsite . '_blog'), $nid, 'blog');
  }

  /**
   * @Given /^I bind the content type "([^"]*)" with "([^"]*)"$/
   */
  public function iBindTheContentTypeWithIn($bundle, $vocabulary) {
    FeatureHelp::BindContentToVocab($bundle, $vocabulary);
  }

  /**
   * @Then /^I should find the text "([^"]*)" in the file$/
   *
   * Defining a new step because when using the step "I should see" for the iCal
   * page the test is failing.
   */
  public function iShouldFindTheTextInTheFile($string) {
    $element = $this->getSession()->getPage();

    if (strpos($element->getContent(), $string) === FALSE) {
      throw new Exception("the string '$string' was not found.");
    }
  }

  /**
   * @Then /^I should not find the text "([^"]*)" in the file$/
   *
   */
  public function iShouldNotFindTheTextInTheFile($string) {
    $element = $this->getSession()->getPage();

    if (strpos($element->getContent(), $string) !== FALSE) {
      throw new Exception("the string '$string' was found.");
    }
  }

  /**
   * @When /^I edit the term "([^"]*)"$/
   */
  public function iEditTheTerm($name) {
    $tid = FeatureHelp::GetTermId($name);
    $purl = FeatureHelp::GetTermVsitePurl($tid);
    $purl = !empty($purl) ? $purl . '/' : '';

    return array(
      new Step\When('I visit "' . $purl . 'taxonomy/term/' . $tid . '/edit"'),
    );
  }

  /**
   * @Then /^I verify the alias of node "([^"]*)" is "([^"]*)"$/
   */
  public function iVerifyTheAliasOfNodeIs($title, $alias) {
    $nid = FeatureHelp::GetNodeId($title);
    $actual_alias = FeatureHelp::GetNodeAlias($nid);

    if ($actual_alias != $alias) {
      throw new Exception("The alias of the node '$title' should be '$alias', but is '$actual_alias' instead.");
    }
  }

  /**
   * @Then /^I verify the alias of term "([^"]*)" is "([^"]*)"$/
   */
  public function iVerifyTheAliasOfTermIs($name, $alias) {
    $tid = FeatureHelp::GetTermId($name);
    $actual_alias = FeatureHelp::GetTermAlias($tid);

    if ($actual_alias != $alias) {
      throw new Exception("The alias of the term '$name' should be '$alias', but is '$actual_alias' instead.");
    }
  }

  /**
   * @Then /^I should see the publication "([^"]*)" comes before "([^"]*)"$/
   */
  public function iShouldSeeThePublicationComesBefore($first, $second) {
    $page = $this->getSession()->getPage()->getContent();

    $pattern = '/<div class="biblio-category-section">[\s\S]*' . $first . '[\s\S]*' . $second . '[\s\S]*<\/div><div class="biblio-category-section">/';
    if (!preg_match($pattern, $page)) {
      throw new Exception("The publication '$first' does not come before the publication '$second'.");
    }
  }

  /**
   * @Then /^I should see the publication "([^"]*)" comes before "([^"]*)" in the LOP widget$/
   */
  public function iShouldSeeThePublicationComesBeforeLopWidget($first, $second) {
    $page = $this->getSession()->getPage()->getContent();

    $pattern = "/<div id='boxes-box-box-list-of-publications' class='boxes-box'>[\s\S]*" . $first . "[\s\S]*" . $second . "[\s\S]*<\/div>/";
    if (!preg_match($pattern, $page)) {
      throw new Exception("The publication '$first' does not come before the publication '$second'.");
    }
  }

  /**
   * @Then /^I should see the FAQ "([^"]*)" comes before "([^"]*)"$/
   */
  public function iShouldSeeTheFaqComesBefore($first, $second) {
    $page = $this->getSession()->getPage()->getContent();

    $pattern = '/<div class="view-content">[\s\S]*' . $first . '[\s\S]*' . $second . '[\s\S]*<\/section>/';
    if (!preg_match($pattern, $page)) {
      throw new Exception("The FAQ '$first' does not come before the FAQ '$second'.");
    }
  }

  /**
   * @Given /^I define "([^"]*)" domain to "([^"]*)"$/
   */
  public function iDefineDomainTo($vsite, $domain) {
    $this->domains[] = $vsite;
    $nid = FeatureHelp::getNodeId($vsite);
    if ($group = vsite_get_vsite($nid)) {
      $group->controllers->variable->set('vsite_domain_name', $domain);
      $group->controllers->variable->set('vsite_domain_shared', $domain);
    }
  }

  /**
   * @Given /^I verify the url is "([^"]*)"$/
   */
  public function iVerifyTheUrlIs($url) {
    if (strpos($this->getSession()->getCurrentUrl(), $url) === FALSE) {
      throw new Exception(sprintf("Your are not in the url %s but in %s", $url, $this->getSession()->getCurrentUrl()));
    }
  }

  /**
   * @Given /^I make the node "([^"]*)" sticky$/
   */
  public function iMakeTheNodeSticky($title) {
    $nid = FeatureHelp::GetNodeId($title);
    FeatureHelp::MakeNodeSticky($nid);
  }

  /**
   * @Then /^I should see the button "([^"]*)"$/
   */
  public function iShouldSeeTheButton($button) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//input[@type='submit' or @type='button'][@value='$button' or @id='$button' or @name='$button']");

    if (!$element) {
      throw new Exception("Could not find a button with id|name|value equal to '$button'");
    }
  }

  /**
   * @Then /^I should verify the next week calendar is displayed correctly$/
   */
  public function iShouldVerifyNextWeekDisplayed() {
    $str_next_sunday_date = date('F-j-Y', strtotime('next Sunday'));
    $parts = explode('-', $str_next_sunday_date);
    $week_header = 'Week of ' . $parts[0] . ' ' . $parts[1] . ', ' . $parts[2];
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//h3[.='$week_header']");

    if (!$element) {
      $element = $page->find('xpath', "//h3");
      throw new Exception("The weekly calendar for the '$week_header' is not displayed correctly. It was '" . $element->getText() ."'");
    }
  }

  /**
   * @Then /^I should not see the button "([^"]*)"$/
   */
  public function iShouldNotSeeTheButton($button) {
    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//input[@type='submit' or @type='button'][@value='$button' or @id='$button' or @name='$button']");

    if ($element) {
      throw new Exception("A button with id|name|value equal to '$button' was found.");
    }
  }

  /**
   * @Given /^I set feature "([^"]*)" to "([^"]*)" on "([^"]*)"$/
   */
  public function iSetFeatureStatus ($feature, $status, $group) {

    $features = FeatureHelp::VsiteGetVariable($group, 'spaces_features');
    $info = spaces_features('og');

    $current_value = "current value not found";

    foreach ($info as $k => $i) {
      if ($i->info['name'] == $feature) {
        $current_value = $features[$k];
        break;
      }
    }

    /* cases
     * Disabled > Enabled
     * Disabled > Private
     * Enabled > Disabled
     * Enabled > Private
     * Private > Enabled
     * Private > Disabled
     *
     * Private = 2
     * Enabled = 1
     * Disabled  = 0
     */

    switch ($status) {
      case 'Disabled':
      default:
        $new_value = 0;
        break;
      case 'Enabled':
      case 'Public':
        $new_value = 1;
        break;
      case 'Private':
        $new_value = 2;
        break;
    }

    $opening = array(
      new Step\When('I visit "' . $group . '"'),
      new Step\When('I make sure admin panel is open'),
      new Step\When('I open the admin panel to "Settings"'),
      new Step\When('I sleep for "1"'),
      new Step\When('I click on the "Enable / Disable Apps" control'),
      new Step\When('I scroll to find "'.$feature.'" in the ".app-form" element'),
      new Step\When('I wait "1 second"')
    );

    $closer = array(
      new Step\When('I wait "5 seconds"'),
      new Step\When('I scroll to find "Save"'),
      new Step\When('I press "Save"'),
      new Step\When("I wait for page actions to complete"),
    );

    $enable = array(
      new Step\When('I check the "Enable" box in the "'.$feature.'" row'),
    );

    $disable = array(
      new Step\When('I check the "Disable" box in the "'.$feature.'" row'),
    );

    $public = array(
      new Step\When('I click the "[app-privacy-selector]" control in the "'.$feature.'" row'),
      new Step\When('I click the "The Public" control in the "'.$feature.'" row'),
    );

    $private = array(
      new Step\When('I click the "[app-privacy-selector]" control in the "'.$feature.'" row'),
      new Step\When('I click the "Site Members Only" control in the "'.$feature.'" row'),
    );

    $output = array();
    if ($current_value === "current value not found") {
      throw new Exception("No current value found for feature '$feature'");
    }
    if ($current_value == $new_value) {
      return;
    }
    else if ($new_value == 0) {
      $output = array_merge($opening, $disable, $closer);
    }
    elseif ($current_value == 0 && $new_value == 1) {
      $output = array_merge($opening, $enable, $closer);
    }
    else if ($current_value == 0 && $new_value == 2) {
      $output = array_merge($opening, $enable, $closer, $opening, $private, $closer);
    }
    else if ($current_value == 1 && $new_value == 2) {
      $output = array_merge($opening, $private, $closer);
    }
    else if ($current_value == 2 && $new_value == 1) {
      $output = array_merge($opening, $public, $closer);
    }

    return $output;
  }

  /**
   * @Given /^I check the "([^"]*)" box in the "([^"]*)" row$/
   */
  public function iCheckTheBoxInTheRow($column, $row) {
    $x = '//table/tbody/tr[contains(.,"'.$row.'")]/td[count(//table/thead/tr/th[.="'.$column.'"]/preceding-sibling::th)+1]/input[@type="checkbox"]';
    $elem = $this->getSession()->getPage()->find('xpath', $x);
    if (!$elem) {
      throw new Exception("No checkbox in the \"$column\" column of row \"$row\"");
    }

    // We cannot use check() on angular forms. Angular WILL NOT detect the changes to the model.
    // We have to use click()
    $elem->click();
  }

  /**
   * @Given /^I click the "([^"]*)" control in the "([^"]*)" row$/
   */
  public function iClicktheControlInTheRow($control, $row) {
    $x = '//table/tbody/tr[contains(.,"'.$row.'")]/td//';
    if ($control[0] == '[') {
      // this is an angular directive we're clicking on
      $control = trim($control, '[]');
      $x .= "*[@$control]";
    }
    else {
      $x .= '*[.="'.$control.'"]';
    }
    $elem = $this->getSession()->getPage()->find('xpath', $x);
    if (!$elem) {
      throw new Exception("No control \"$control\" in the row \"$row\"");
    }

    $elem->click();
  }

  /**
   * @Given /^I update the node "([^"]*)" field "([^"]*)" to "([^"]*)"$/
   */
  public function iUpdateTheNodeFieldTo($title, $field, $value) {
    $nid = FeatureHelp::getNodeId($title);

    $purl = FeatureHelp::GetNodeVsitePurl($nid);
    $purl = !empty($purl) ? $purl . '/' : '';

    return array(
      new Step\When('I visit "' . $purl . 'node/' . $nid . '/edit"'),
      new Step\When('I fill in "' . $field . '" with "' . $value . '"'),
      new Step\When('I press "Save"'),
    );
  }

  /**
   * @Given /^I make "([^"]*)" a member in vsite "([^"]*)"$/
   */
  public function iMakeAMemberInVsite($username, $group) {
    return array(
      new Step\When('I visit "' . $group . '/cp/users/add"'),
      new Step\When('I fill in "User" with "' . $username . '"'),
      new Step\When('I press "Add member"'),
    );
  }

  /**
   * @Given /^I make registration to event without javascript available$/
   */
  public function iMakeRegistrationToEventWithoutJavascriptAvailable() {
    FeatureHelp::EventRegistrationForm();
  }

  /**
   * @Given /^I make registration to event without javascript unavailable$/
   */
  public function iMakeRegistrationToEventWithoutJavascriptUnavailable() {
    FeatureHelp::EventRegistrationLink();
  }

  /**
   * @When /^I enable read-only mode$/
   */
  public function iEnableReadOnlyMode() {
    FeatureHelp::SetReadOnly(TRUE);
  }

  /**
   * @Then /^I disable read-only mode$/
   */
  public function iDisableReadOnlyMode() {
    FeatureHelp::SetReadOnly(FALSE);
  }

  /**
   * @Then /^I enable pinserver$/
   */
  public function iEnablePinserver() {
    module_enable(array('pinserver', 'pinserver_authenticate', 'os_pinserver_auth'));
  }

  /**
   * @Then /^I disable pinserver$/
   */
  public function iDisablePinserver() {
    module_disable(array('pinserver', 'pinserver_authenticate', 'os_pinserver_auth'));
  }

  /**
   * @Given /^I verify that "([^"]*)" is the owner of vsite "([^"]*)"$/
   */
  public function iVerifyThatIsTheOwnerOfVsite($username, $group) {
    $uid = FeatureHelp::GetUserByName($username);

    if ($uid != node_load(FeatureHelp::getNodeId($group))->uid) {
      throw new Exception("User '$username' is not the owner of vsite '$group'.");
    }
  }

  /**
   * @Given /^I edit the membership of "([^"]*)" in vsite "([^"]*)"$/
   */
  public function iEditTheMembershipOfInVsite($username, $group) {
    $uid = FeatureHelp::GetUserByName($username);
    return array(
      new Step\When('I visit "' . $group . '/cp/users/edit_membership/' . $uid . '"'),
    );
  }

  /**
   * @Given /^I re import feed item "([^"]*)"$/
   */
  public function iReImportFeedItem($node) {
    $nid = FeatureHelp::GetNodeId($node);

    $source = feeds_source('os_reader', $nid);
    try {
      $source->import();
    } catch (\Exception $e) {

    }
  }

  /**
   * @Then /^I verify the feed item "([^"]*)" exists only "([^"]*)" time for "([^"]*)"$/
   */
  public function iVerifyTheFeedItemeExistsOnlyTimeFor($node, $time, $vsite) {
    $count = FeatureHelp::CountNodeInstances($node, $vsite);

    if ($count != $time) {
      throw new Exception(sprintf('The feed items has been imported %s times.', $count));
    }
  }

  /**
   * @Given /^I edit the entity "([^"]*)" with title "([^"]*)"$/
   */
  public function iEditTheEntityWithTitle($entity_type, $title) {
    $id = FeatureHelp::getEntityID($entity_type, $title);
    $purl = FeatureHelp::GetEntityVsitePurl('file', $id);
    $purl = !empty($purl) ? $purl . '/' : '';

    return array(
      new Step\When('I visit "' . $purl . $entity_type . '/' . $id . '/edit"'),
    );
  }

  /**
   * @Given /^I verify that the profile "([^"]*)" has a child site named "([^"]*)"$/
   */
  public function iVerifyTheProfileHasChildSite($profile_title, $child_site_title) {
    $child_site_nid = FeatureHelp::getEntityID('node', $child_site_title, 'personal');
    $child_site_from_profile = FeatureHelp::getChildSiteNid($profile_title);

    if (!$child_site_from_profile) {
      throw new Exception(sprintf('The profile %s has no child site.', $profile_title));
    }
    elseif ($child_site_from_profile != $child_site_nid) {
      throw new Exception(sprintf('The child site of the profile "%s" is not the site "%s".', $profile_title, $child_site_title));
    }
  }

  /**
   * @given /^I whitelist the domain "([^"]*)"$/
   */
  public function iWhitelistTheDomain($domain) {
    FeatureHelp::AddToWhiteList($domain);
  }

  /**
   * @Then /^I verify "([^"]*)" comes before "([^"]*)"$/
   */
  public function iVerifyComesBefore($first, $second) {
    $page = $this->getSession()
      ->getPage()
      ->getContent();

    $pattern = "/[\s\S]*" . $first . "[\s\S]*" . $second . "[\s\S]*/";
    if (!preg_match($pattern, $page)) {
      throw new Exception("'$first' does not come before '$second'.");
    }
  }

  /**
   * @Given /^I should see "([^"]*)" in "([^"]*)"$/
   */
  public function iShouldSeeIn($value, $path) {
    return array(
      new Step\When('I visit "' . $path . '"'),
      new Step\When('I should see "' . $value . '"'),
    );
  }

  /**
   * @Given /^I delete the node "([^"]*)"$/
   */
  public function iDeleteTheNode($title) {
    $nid = FeatureHelp::getNodeId($title);
    node_delete($nid);
  }


  /**
   * @Given /^I drill down to see the hour$/
   */
  public function iDrillDownToSeeTheHour() {
    for ($i = 0; $i <= 3; $i++) {
      $element = $this->getSession()->getPage()->find('xpath', "//*[contains(@class, 'facetapi-facet-created')]//a[@class='facetapi-inactive' and last()]");

      if (!$element) {
        throw new Exception('Link was not found.');
      }

      $element->click();
    }
  }

  /**
   * @Then /^I verify the facet is in UTC format$/
   */
  public function iVerifyTheFacetIsInUTCFormat() {
    $element = $this->getSession()->getPage()->find('xpath', "//*[contains(@class, 'facetapi-facet-created')]");
    $nid = FeatureHelp::getNodeId("Tesla's Blog");

    if (!$node = node_load($nid)) {
      throw new Exception("The node Tesla's Blog was not found");
    }

    $found = strpos($element->getText(), format_date($node->created, 'custom', 'g:i A'));

    if ($found === FALSE) {
      throw new Exception('the formatted creates timestamp was not found in the facet filter value.');
    }
  }

  /**
   * @Given /^I click "([^"]*)" under "([^"]*)"$/
   */
  public function iClickUnder($link, $class) {
    $element = $this->getSession()->getPage()->find('xpath', "//*[@class='{$class}']/a[.='{$link}']");

    if (!$element) {
      throw new \Exception('The link was no fount.');
    }

    $element->click();
  }

  /**
   * @Given /^I save the page address$/
   */
  public function iSaveThePageAddress() {
    $element = $this->getSession()->getPage()->find('xpath', "//div[@class='form-region-main']//div[@class='description']");
    $childrens = explode(" ", $element->getText());

    if (!$childrens) {
      throw new Exception('The text was not found in the edit page.');
    }

    $this->url = $childrens[1];
  }

  /**
   * @Then /^I verify the page kept the same$/
   */
  public function iVerifyThePageKeptTheSame() {
    $prev_url = $this->url;
    $this->iSaveThePageAddress();

    if ($this->url != $prev_url) {
      throw new Exception('The text has been changed during the title changing.');
    }
  }

  /**
   * @Given /^I edit the page "([^"]*)"$/
   */
  public function iEditThePage($title) {
    $element = $this->getSession()->getPage()->find('xpath', "//*[@class='page-edit']/a[.='Edit']");
    $this->visit($element->getAttribute('href'));
  }

  /**
   * @Given /^I verify the url did not changed$/
   */
  public function iVerifyTheUrlDidNotChanged() {
    if ($this->getSession()->getCurrentUrl() != $this->url) {
      throw new Exception('The url of the pages has changed.');
    }
  }

  /**
   * @Given /^I fill in the field "([^"]*)" with the node "([^"]*)"$/
   *
   * This step is used to fill in an autocomplete field.
   */
  public function iFillInTheFieldWithTheNode($id, $title) {
    $nid = FeatureHelp::getNodeId($title);
    $element = $this->getSession()->getPage();
    $value = $title . ' (' . $nid . ')';
    $element->fillField($id, $value);
  }

  /**
   * @Given /^I can't visit "([^"]*)"$/
   */
  public function iCanTVisit($url) {
    $access_denied_string = "denied";

    $this->visit($url);
    try {
      $this->assertSession()->statusCodeEquals(403);
    } catch (Exception $e) {
      print "No status code found.\n";
      print "Checking for '$access_denied_string' in page content.\n";
    }
    $content = $this->getSession()->getPage()->getContent();
    if (preg_match("/$access_denied_string/i", $content)) {
      return;
    }
 
    throw new Exception("Did not get 403 status code or '$access_denied_string'.");
  }

  /**
   * @Given /^I fill in the "([^"]*)" "([^"]*)" field under "([^"]*)" with "([^"]*)"$/
   */
  public function iFillInTheFieldContainingText($nth, $field_type, $field_under_text, $value) {
    $element = $this->_getNthFieldBelowXyz($nth, $field_type, $field_under_text);
    $element->setValue($value);
  }

  /**
   * Create an entity of a given type and title.
   */
  private function createEntity($type, $title) {
    $values = array(
      'type' => $type,
      'uid' => 1,
      'created' => time(),
      'title' => $title,
    );

    // Create an event that ends tomorrow.
    $entity = entity_create('node', $values);
    return $entity;
  }

  /**
   * @Given /^I should see "([^"]*)" in the "([^"]*)" column$/
   */
  public function iShouldSeeInTheColumn($value, $column) {

    // temporary
    if (!$this->getSession()->getPage()->find("xpath", "//div[@id='content']//table")) {
      throw new Exception(sprintf("No files found."));
    }

    $column_str = strtolower($column);
    $text = $this->lower_case('text()');
    $query = "//div[@id='content']//table/tbody/tr/td[count(//table/thead/tr/th[contains({$text}, '{$column_str}')]/preceding-sibling::th)+1]";
    $elements = $this->getSession()->getPage()->findAll('xpath', $query);
    if (count($elements) == 0) {
      throw new Exception(sprintf("No column %s found on page.", $column));
    }

    foreach ($elements as $elem) {
      if ($elem->getText() == $value) {
        return;
      }
    }
    throw new Exception(sprintf("No row has the value \"%s\" in the column \"%s\".", $value, $column));
  }

  private function lower_case($string, $escape = FALSE) {
    if ($escape) {
      $string = "'".$string."'";
    }
    return "translate($string, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')";
  }

  /**
   * @Given /^I should see "([^"]*)" in the "([^"]*)" column for the row "([^"]*)"$/
   */
  public function iShouldSeeInTheColumnInTheRow($value, $column, $row) {

    $column_str = strtolower($column);
    $row_str = strtolower($row);
    $text = $this->lower_case('text()');
    $dot = $this->lower_case('.');
    $query = "//table//text()[contains({$dot},'{$row_str}')]/ancestor::*[self::tr]/td[count(//table/thead/tr/th[contains({$text}, '{$column_str}')]/preceding-sibling::th)+1]";

    $element = $this->getSession()->getPage()->find('xpath', $query);
    if (!$element) {
      throw new Exception(sprintf("The column \"%s\" or row \"%s\" was not found", $column, $row));
    }

    if ($element->getText() != $value) {
      throw new Exception(sprintf("The value for the %s column should be %s but it is %s", $column, $value, $element->getText()));
    }
  }

  /**
   * @Given /^I should not see "([^"]*)" in the "([^"]*)" column for the row "([^"]*)"$/
   */
  public function iShouldNotSeeInTheColumnInTheRow($value, $column, $row) {
    $index = 0;
    switch ($column) {
      case 'used in':
        $index = 5;
        break;
    }
    $element = $this->getSession()->getPage()->find('xpath', "//div[@id='content']//table//tr[contains(., '{$row}')]//td[{$index}]");
    if ($element->getText() == $value) {
      throw new Exception(sprintf("The value for the %s column should not be %s", $column, $value));
    }
  }

  /**
   * @Given /^I should not find the text "([^"]*)"$/
   *
   * This step is used to for looking for text in the page while respecting
   * the case sensitivity of the searched text.
   *
   * @see @pageTextNotContains
   */
  public function iShouldNotFindTheText($text) {
    $actual = $this->getSession()->getPage()->getText();
    $actual = preg_replace('/\s+/u', ' ', $actual);
    $regex  = '/'.preg_quote($text, '/').'/u';

    if (preg_match($regex, $actual)) {
      $message = sprintf('The text "%s" appears in the text of this page, but it should not.', $text);
      throw new Exception($message);
    }
  }

  // Media Browser functions
  /**
   * @When /^I wait "([^"]*)" for the media browser to open$/
   */
  public function iWaitForTheMediaBrowserToOpen($time) {
    $this->getSession()->wait((int)$time * 1000);
    if (!$elem = $this->getSession()->getPage()->find('css', '.ui-dialog.media-wrapper') || !$this->getSession()->getPage()->find('css', '.ui-dialog.media-wrapper .media-browser-panes')) {
      throw new Exception('The media browser failed to open.');
    }
  }

  /**
   * @When /^ I upload the file "([^"]*)" to the "([^"]*)" control$/
   *
   * Skip for now. May need to update ng-file-upload for it to work ever
   */
  public function iUploadFileToControl($filename, $control) {
    $driver = $this->getSession()->getDriver();
  }

  /**
   * @When /^I drop the file "([^"]*)" onto the "([^"]*)" area$/
   */
  public function iDropFileOnto($file, $area) {
    // Make sure the element we want exists on the page.
    $xpath = "//*[@ng-file-drop and count(./descendant::span[contains(text(), '$area')])]";
    if (!($elem = $this->getSession()->getPage()->find('xpath', $xpath))) {
      throw new Exception("No droppable region with text \"$area\" found.");
    }
    $target = $elem->getXpath();

    if ($filepath = $this->getMinkParameter('files_path')) {
      $path = rtrim(realpath($filepath), DIRECTORY_SEPARATOR).DIRECTORY_SEPARATOR.$file;
      if (!file_exists($path)) {
        throw new Exception("Target file $path not found");
      }

      $driver = $this->getSession()->getDriver();
      $inputId = 'elem_' . substr(md5(time()), 0, 7);
      $driver->executeScript("$inputId = window.jQuery('<input id=\"$inputId\" type=\"file\">').appendTo('body');");
      if (!($elem->getSession()->getPage()->find('xpath', "//input[@id='$inputId']"))) {
        throw new Exception('Dummy input not found');
      }
      $path = preg_replace('|[\/\\\\]|', DIRECTORY_SEPARATOR, $path);
      $driver->attachFile("//input[@id='$inputId']", $path);

      // File is on the field now. We need to grab it and make a drop event out of it.
      // We can't do this with jQuery because the handlers are not bound using it. ng-file-upload use browser methods.
      $driver->executeScript("
        var drag = document.createEvent(\"HTMLEvents\");
        drag.initEvent('dragover', true, true);
        drag.dataTransfer = {
          files: $inputId.get(0).files
        };
        var drop = document.createEvent(\"HTMLEvents\");
        drop.initEvent('drop', true, true);
        drop.dataTransfer = {
          files : $inputId.get(0).files
        };
        var result = document.evaluate(\"$target\", document, null, XPathResult.ORDERED_NODE_ITERATOR_TYPE, null);
        var elem = result.iterateNext();
        elem.dispatchEvent(drag);
        elem.dispatchEvent(drop);");
    }
    else {
      throw new Exception('Mink files_path parameter not configured.');
    }
  }

  /**
   * @When /^I drop the files "([^"]*)" onto the "([^"]*)" area$/
   */
  public function iDropFilesOnto($files, $area) {
    // Make sure the element we want exists on the page.
    $xpath = "//*[@ng-file-drop and count(./descendant::span[contains(text(), '$area')])]";
    if (!($elem = $this->getSession()->getPage()->find('xpath', $xpath))) {
      throw new Exception("No droppable region with text \"$area\" found.");
    }
    $target = $elem->getXpath();

    if ($filepath = $this->getMinkParameter('files_path')) {
      $files = explode(', ', $files);
      $paths = array();
      foreach ($files as $file) {
        $path = rtrim(realpath($filepath), DIRECTORY_SEPARATOR) . DIRECTORY_SEPARATOR . $file;
        if (!file_exists($path)) {
          throw new Exception("Target file $path not found");
        }
        $paths[] = $path;
      }

      // Selenium has no support for <input type="file" multiple>. So instead, we use multiple input elements,
      // then dummy up a JS FilesList object to pass to the element.
      $driver = $this->getSession()->getDriver();
      $inputs = array();
      foreach ($paths as $k => $path) {
        $inputId = 'elem_' . substr(md5(time()), 0, 7) . '_' . $k;
        $driver->executeScript("$inputId = window.jQuery('<input id=\"$inputId\" type=\"file\">').appendTo('body');");
        if (!($elem->getSession()->getPage()->find('xpath', "//input[@id='$inputId']"))) {
          throw new Exception('Dummy input not found');
        }
        $driver->attachFile("//input[@id='$inputId']", $path);
        $inputs[] = $inputId;
      }

      // Files are on the separate fields now. We need to grab them, make them into something similar to a FilesList
      // object, and turn that into an event
      $inputStr = json_encode($inputs);

      $driver->executeScript("
        var inputs = $inputStr;
        drop = document.createEvent(\"HTMLEvents\");
        drop.initEvent('drop', true, true);
        drop.dataTransfer = {
          files : []
        };
        for (var i = 0; i < inputs.length; i++) {
          drop.dataTransfer.files.push(jQuery('#'+inputs[i]).get(0).files[0]);
        }
        drop.dataTransfer.files.item = function (i) {
          return this[i];
        }
        var drag = document.createEvent(\"HTMLEvents\");
        drag.initEvent('dragover', true, true);
        drag.dataTransfer = drop.dataTransfer;
        var result = document.evaluate(\"$target\", document, null, XPathResult.ORDERED_NODE_ITERATOR_TYPE, null);
        var elem = result.iterateNext();
        elem.dispatchEvent(drag);
        elem.dispatchEvent(drop);");
    }
    else {
      throw new Exception('Mink files_path parameter not configured.');
    }
  }

  /**
   * @Then /^I should wait for "([^"]*)" directive to "([^"]*)"$/
   */
  public function iWaitForDirective($directive, $appear) {
    $directive = strtolower(preg_replace('/([ ]+)/', '-', $directive));
    $xpath = "//*[@$directive]";
    $this->waitForXpathNode($xpath, $appear);
  }

  /**
   * @Then /^I should see the media browser "([^"]*)" tab is active$/
   */
  public function iShouldSeeTabActive($tab) {
    // Js is async may take up to 3 sec to appear. so we wait.
    $duration = 5000;
    $this->getSession()->wait($duration);
    // In case element doesn't exists.
    if (!($elem = $this->getSession()->getPage()->find('css', '.media-browser-button.active'))) {
      throw new Exception('No Media Browser tab is active.');
    }

    if ($elem->getText() != $tab) {
      throw new Exception('Wrong tab is active');
    }
  }

  /**
   * @Then /^I confirm the file "([^"]*)" in the site "([^"]*)" is the same file as "([^"]*)"$/
   */
  public function iConfirmTheFileInSiteIsSameAs($filename, $site, $original) {
    $file = $this->getFile($filename, $site);

    // change directories to so all the file wrapper functions work properly
    $current = getcwd();
    chdir(DRUPAL_ROOT);

    // catch any exceptions so we can change the directory back should an exception happen
    try {
      if ($filepath = $this->getMinkParameter('files_path')) {
        $originalPath = $filepath . '/' . $original;
        if (filesize(drupal_realpath($file->uri)) != filesize($originalPath) || sha1_file($file->uri) != sha1_file($originalPath)) {
          throw new Exception("File \"$filename\" in site \"$site\" is not the same file as \"$original\"");
        }
      } else {
        throw new Exception('Mink files_path parameter not configured.');
      }
    } catch (Exception $e) {
      // catch everything so we can change the directory back to the original state
      // then throw them again
      chdir($current);
      throw $e;
    }
  }

  /**
   * @Then /^I confirm the file "([^"]*)" in the site "([^"]*)" is not the same file as "([^"]*)"$/
   */
  public function iConfirmTheFileInSiteIsNotSameAs($filename, $site, $original) {
    $file = $this->getFile($filename, $site);

    // change directories to so all the file wrapper functions work properly
    $current = getcwd();
    chdir(DRUPAL_ROOT);

    // catch any exceptions so we can change the directory back should an exception happen
    try {
      if ($filepath = $this->getMinkParameter('files_path')) {
        $originalPath = $filepath.'/'.$original;
        if (filesize(drupal_realpath($file->uri)) == filesize($originalPath) && sha1_file($file->uri) == sha1_file($originalPath)) {
          throw new Exception("File \"$filename\" in site \"site\" is the same file as \"original\"");
        }
      }
      else {
        throw new Exception('Mink files_path parameter not configured.');
      }
    } catch (Exception $e) {
      // catch everything so we can change the directory back to the original state
      // then throw them again
      chdir($current);
      throw $e;
    }
  }

  protected function getFile($filename, $site) {
    $uri = "$site/files/$filename";
    $q = db_select('file_managed', 'fm')
      ->fields('fm', array('fid'))
      ->condition('uri', '%'.$uri, 'LIKE')
      ->execute();

    foreach ($q as $r) {
      return file_load($r->fid);
    }
    throw new Exception("file \"$filename\" not found in site \"$site\"");
  }

  /**
   * @Then /^I should see "([^"]*)" in an? "([^"]*)" element$/
   *
   * Check text in multiple matching elements for a match
   * Default implementation (in the "" element) does not work when multiple elements match selector
   */
  public function iShouldSeeInAElement($text, $selector) {
    usleep(200);
    //error_log($this->getSession()->getPage()->getHtml());
    $elems = $this->getSession()->getPage()->findAll('css', $selector);
    if (count($elems) == 0) {
      throw new Exception("No element matching selector \"$selector\" found.");
    }
    foreach ($elems as $e) {
      if (stripos($e->getText(), $text) !== FALSE) {
        return;
      }
    }
    throw new Exception("The text \"$text\" was not found in any element matching \"$selector\"");
  }

  /**
   * @Then /^I should not see "([^"]*)" in an? "([^"]*)" element$/
   *
   * Check text in multiple matching elements for a match
   * Default implementation (in the "" element) does not work when multiple elements match selector
   */
  public function iShouldNotSeeInAElement($text, $selector) {
    $elems = $this->getSession()->getPage()->findAll('css', $selector);
    foreach ($elems as $e) {
      if (stripos($e->getText(), $text) !== FALSE) {
        throw new Exception("The text \"$text\" was found in an element matching \"$selector\"");
      }
    }
  }

  /**
   * @when /^I mouse over "([^"]*)"$/
   */
  public function iMouseOver($text) {
    $elem = $this->getSession()->getPage()->find('xpath', "//*[text() = '$text']");

    if ($elem) {
      $elem->mouseOver();
    }
    else {
      throw new Exception ("No element with text \"$text\" is found.");
    }
  }

  /**
   * @When /^I mouse over the "([^"]*)" element$/
   */
  public function iMouseOverElement($selector) {
    $elem = $this->getSession()->getPage()->find('css', $selector);

    if ($elem) {
      $elem->mouseOver();
    }
    else {
      throw new Exception("No element matching \"$selector\" is found.");
    }
  }

  /**
   * @When /^I wait "([^"]*)"$/
   */
  public function iWait($time) {
    $seconds = strtotime($time, 0);
    $this->getSession()->getDriver()->wait($seconds * 1000, false);
  }

  /**
   * @Given /^I should find the text "([^"]*)"$/
   *
   * This step is used to for looking for text in the page while respecting
   * the case sensitivity of the searched text.
   */
  public function iShouldFindTheText($text) {
    $actual = $this->getSession()->getPage()->getText();
    $actual = preg_replace('/\s+/u', ' ', $actual);
    $regex  = '/'.preg_quote($text, '/').'/u';

    if (!preg_match($regex, $actual)) {
      $message = sprintf('The text "%s" did not appear in the text of this page, but it should have.', $text);
      throw new Exception($message);
    }
  }

  /**
   * @Given /^I should match the regex "([^"]*)"$/
   *
   * This step is used to match a regular expression in the page
   */
  public function iShouldMatchTheRegex($pattern) {
    $page_text = $this->getSession()->getPage()->getText();

    $page_text = preg_replace('/\s+/u', ' ', $page_text);
    $regex = '/'.$pattern.'/iu';

    if (!preg_match($regex, $page_text)) {
      $message = sprintf('The regex pattern "%s" did not appear in the text of this page, but it should have.', $pattern);
      throw new Exception($message);
    }
  }

  /**
   * @Then /^I should wait for the text "([^"]*)" to "([^"]*)"$/
   */
  public function iShouldWaitForTheTextTo($text, $appear) {
    try {
      $this->waitForXpathNode("//*[text()[contains(.,\"$text\")]]", $appear);
    }
    catch (Exception $e) {
      if ($e->getMessage() == "waitFor timed out.") {
        throw $e;
      }
      throw new Exception("Text \"$text\" did not \"$appear\" after 5 seconds.");
    }
  }

  /**
   *
   * @param string $xpath
   *   The XPath string.
   * @param bool $appear
   *   Determine if element should appear. Defaults to TRUE.
   *
   * @throws Exception
   */
  private function waitForXpathNode($xpath, $appear = 'appear') {
    $appear = $appear == 'appear';
    $this->waitFor(function(FeatureContext $context) use ($xpath, $appear) {
      try {
        //$this->getSession()->getPage()->find()
        //$nodes = $context->getSession()->getPage()->find('xpath', $xpath);
        $nodes = $context->getSession()->getDriver()->find($xpath);
        if (count($nodes) > 0) {
          $visible = $nodes[0]->isVisible();
          return $appear ? $visible : !$visible;
        }
        return !$appear;
      }
      catch (WebDriver\Exception $e) {
        error_log('exception');
        if ($e->getCode() == WebDriver\Exception::NO_SUCH_ELEMENT) {
          error_log('exception. returning ' . (!$appear ? "true":"false"));
          return !$appear;
        }
        error_log($e->getCode());
        error_log($e->getMessage());
        if ($appear) {
          throw $e;
        }
      }
    }, 20000);
  }

  /**
   * Helper function; Execute a function until it return TRUE or timeouts.
   *
   * @param $fn
   *   A callable to invoke.
   * @param int $timeout
   *   The timeout period. Defaults to 10 seconds.
   *
   * @throws Exception
   */
  private function waitFor($fn, $timeout = 5000) {
    $start = microtime(true);
    $end = $start + $timeout / 1000;
    while (microtime(true) < $end) {
      if ($fn($this)) {
        return;
      }
      usleep(10);
    }
    throw new \Exception('waitFor timed out.');
  }

  /*
   * @Given /^I logout$/
   */
  public function iLogout() {
    $this->visit('user/logout');
  }

  /**
   * @When /^I remove the file "([^"]*)" from the node "([^"]*)" of type "([^"]*)"$/
   */
  public function iRemoveTheFileFromTheNode($filename, $title, $type) {
    $nid = FeatureHelp::getNodeId($title);

    $wrapper = entity_metadata_wrapper('node', $nid);

    switch ($type) {
      case "media gallery":
        $field_name = 'media_gallery_file';
        break;
    }

    // Remove the file from the field.
    $new_files = array();
    $files = $wrapper->{$field_name}->value();
    foreach ($files as $file) {
      if ($file['filename'] == $filename) {
        continue;
      }
      $new_files[] = array(
        'fid' => $file['fid'],
        'display' => 1,
      );
    }

    // Set the field again and save.
    $wrapper->{$field_name}->set($new_files);
    $wrapper->save();
  }

  /**
   * @When /^I click on "([^"]*)" button in the media browser$/
   */
  public function iClickOn($text) {
    $element = $this->getSession()->getPage()->find('xpath', "//*[contains(@class, 'media-browser-button') and text() = '{$text}']");
    $element->click();
  }

  /**
   * @When /^I click on the tab "([^"]*)"$/
   */
  public function iClickOnTheTab($arg1) {
    if ($element = $this->getSession()->getPage()->find('xpath', "//*[.='{$arg1}']")) {
      $element->press();
      usleep(50);
    }
    else {
      throw new ElementNotFoundException($this->getSession(), "No tab with text ($arg1) found on page.");
    }
  }

  /**
   * @When /^I click on the "([^"]*)" control$/
   */
  public function iClickOnControl($text) {
    if ($element = $this->getSession()->getPage()->find('xpath', "//*[translate(text(), ' ', '') = translate('{$text}', ' ', '')]")) {
      $element->click();
    }
    else {
      throw new ElementNotFoundException($this->getSession(), "No element with text ($text) found on page.");
    }
  }

  /**
   * @When /^I click on the "([^"]*)" control in the "([^"]*)" element$/
   */
  public function iClickOnControlInElement($text, $css) {
    $page = $this->getSession()->getPage();
    $parents = $page->findAll('css', $css);

    foreach ($parents as $p) {
      if ($p->isVisible()) {
        if ($elem = $p->find('xpath', "//*[text() = '{$text}']")) {
          $elem->click();
        }
        else {
          throw new ElementNotFoundException($this->getSession(), "No $text found in $css element.");
        }
      }
    }
  }

  /**
   * @Given /^I am deleting the file "([^"]*)"$/
   */
  public function iAmDeletingTheFile($filename) {
    $fid = FeatureHelp::getEntityID('file', $filename);
    $file = file_load($fid);
    file_delete($file);
  }

  /**
   * @Given /^I should verify the file "([^"]*)" exists$/
   */
  public function iShouldVerifyTheFileExists($filename) {
    $fid = FeatureHelp::getEntityID('file', $filename);

    $result = db_select('file_usage', 'fu')
      ->fields('fu')
      ->condition('fu.module', 'os_files')
      ->condition('fu.fid', $fid)
      ->execute()
      ->fetchAssoc();

    if (empty($result)) {
      throw new Exception(sprintf("No file usage was found for the file %s", $filename));
    }
  }

  /**
   * @Then /^I should see a table with the text "([^"]*)" in its header$/
   */
  public function iShouldSeeATableWithTheTextInItsHeader($text) {
    $element = $this->getSession()->getPage()->find('xpath', "//div[@id='content']//table//thead//tr//th[contains(., '{$text}')]");
    if (!$element) {
      throw new Exception(sprintf("The header of the table doesn't contain the text %s", $text));
    }
  }

  /**
   * @Given /^I sign up "([^"]*)" with email "([^"]*)" to the event "([^"]*)"$/
   */
  public function iSignUpWithEmailToTheEvent($name, $email, $event) {
    $event_id = FeatureHelp::getNodeId($event);
    return array(
      new Step\When('I visit "john/os_events/nojs/registration/' . $event_id . '"'),
      new Step\When('I fill in "edit-anon-mail" with "' . $email . '"'),
      new Step\When('I fill in "edit-field-full-name-und-0-value" with "' . $name . '"'),
      new Step\When('I press "edit-submit"'),
    );
  }

  /**
   * @Given /^I manage registrations for the event "([^"]*)"$/
   */
  public function iManageRegistrationsForTheEvent($event) {
    $event_id = FeatureHelp::getNodeId($event);
    return array(
      new Step\When('I visit "john/node/' . $event_id . '/registrations"'),
    );
  }

  /**
   * @When /^I export the registrants list for the event "([^"]*)" in the site "([^"]*)"$/
   */
  public function iExportTheRegistrantsListForTheEventInTheSite($event, $site) {
    $url = $this->getSession()->getCurrentUrl() . '/export?testing=true';

    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $url);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($curl, CURLOPT_HEADER, false);

    if (!$data = curl_exec($curl)) {
      throw new Exception(sprintf("Could not export the list of registrants."));
    }
    curl_close($curl);
    $this->exportedRegistrants = $data;
  }

  /**
   * @Then /^I verify the file contains the user "([^"]*)" with email of "([^"]*)"$/
   */
  public function iVerifyTheFileContainsTheUserWithEmailOf($name, $email) {
    if (!preg_match("/" . $email . "(.*)" . $name . "/", $this->exportedRegistrants)) {
      throw new Exception(sprintf("List of registrants is exported wrong."));
    }
  }

  /**
   * @AfterStep
   */
  public function dumpInfoAfterFailedStep(StepEvent $event) {
    if ($event->getResult() == StepEvent::FAILED)  {

      try {
        $this->iDisplayWatchdog();
        $this->iShouldPrintPage();

        if ($this->getSession()->getDriver() instanceof \Behat\Mink\Driver\Selenium2Driver) {
          $this->iPrintPageScreenShot();
        }
      }
      catch (\Exception $e) {

      }

    }
  }

  /**
   * @When /^I change the date of "([^"]*)" in "([^"]*)"$/
   */
  public function iChangeTheDateOfIn($label, $site) {
    $nid = FeatureHelp::getNodeIdInVsite($label, $site);
    $wrapper = entity_metadata_wrapper('node', $nid);
    $date = $wrapper->field_date->value();
    // Set the event to last year.
    $date[0]['value'] = str_replace(date('Y'), date('Y') - 1, $date[0]['value']);
    $date[0]['value2'] = str_replace(date('Y'), date('Y') - 1, $date[0]['value2']);
    $wrapper->field_date->set($date);
    $wrapper->save();
  }

  /**
   * @Given /^I run the "([^"]*)" report with "([^"]*)" <checked>:$/
   */
  public function iRunTheReportWithChecked($report, $multiValueFieldLabel, TableNode $table) {
    $steps = array();
    $steps[] = new Step\When('I visit "admin/reports/os/' . $report . '"');
    $table_rows = $table->getRows();
    // Iterate over each row, just so if there's an error we can supply
    // the row number, or empty values.
    foreach ($table_rows as $i => $checkbox) {
      $steps[] = new Step\When('I check the box "' . $checkbox[0] . '"');
    }
    $steps[] = new Step\When('I press "Download CSV of Full Report"');
    return $steps;
  }

  /**
   * @Then /^I will see a report with content in the following <columns>:$/
   */
  public function iWillSeeAReportWithContentInTheFollowingColumns(TableNode $contentRequirements) {
    $data = $this->getDataFromCSVFile($this->getSession()->getPage());
    // get content requirements from test case
    $requirements = array();
    foreach($contentRequirements->getRows() as $row) {
      $requirements[$row[0]] = ($row[1] == 'populated') ? 1 : 0;
    }

    // compare data to requirements
    foreach($data as $count => $data_row) {
      foreach($requirements as $header => $content) {
        if((!isset($data_row[$header]) || !$data_row[$header]) && $requirements[$header]) {
          throw new Exception(sprintf("Row #" . ($count + 1) . ", column '$header' should contain content but it doesn't."));
        }
      }
    }
  }

  /**
   * @Given /^I run the "([^"]*)" report with "([^"]*)" set to "([^"]*)" and <checkboxes> selected:$/
   */
  public function iRunTheReportWithSetToAndCheckboxesSelected($report, $fieldName, $fieldValue, TableNode $table) {
    $steps = array();
    $steps[] = new Step\When('I visit "admin/reports/os/' . $report . '"');
    $steps[] = new Step\When('I fill in "' . $fieldName . '" with "' . $fieldValue. '"');
    $table_rows = $table->getRows();

    // Iterate over each row, just so if there's an error we can supply
    // the row number, or empty values.
    foreach ($table_rows as $i => $checkbox) {
      $steps[] = new Step\When('I check the box "' . $checkbox[0] . '"');
    }
    $steps[] = new Step\When('I press "Download CSV of Full Report"');
    return $steps;
  }

  /**
   * @Then /^I will see a report with the following <rows>:$/
   */
  public function iWillSeeAReportWithTheFollowingRows(TableNode $requiredRows) {
    global $base_url;
    $url_parts = explode(".", str_replace("http://", "", $base_url));
    $install = $url_parts[0];

    $file_data = $this->getDataFromCSVFile($this->getSession()->getPage());
    $required_data = $requiredRows->getRows();
    $headers = array_splice($required_data, 0, 1);
    foreach ($required_data as $num => $row) {
      foreach ($row as $index => $value) {
        $header = $headers[0][(int)$index];
        if ($header == "os install") {
          $required_data[(int)$num][$header] = $install;
        }
        elseif ($header == "site url") {
          $required_data[(int)$num][$header] = $base_url . "/" . $value;
        }
        else {
          $required_data[(int)$num][$header] = $value;
        }
        unset($required_data[(int)$num][(int)$index]);
      }
    }
    foreach ($required_data as $index => $row) {
      if (array_diff($row, $file_data[$index]) !== array()) {
        throw new Exception(sprintf("Row #" . ($index + 1) . " does not match."));
      }
    }
  }

  /**
   * @Then /^I will see a report with no results$/
   */
  public function iWillSeeAReportWithNoResults() {
    return new Step\Then('I should see "No results in report."');
  }

  /**
   * @Given /^I run the "([^"]*)" report with "([^"]*)" set to the "([^"]*)" of this "([^"]*)"$/
   */
  public function iRunTheReportWithSetToTheOfThis($report, $dateField, $dateParameter, $datePeriod) {
    $steps = array();
    $steps[] = new Step\When('I visit "admin/reports/os/' . $report . '"');

    $beginning = "";
    $end = "";
    $now = time();
    switch ($datePeriod) {
      case "year":
        $beginning = date("Y", $now) . "0101";
        $end = date("Y", $now) . "1231";
        break;
    }
    $fieldValue = ${$dateParameter};
    if ($dateField == "Creation start") {
      $steps[] = new Step\When('I fill in "edit-creationstart" with "' . $fieldValue . '"');
    } elseif ($dateField == "Creation end") {
      $steps[] = new Step\When('I fill in "edit-creationend" with "' . $fieldValue . '"');
    } else {
      $steps[] = new Step\When('I fill in "' . $dateField . '" with "' . $fieldValue . '"');
    }
    $steps[] = new Step\When('I select the radio button named "includesites" with value "content"');
    $steps[] = new Step\When('I press "Download CSV of Full Report"');

    return $steps;
  }

  /**
   * @Then /^I will see a report with "([^"]*)" values "([^"]*)" to the "([^"]*)" of this "([^"]*)"$/
   */
  public function iWillSeeAReportWithValuesToTheOfThis($column, $operator, $dateParameter, $datePeriod) {
    $beginning = "";
    $end = "";
    $now = time();
    switch ($datePeriod) {
      case "year":
        $beginning = date("Y", $now) . "0101";
        $end = date("Y", $now) . "1231";
        break;
    }
    $submittedValue = ${$dateParameter};
    switch ($operator) {
      case "equal":
        $operatorCode = "=";
        break;
      case "less than or equal":
        $operatorCode = "<=";
        break;
      case "greater than or equal":
        $operatorCode = ">=";
        break;
    }

    $file_data = $this->getDataFromCSVFile($this->getSession()->getPage());
    foreach ($file_data as $num => $row) {
      $fileValue = $row[$column];
      $dateComparison = eval('return strtotime($fileValue) ' . $operatorCode . ' strtotime($submittedValue);');
      if (!$dateComparison) {
        throw new Exception(sprintf("Row #" . ($num + 1) . " has a '$column' value that is not $operator to $submittedValue."));
      }
    }
  }

  /**
   * Helper function to convert CSV file to associative array
   */
  public function getDataFromCSVFile($source) {
    $fileContent = $source->getContent();
    if (strpos($fileContent, "html>") !== FALSE) {
        throw new Exception(sprintf("CSV file was not created."));
    }

    $csv = array_map('str_getcsv', explode("\n", $fileContent));
    array_walk($csv, function(&$a) use ($csv) {
      $a = array_combine($csv[0], $a);
    });
    // remove column header
    array_shift($csv);

    return $csv;
  }

  /**
   * @Then /^I validate the href attribute of metatags link from type$/
   */
  public function iValidateTheHrefAttributeOfMetatagsLinkFromType() {

    // Getting the current node been viewed.
    $node = node_load(FeatureHelp::getNodeIdInVsite('About', 'john'));

    // Base path including the purl.
    $vsite = vsite_get_vsite(2);
    $base_url = variable_get('purl_base_domain');
    $purl_base_path = $base_url . '/' . $vsite->group->purl;

    // Expected urls.
    $expected_tags_value = array(
      'canonical' => url('node/' . $node->nid, array('absolute' => TRUE)),
      'shortlink' => $purl_base_path . '/node/' . $node->nid,
    );

    // Iterate over each metatag and validate its "href" value accordingly.
    foreach ($expected_tags_value as $metatag_name => $value) {

      // Getting the metatag element.
      $page = $this->getSession()->getPage();
      if (!$metatag_element = $page->find('xpath', "//head/link[@rel='{$metatag_name}']")) {
        throw new \Exception(format_string("Could not find the target metatag: '@metatag'", array('@metatag' => $metatag_name)));
      }

      // In case the metatag url in wrong.
      $metatag_href = $metatag_element->getAttribute('href');
      if ($metatag_href != $expected_tags_value[$metatag_name]) {
        // In case the target element is not found.
        $variables = array(
          '@metatag' => $metatag_name,
          '@metatag_given_url' => $metatag_href,
          '@metatag_expected_url' => $expected_tags_value[$metatag_name],
        );

        throw new \Exception(format_string("The '@metatag' metatag expected url is: '@metatag_expected_url' but the given url is: '@metatag_given_url'", $variables));
      }
    }
  }

  /**
   * @Given /^I adding the embedded video$/
   */
  public function iAddingTheEmbeddedVideo() {
    $page = $this->getSession()->getPage();
    if ($elem = $page->find('xpath', "//button[.='Insert']")) {
      $elem->press();
    }
    else {
      throw new \Exception("No insert button found.");
    }
  }

  /**
   * @Given /^I logout$/
   */
  public function iLogout1() {
    $this->getSession()->visit($this->locatePath('/user/logout'));
  }

  /**
   * @Then /^I case sensitive check the text "([^"]*)"$/
   */
  public function iCaseSensitiveCheckTheText($text) {
    $page = $this->getSession()->getPage();

    if (!$page->find('xpath', '//*[.="' . $text . '"]')) {
      throw new \Exception("The text '{$text}'' was not found in the screen");
    }
  }

  /**
   * @Given /^I case sensitive check the text "([^"]*)" not exists$/
   */
  public function iCaseSensitiveCheckTheTextNotExists($text) {
    $page = $this->getSession()->getPage();

    if ($page->find('xpath', '//*[.="' . $text . '"]')) {
      throw new \Exception("The text '{$text}'' was not found in the screen");
    }
  }

  /**
   * @Given /^I make sure admin panel is open$/
   */
  public function adminPanelOpen() {
    $page = $this->getSession()->getPage();
    $this->waitForPageActionsToComplete();

    if ($page->find('css', '[left-menu].closed')) {
      return array(
        new Step\When('I press "Close Menu"'),
        new Step\When('I sleep for "1"'),
      );
    }
    elseif (!$page->find('css', '[left-menu]')) {
      throw new \Exception("The admin panel was not found on this page. Are you sure its installed and enabled?");
    }

    return array();
  }

  /**
   * @Given /^I make sure admin panel is closed$/
   */
  public function adminPanelClosed() {
    $page = $this->getSession()->getPage();
    $this->waitForPageActionsToComplete();
    if (! $page->find('css', '[left-menu].closed')) {
      return array(
        new Step\When('I press "Close Menu"'),
        new Step\When('I sleep for "1"'),
      );
    }
    elseif (!$page->find('css', '[left-menu]')) {
      throw new \Exception("The admin panel was not found on this page. Are you sure its installed and enabled?");
    }

    return array();
  }

  /**
   * @Given /^I open the admin panel to "([^"]*)"$/
   */
  public function iOpenAdminPanelTo($text) {
    $output = $this->adminPanelOpen();
    $page = $this->getSession()->getPage();

    $this->_captureJavaScriptConsoleErrors();

    //$elem = $page->find('xpath', "//*[text() = '{$text}']/ancestor::li[@admin-panel-menu-row]");
    $elem = $page->find('xpath', "//li[@admin-panel-menu-row]/descendant::span[text()='$text']/ancestor::li[@admin-panel-menu-row][1]");
    if (!$elem) {
      $this->_printJavaScriptConsoleErrors();
      throw new \Exception("The link $text cannot be found in the admin panel.");
    }
    if (!$elem->hasClass('open')) {
      $output[] = new Step\When('I click on the "'.$text.'" control');
      $output[] = new Step\When('I sleep for "1"');
    }

    return $output;
  }

  /**
   * @Given /^I open the user menu$/
   */
  public function iOpenUserMenu() {
    $page = $this->getSession()->getPage();
    while (!$page->find('css', '.topRight_Menu')) {
      usleep(100);
    }
    if ($elem = $page->find('css', '.topRight_Menu')) {
      $elem->click();
      sleep(1);
    }
    else {
      $url = $this->getSession()->getCurrentUrl();
      throw new \Exception("Could not find user menu on page $url");
    }
  }

  /**
   * @Given /^I visit the Add class material URL for "([^"]*)" on vsite "([^"]*)"$/
   */
  public function iVisitTheAddClassMaterialUrl($url, $vsite) {

    $session = $this->getSession();
    $nid = $this->_getNodeIdOfUrl("$vsite/$url");

    return new Step\When('I visit "' . $vsite . '/node/add/class-material?field_class=' . $nid . '"');
  }

  /**
   * @When /^the overlay opens$/
   */
  public function overlayOpens() {
    $this->waitFor(function (FeatureContext $context) {
      if ($overlay = $context->getSession()->getPage()->find('css', 'iframe.overlay-active')) {
        $function = <<<JS
        (function () {
          var old = document.getElementById("iframeSwitchTo");
          if (old) {
            if (old.classList.contains('overlay-active')) {
              return;
            }
            else {
              old.id = "";
            }
          }
          var iframe = document.querySelector("iframe.overlay-active");
          iframe.id = "iframeSwitchTo";
        })();
JS;
        $context->getSession()->executeScript($function);
        $context->getSession()->switchToIframe("iframeSwitchTo");
        return true;
      }
      return false;
    }, 20000);
  }

  /**
   * @When /^I wait for the overlay to open$/
   */
  public function iWaitOverlayOpen() {
    return array(
      new Step\When("I wait for page actions to complete"),
      new Step\When("the overlay opens")
    );
  }

  /**
   * @When /^the overlay closes$/
   */
  public function overlayCloses() {
    $this->getSession()->getDriver()->switchToIFrame(null);
    return array(
      new Step\When("I wait for page actions to complete")
    );
  }


  /**
   * @When /^I click on "([^"]*)" in the tools for "([^"]*)"$/
   */
  public function iClickOnTools($link, $node) {
    $page = $this->getSession()->getPage();
    if ($elem = $page->find('xpath', "//*[normalize-space(text()) = '$node']/ancestor::section//article")) {
      $elem->mouseOver();
      if ($clink = $elem->find('xpath', "//a[contains(@class, 'contextual-links-trigger')]")) {
        $clink->click();
        if ($target = $elem->find('xpath', "//ul[contains(@class, 'contextual-links')]//a[text() = '$link']")) {
          $timeout = 0;
          $start = microtime(true);
          while (!$target->isVisible() && $timeout < 1000) {
            usleep(100);
            $timeout = (microtime(true) - $start * 1000);
          }
          $target->click();
        }
        else {
          throw new Exception("No contextual link $link found for node $node");
        }
      }
      else {
        throw new Exception("No contextual links found for node $node");
      }
    }
    else {
      throw new Exception("No node $node found on page.");
    }
  }

  /**
   * @When /^I scroll in the "([^"]*)" element until I find "([^"]*)"$/
   */
  public function iScrollUntil($element, $text) {
    $page = $this->getSession()->getPage();
    $driver = $this->getSession()->getDriver();

    $container = $page->find('css', $element);
    $scrolltest = "var elem = document.querySelector('$element');
      return elem.scrollHeight == elem.scrollTop + elem.clientHeight";
    if (!$container) {
      throw new Exception("The element matching '$element' was not found.");
    }

    $attempts = 0;
    while (!$driver->isVisible("//*[text() = '$text']") && !$page->getSession()->evaluateScript($scrolltest) && $attempts < 20) {
      echo $attempts;
      $page->getSession()->getDriver()->executeScript("document.querySelector('$element').scrollTop += 100");
      usleep(100);
      $attempts++;
    }

    if (!$driver->isVisible("//*[text() = '$text']")) {
      throw new Exception("The text '$text' was not found in the '$element' element.");
    }
    elseif ($attempts == 20) {
      throw new Exception("20 attempts were made and the element is still not visible.");
    }
  }

  /**
   * @When /^I scroll to find "([^"]*)"$/
   */
  function iScrollToFind($text) {
    $this->getSession()->executeScript("
      var result = document.evaluate('//*[.=\"$text\"]', document.body, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null);
      var elem = result.singleNodeValue;
      elem.scrollIntoView();
    ");
  }

  /**
   * @When /^I scroll to find "([^"]*)" in the "([^"]*)" element$/
   */
  function iScrollToFindInElement($text, $selector) {
    $script = '';
    switch($selector[0]) {
      case '/':
        // xpath
        $script .= 'var result = document.evaluate("'.$selector.'", document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null);';
        $script .= 'var elem = result.singleNodeValue;';
        break;
      case '.':
      case '#':
        // css
        $script .= 'var elem = document.querySelector("'.$selector.'");';
    }

    $script .= "var target = document.evaluate('.//*[.=\"$text\"]', elem, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;";
    $script .= "target.scrollIntoView()";
    $this->getSession()->executeScript($script);
  }

  /**
   * @When /^I set the form "([^"]*)" to "([^"]*)"$/
   */
  public function iSetTheFormTo($form, $dirtiness) {
    $dirty = $dirtiness == 'dirty' ? 'true' : 'false';

    $this->getSession()->executeScript(sprintf('
      var form = angular.element(document.getElementById("%s")).controller("form");
      if (form) {
        if (%s) {
          form.$setDirty();
        }
        else {
          form.$setPristine();
        }
      }
      else {
        throw "Form does not exist or have a controller assigned to it."
      }', $form, $dirty));
  }

  /**
   * @when /^Arbitrary script "([^"]*)"$/
   */
  public function arbitraryScript($script) {
    $this->getSession()->evaluateScript($script);
  }

  /**
   * @Then /^I verifying the date picker behaviour$/
   */
  public function iAmVerifyingTheDatePickerBehaviour() {
    $page = $this->getSession()->getPage();
    $page->find('xpath', '//input[@id="edit-biblio-year-coded-0"]')->click();

    $month_picker = $page->find('xpath', '//div[@id="edit-field-biblio-pub-month"]');
    $day_picker = $page->find('xpath', '//div[@id="edit-field-biblio-pub-day"]');

    if (!$month_picker->isVisible() || !$day_picker->isVisible()) {
      throw new Exception('The day and/or month picker was not found on the page.');
    }

    $page->find('xpath', '//input[@id="edit-biblio-year-coded-10000"]')->click();

    $month_picker = $page->find('xpath', '//div[@id="edit-field-biblio-pub-month"]');
    $day_picker = $page->find('xpath', '//div[@id="edit-field-biblio-pub-day"]');

    if ($month_picker->isVisible() || $day_picker->isVisible()) {
      throw new Exception('The day and/or month picker found on the page but they not suppose to.');
    }
  }

  /**
   * @Given /^I create a new publication with a type$/
   */
  public function iCreateANewPublicationWithADatePicker() {
    $this->randomizeMe();
    $this->getSession()->getDriver()->executeScript('CKEDITOR.instances["edit-title-field-und-0-value"].setData("' . $this->randomText . '");');
    $this->getSession()->getPage()->find('xpath', '//input[@id="edit-biblio-year-coded-0"]')->click();
    $this->getSession()->getPage()->find('xpath', '//input[@id="edit-biblio-year"]')->setValue('2010');
    $this->getSession()->getPage()->pressButton('Save');
    $this->assertTextVisible("Forthcoming. “{$this->randomText},” 2010.");
  }

  /**
   * @Then /^I create a new publication with a date picker$/
   */
  public function iEditTheNewPublicationAndChooseAnotherValues() {
    $page = $this->getSession()->getPage();
    $this->randomizeMe();

    $this->getSession()->getDriver()->executeScript('CKEDITOR.instances["edit-title-field-und-0-value"].setData("' . $this->randomText . '");');


    $this->getSession()->getPage()->find('xpath', '//input[@id="edit-biblio-year"]')->setValue('2010');

    $page->find('xpath', '//input[@id="edit-biblio-year-coded-0"]')->click();
    $page->find('xpath', '//div[@id="s2id_edit-field-biblio-pub-month-und"]//a[@class="select2-choice"]')->click();
    $page->find('xpath', '//ul[@class="select2-results"]//li[contains(@class, "select2-result-selectable")][3]')->click();

    $page->find('xpath', '//div[@id="s2id_edit-field-biblio-pub-day-und"]//a[@class="select2-choice"]')->click();
    $page->find('xpath', '//ul[@class="select2-results"]//li[contains(@class, "select2-result-selectable")][3]')->click();

    $this->getSession()->getPage()->pressButton('Save');
    $this->assertTextVisible("“{$this->randomText},” 2010.");
  }

  /**
   * @Given /^I print page screen shot$/
   */
  public function iPrintPageScreenShot() {
    $driver = $this->getSession()->getDriver();
    $screenshot = $driver->getScreenshot();
    $client_id = 'f10ef45787db6fc';
    $request = $this->invokeRestRequest('post', 'https://api.imgur.com/3/image.json',
      ['Authorization' => 'Client-ID ' . $client_id],
      ['image' => base64_encode($screenshot)]
    );
    $json = $request->json();
    print_r('The screen shot of the page is: ' . $json['data']['link']);
  }

  /**
   * @When /^I set the variable "([^"]*)" to "([^"]*)" in the vsite "([^"]*)"$/
   */
  public function iSetVariableInVsite($name, $val, $vsite) {
    FeatureHelp::variableSetSpace($name, $val, $vsite);
  }

  /**
   * @When /^I open the "([^"]*)" settings form for the site "([^"]*)"$/
   */
  public function iNavigateCpSettings($type, $vsite) {

    return array(
      new Step\When('I visit "' . $vsite . '"'),
      new Step\When('I open the admin panel to "Settings"'),
      new Step\When('I open the admin panel to "App Settings"'),
      new Step\When('I click on the "' . trim($type) . '" control in the ".menu-container .simplebar-scroll-content" element'),
    );
  }

  /**
   * @When /^I submit cp settings of the site$/
   */
  public function iSubmitCpSettings() {

    return array(
      new Step\When('I press "Save"'),
      new Step\When('I wait for page actions to complete'),
    );
  }

  /**
   * @When /^I change publication citation style to "([^"]*)"$/
   */
  public function iChangePublicationType($type) {

    return array(
      new Step\When('I click on the "' . trim($type) . '" control'),
    );
  }

  /**
   * @When /^I uncheck the box "([^"]*)" with id "([^"]*)" publication citation filter$/
   */
  public function iChangePublicationFilter($type, $id) {

    $page = $this->getSession()->getPage();
    $element = $page->find('xpath', "//input[@id='{$id}']");
    if ($element->isChecked()) {
        return array(
        new Step\When('I click on the "' . trim($type) . '" control'),
      );
    }
  }

  /**
   * @Given /^I set the default site to "([^"]*)"$/
   */
  public function iSetTheDefaultSiteTo($site) {
    $this->nid = FeatureHelp::getNodeId($site);
  }

  /**
   * @When /^Switch to the iframe "([^"]*)"$/
   */
  public function switchToTheIframe($iframe_id) {
    $this->waitFor(function (FeatureContext $context) {
      if ($overlay = $context->getSession()->getPage()->find('css', 'iframe.media-modal-frame')) {
        $context->getSession()->switchToIframe("mediaStyleSelector");
        return true;
      }
      return false;
    }, 20000);
  }

  /**
   * @When /^I click on the first "([^"]*)" control in the "([^"]*)" element$/
   */
  public function iClickOnFirstControlInElement($text, $css) {
    $page = $this->getSession()->getPage();
    $parents = $page->findAll('css', $css);

    foreach ($parents as $p) {
      if ($p->isVisible()) {
        if ($elem = $p->find('xpath', "//*[text() = '{$text}']")) {
          $elem->click();
          return;
        }
        else {
          throw new ElementNotFoundException($this->getSession(), "No $text found in $css element.");
        }
      }
    }
  }

  /**
   * Click on the element with the provided xpath query
   *
   * @When /^I click on the element with xpath "([^"]*)"$/
   */
  public function iClickOnTheElementWithXPath($xpath) {
    $session = $this->getSession(); // get the mink session
    $element = $session->getPage()->find(
        'xpath',
        $session->getSelectorsHandler()->selectorToXpath('xpath', $xpath)
    ); // runs the actual query and returns the element

    // errors must not pass silently
    if (null === $element) {
        throw new \InvalidArgumentException(sprintf('Could not evaluate XPath: "%s"', $xpath));
    }

    // ok, let's click on it
    $element->click();
  }

  /**
   * Click on the element with the provided CSS Selector
   *
   * @When /^I click on the element with css selector "([^"]*)"$/
   */
  public function iClickOnTheElementWithCSSSelector($cssSelector) {
    $session = $this->getSession();
    $element = $session->getPage()->find(
        'xpath',
        $session->getSelectorsHandler()->selectorToXpath('css', $cssSelector) // just changed xpath to css
    );
    if (null === $element) {
        throw new \InvalidArgumentException(sprintf('Could not evaluate CSS Selector: "%s"', $cssSelector));
    }
    $element->click();
  }

  /*
   * Helper to get unaliased edit path from Drupal URL
   */
  private function _getUnaliasedPathFromAliasPath($url, $vsite) {

    foreach (array("$vsite/", "") as $prefix) {

      $unaliased_path = drupal_lookup_path('source', "$prefix$url");

      if (! $unaliased_path) {
        $nid = $this->_getNodeIdOfUrl("$prefix$url");
        $unaliased_path = "$vsite/node/$nid";
        break;
      } else {
        break;
      }
    }

    return $unaliased_path;
  }

  /**
   * Visit the internal (unaliased) Drupal path of the current page
   *
   * @When /^I visit to delete the post "([^"]*)" on vsite "([^"]*)"$/
   */
  public function iVisitTheDeletePathOfPage($url, $vsite) {
    $unaliased_path = drupal_lookup_path('source', $url);

    # Check the url with the vsite prepended
    if (! $unaliased_path) {
      $unaliased_path = drupal_lookup_path('source', "$vsite/$url");
    }

    if (! $unaliased_path) {
      throw new Exception("Could not find an unaliased path for '$url' on vsite '$vsite'.");
    }

    $this->visit("/$vsite/$unaliased_path/delete");
  }

  /*
   * Helper function to get node id from Drupal aliased URL
   */
  private function _getNodeIdOfUrl($url) {

    $this->visit($url);

    $a_element = $this->getSession()->getPage()->find('xpath', "//a[contains(@href, '?destination=node/') or contains(@href, 'destination%3Dnode%2F')]");
    $page = $this->getSession()->getPage()->getContent();

    if ($a_element) {
      $href_unaliased = $a_element->getAttribute('href');

      if (preg_match("/\bnode(?:\%2f|\/)(\d+)/i", $href_unaliased, $matches)) {
        if (isset($matches[1])) {
          $nid = (int)($matches[1]);
          return $nid;
        }
      }
    }
  }

  /**
   *
   * @When /^I visit the "([^"]*)" form for node "([^"]*)" in site "([^"]*)"$/
   */
  public function iVisitTheFormForNodeInSite($form, $url, $vsite) {
    $path = $this->_getUnaliasedPathFromAliasPath($url, $vsite);
    if (! $path) {
      throw new Exception("Could not find an unaliased path for '$url' on vsite '$vsite'.");
    }
    $this->visit("$path/$form");
  }

  /**
   *
   * @Then /^I can not visit "([^"]*)" form for node "([^"]*)" in group "([^"]*)"$/
   */
  public function iCanNotVisitFormForNodeInGroup($form, $url, $vsite) {
    $path = $this->_getUnaliasedPathFromAliasPath($url, $vsite);
    if (! $path) {
      throw new Exception("Could not find an unaliased path for '$url' on vsite '$vsite'.");
    }
    $this->iCanTVisit("$path/$form");
  }

  /**
   * @When /^I intentionally throw some javascript errors$/
   */
  public function iIntentionallyThrowSomeJsErrors() {
    $BehatConsoleErrorsScript = "
    (function () {
      if (!window.BehatScriptRun) {
        window.BehatScriptRun = true;
        window.BehatConsoleErrors = [];

        window.onerror = function (error, url, line) {
          BehatConsoleErrors.push({error: error, url: url, line: line});
        }
      }
    })();
    ";

    $errorCausingScripts = array(
      "var abc = xyz.ThisPropertyDoesNotExist.NorDoesThisOne;",
      "throw new Error('Something bad happened.');",
    );

    foreach ($errorCausingScripts as $s) {
      $this->getSession()->executeScript($s);
      $this->getSession()->executeScript($BehatConsoleErrors);
      if ($jserrors = $this->getSession()->evaluateScript("return window.BehatConsoleErrors")) {
        print_r($jserrors);
      }
    }
  }

  /**
   * @When /^I visit the unaliased registration path of "([^"]*)" on vsite "([^"]*)" and append "([^"]*)"$/
   */
  public function iVisitTheUnaliasedRegistrationPathOfAndAppend($url, $vsite, $appendage) {
    $unaliased_path = drupal_lookup_path('source', $url);

    # Check the url with the vsite prepended
    if (! $unaliased_path) {
      $unaliased_path = drupal_lookup_path('source', "$vsite/$url");
    }

    if (! $unaliased_path) {
      throw new Exception("Could not find an unaliased path for '$url' on vsite '$vsite' with '$appendage' appended.");
    }

    if (preg_match('/node\/(\d+)/', $unaliased_path, $matches)) {
      if (isset($matches[1])) {
        $nid = $matches[1];
      }
    }

    if (! isset($nid)) {
      throw new Exception("Could not find a node ID via drupal_lookup_path(): $unaliased_path");
    }

    $this->visit("$vsite/os_events/nojs/registration/$nid/$appendage");
  }
  /**
   *
   * @Then /^I should see "([^"]*)" events named "([^"]*)" over the next "([^"]*)" pages$/
   *
   */
  public function iShouldSeeNEventsNamedXyzOverTheNextNDateUnits($num_events, $event_title, $num_intervals) {

    $num_events_counted = 0;

    $counter = 0;
    while ($counter++ <= $num_intervals) {
      $num_events_counted +=
        count($this->getSession()->getPage()->findAll('xpath',
          "//div[@class='calendar-calendar']//td[starts-with(@id, 'os_events-')]//span[@class='field-content']/a[text()='$event_title']"));

      $page = $this->getSession()->getPage()->getContent();
      $date_next_arrow = $this->getSession()->getPage()->find('xpath', "//section[@id='main-content']//li[@class='date-next']/a");
      $date_next_arrow->click();
    }

    $counter = 0;
    if ($num_events_counted == (int)$num_events) {

      # Return to today's calendar page
      while ($counter++ <= $num_intervals) {
        $date_prev_arrow = $this->getSession()->getPage()->find('xpath', "//section[@id='main-content']//li[@class='date-prev']/a");
        $date_prev_arrow->click();
      }
      return true;
    }
    throw new Exception("Found $num_events_counted events, but expected $num_events.\n");
  }

  /**
   *
   * @Given /^I fill in "([^"]*)" with date interval "([^"]*)" from "([^"]*)"$/
   *
   */
  public function iFillInFieldWithDateInterval($element_id, $date_interval, $start_date) {
    $now = new DateTime($start_date);
    $future_date = $now->add(new DateInterval($date_interval))->format("M d Y");
    return new Step\When("I fill in \"$element_id\" with \"$future_date\"");
  }

  /**
   * @Given /^I fill in the "([^"]*)" "([^"]*)" field under "([^"]*)" with date interval "([^"]*)" from "([^"]*)"$/
   */
  public function iFillInTheNthFieldBelowXyzWithDateInterval($nth, $field_type, $field_under_text, $date_interval, $start_date) {
    $element = $this->_getNthFieldBelowXyz($nth, $field_type, $field_under_text);
    $future_date = $this->_getDateInterval($start_date, $date_interval);
    $element->setValue($future_date);
  }

  /**
   * @Given /^I fill in the "([^"]*)" "([^"]*)" field above the "([^"]*)" "([^"]*)" with date interval "([^"]*)" from "([^"]*)"$/
   */
  public function iFillInTheNthFieldAboveXyzWithDateInterval($nth, $field_type1, $field_under_text, $field_type2, $date_interval, $start_date) {
    $element = $this->_getNthFieldAboveXyz($nth, $field_type1, $field_under_text, $field_type2);
    $future_date = $this->_getDateInterval($start_date, $date_interval);
    $element->setValue($future_date);
  }

  /**
   * @Given /^I fill in the "([^"]*)" "([^"]*)" field within the "([^"]*)" section with date interval "([^"]*)" from "([^"]*)"$/
   */
  public function iFillInTheNthFieldWithinXyzWithDateInterval($nth, $field_type1, $field_within_text, $date_interval, $start_date) {
    $element = $this->_getNthFieldWithinXyz($nth, $field_type1, $field_within_text, $field_type2);
    $future_date = $this->_getDateInterval($start_date, $date_interval);
    $element->setValue($future_date);
  }

  /*
   * Helper function to convert ordinal number (nth) to cardinal number (n)
   */
  private function _ordinal_to_cardinal($nth) {
    $nth_index = preg_replace("/(st|nd|th)/i", "", $nth);

    if (! preg_match("/\d+/", $nth_index)) {
      throw new Exception("Expected an ordinal number, e.g., 1st, 22nd, 1457th), but did not find one.");
    }

    return (int)$nth_index - 1;
  }

  /**
   * Helper function to get an input element under a div label
   */
  private function _getNthFieldBelowXyz($nth, $field_type, $field_under_text) {
    $page = $this->getSession()->getPage();
    $nth_index = $this->_ordinal_to_cardinal($nth);
    $xpath_expr = "//label[contains(text(), '$field_under_text')]/..//input[@type='$field_type']";
    $elements = $page->findAll('xpath', $xpath_expr);

    if (isset($elements[$nth_index])) {
      return $elements[$nth_index];
    } else {
      throw new Exception("XPath expression not found at the $nth index: '$xpath_expr'.");
    }
  }

  /**
   * Helper function to get an input element within a div label
   */
  private function _getNthFieldWithinXyz($nth, $field_type, $field_within_text) {
    $page = $this->getSession()->getPage();
    $nth_index = $this->_ordinal_to_cardinal($nth);
    $xpath_expr = "//label[contains(text(), '$field_within_text')]/../input[@type='$field_type']";
    $elements = $page->findAll('xpath', $xpath_expr);

    if (isset($elements[$nth_index])) {
      return $elements[$nth_index];
    } else {
      throw new Exception("XPath expression not found at the $nth index: '$xpath_expr'.");
    }
  }

  /**
   * Helper function to get an input element above another element
   */
  private function _getNthFieldAboveXyz($nth, $field_type1, $field_above_text, $field_type2) {
    $page = $this->getSession()->getPage();
    $nth_index = $this->_ordinal_to_cardinal($nth);
    $xpath_expr = "//input[@type='$field_type2'][@value='$field_above_text']/..//input[@type='$field_type1']";
    $elements = $page->findAll('xpath', $xpath_expr);

    if (isset($elements[$nth_index])) {
      return $elements[$nth_index];
    } else {
      throw new Exception("XPath expression not found at the $nth index: '$xpath_expr'.");
    }
  }

  /**
   * @Then /^I should "([^"]*)" event named "([^"]*)" on date "([^"]*)" from "([^"]*)" over the next "([^"]*)" pages$/
   */
  public function iShouldSeeTheEventNamedOnDateIntervalFrom($see_or_not, $event_name, $date_interval, $start_date, $num_intervals) {

    $counter = 0;
    $success = false;
    while ($counter++ <= $num_intervals) {
      $page = $this->getSession()->getPage()->getContent();
      $future_date = $this->_getDateInterval($start_date, $date_interval);
      $xpath_expr =  "//td[@id='os_events-$future_date-0']//a[text()='$event_name']";
      $event_on_date = $this->getSession()->getPage()->findAll('xpath', $xpath_expr);

      switch($see_or_not) {
        case "see":
          if ($event_on_date) {
            $success = true;
            break;
          }
          $msg = "The event '$event_name' was not seen on '$future_date', but should have been there.";
        case "not see":
          if (! $event_on_date) {
            $success = true;
            break;
          }
          $msg = "The event '$event_name' was seen on '$future_date', but should not have been there.";
        default:
          throw new Exception("Invalid parameter.  Expected 'I should \"see\" ...' or 'I should \"not see\" ...'.");
      }
    }

    if (! $success) {
      throw new Exception($msg);
    }

    # Return calendar to home month
    $counter = 0;
    while ($counter++ <= $num_intervals) {
      $date_prev_arrow = $this->getSession()->getPage()->find('xpath', "//li[@class='date-prev']/a");
      $date_prev_arrow->click();
    }
  }

  /*
   * Helper function to perform a date increment, and return a date string 
  */
  private function _getDateInterval($start_date = "now", $date_interval) {
    $now = new DateTime($start_date);
    $future_date = $now->add(new DateInterval($date_interval))->format("M d Y");
    return $future_date;
  }

  /**
   *
   * @Given /^I select the radio button On Until Date E.g., "([^"]*)" with the id "([^"]*)"$/
   *
   */
  public function iSelectTheRadioButtonOnUntilDateMdyWithTheId($eg_date_format, $element_id) {
    $now = new DateTime();
    return new Step\When('I select the radio button "On Until Date E.g., ' . $now->format($eg_date_format) . '" with the id "' . $element_id . '"');
  }

  /**
   *
   * @Given /^I focus on "([^"]*)" element "([^"]*)", and press key "([^"]*)"$/
   *
   */
  public function iFocusOnElementAndPressKey($type, $expr, $char) {
    $elem = $this->getSession()->getPage()->find($type, $expr);
    $elem->focus();
    $this->getSession()->getDriver()->keyDown($expr, $char);
    $this->getSession()->getDriver()->keyUp($expr, $char);
  }

  /**
   *
   * @Given /^I focus on "([^"]*)" element "([^"]*)"$/
   *
   */
  public function iFocusOnElement($type, $expr) {
    $elem = $this->getSession()->getPage()->find($type, $expr);
    $elem->focus();
  }

  /**
   * @When /^I click the gear icon in the content region$/
   */
  public function iClickTheGearIconInTheContentRegion() {
    list ($content_region, $gear_icon, $gear_icon_trigger_link) = $this->_getGearIconInContentRegion();

    $content_region->mouseOver();
    $content_region->click();
    $gear_icon->mouseOver();
    $gear_icon->click();
    $gear_icon_trigger_link->mouseOver();
    $gear_icon_trigger_link->click();
  }

  /**
   * @When /^I click the gear icon in the node content region$/
   */
  public function iClickTheGearIconInTheNodeContentRegion() {
    $content_region = $this->getSession()->getPage()->find('xpath', "//div[@class='node-content']");
    $gear_icon = $this->getSession()->getPage()->find('xpath', "//div[@class='contextual-links-wrapper contextual-links-processed']");
    $gear_icon_trigger_link = $this->getSession()->getPage()->find('xpath', "//div[@id='content']//div/a[text()='Configure']");

    $content_region->mouseOver();
    $content_region->click();
    $gear_icon->mouseOver();
    $gear_icon->click();
    $gear_icon_trigger_link->mouseOver();
    $gear_icon_trigger_link->click();
  }
  /**
   * @When /^I should "([^"]*)" see the "([^"]*)" menu item in the gear menu$/
   */
  public function iDoNotSeeTheMenuItemUnderTheGearMenu($negation, $menu_label) {
    list ($content_region, $gear_icon, $gear_icon_trigger_link) = $this->_getGearIconInContentRegion();

    if (! $gear_icon) {
      return;
    }

    $gear_menu_item = $this->getSession()->getPage()->find('xpath', "//div[@class='contextual-links-wrapper contextual-links-processed']/ul/li/a[text()='$menu_label']");

    if ($negation) {
      if (! $gear_menu_item) {
        return;
      } else {
        throw new Exception("Found menu item '$menu_label', but should not have.");
      }
    }
  }

  private function _getGearIconInContentRegion() {
    $content_region = $this->getSession()->getPage()->find('xpath', "//div[@id='content']");
    $gear_icon = $this->getSession()->getPage()->find('xpath', "//div[@class='contextual-links-wrapper contextual-links-processed']");
    $gear_icon_trigger_link = $this->getSession()->getPage()->find('xpath', "//div[@id='content']//div/a[text()='Configure']");

    return array($content_region, $gear_icon, $gear_icon_trigger_link);
  }

  /**
   * @Given /^I visit the "([^"]*)" parameter in the current query string with "([^"]*)" appended on vsite "([^"]*)"$/
   */
  public function iVisitTheParameterInTheCurrentQueryString($parameter, $appendage, $vsite) {

    $url = $this->getSession()->getCurrentUrl();
    if (preg_match("/$parameter(?:=|%3d)(\S+)/i", $url, $matches)) {

      if (isset($matches[1])) {
        $this->getSession()->visit($this->locatePath((($vsite) ? "/$vsite/" : "") . rawurldecode($matches[1]) . (($appendage) ? "/$appendage" : "")));
      } else {
        throw new Exception("Could not get a $parameter.\n");
      }
    }
  }


  /**
   * @Given /^I click "([^"]*)" in the gear menu$/
   */
  public function iClickInTheGearMenu($menu_item) {
    $gear_menu_item = $this->getSession()->getPage()->find('xpath', "//div[@id='content']//div/a[text()='Configure']/..//a[text()='$menu_item']");
    $gear_menu_item->click();
  }

  /**
   * @Given /^I click "([^"]*)" in the gear menu in node content$/
   */
  public function iClickInTheGearMenuNodeContent($menu_item) {
    $gear_menu_item = $this->getSession()->getPage()->find('xpath', "//div[@class='node-content']//div[@class='contextual-links-wrapper contextual-links-processed contextual-links-active']/..//a[text()='$menu_item']");
    try {
      $gear_menu_item->click();
    }
    catch (Exception $e) {
      throw new Exception($menu_item . " is not clickable");
    }
  }


  /**
   * @When /^I swap the order of the first two items in the outline on vsite "([^"]*)"$/
   */
  public function iSwapTheOrderOfTheBookOutline($vsite) {
    $this->iClickTheGearIconInTheContentRegion();
    $this->iClickInTheGearMenu("Outline");
    $this->iVisitTheParameterInTheCurrentQueryString("destination", "outline", $vsite);

    $handles = $this->getSession()->getPage()->findAll('xpath', "//div[@class='handle']");

    if (sizeof($handles) > 1) {
      $handles[0]->dragTo($handles[1]);
    } else {
      throw new Exception("There needs to be at least two book entries to test re-ordering.\n");
    }

    return array(
      new Step\When('I press "Save Booklet Outline"'),
    );
  }

  /**
   * @Given /^I visit the parent directory of the current URL$/
   */
  public function iVisitParentDirectory() {
    $url = $this->getSession()->getCurrentUrl();
    $this->getSession()->visit($this->locatePath($url . '/..'));
  }

  /**
   * @Then /^I should see breadcrumb "([^"]*)"$/
   *
   */
  public function iShouldSeeBreadcrumb($breadcrumb) {

    $page = $this->getSession()->getPage()->getContent();

    # Ignore HTML tags between breadcrumb separators
    $breadcrumb_pattern = preg_replace("/\s+\/\s+/", ".*\/.*", $breadcrumb);

    if (preg_match("/$breadcrumb_pattern/", $page)) {
      return true;
    }

    return false;
  }

  /**
   * @When /^I edit the media element "([^"]*)"$/
   */
  public function iClickToEditTheMedia($filename) {
    $fid = FeatureHelp::getEntityID('file', $filename);
    $file = file_load($fid);
    return array(
      new Step\When('I visit "john/file/' . $fid . '/edit"'),
    );
  }

  /**
   * @Then /^I should see disqus$/
   */
  public function iShouldSeeDisqus() {

    $page = $this->getSession()->getPage()->getContent();
    $element = $this->getSession()->getPage()->find('css', "div#disqus_thread");

    if ($element) {
      return;
    }

    throw new Exception("Did not find disqus panel.\n");
  }

  /**
   * @Given /^I add a existing sub page named "([^"]*)" under the page "([^"]*)"$/
   */
  public function iAddExistingSubPageUnderPage($child_title, $parent_title) {
    $nid = FeatureHelp::getNodeId($parent_title);
    return array(
      new Step\When('I visit "john/os/pages/' . $nid . '/subpage' . '"'),
    );
  }

  /**
   * @When /^I visit the file "([^"]*)"$/
   */
  public function iVisitTheFile($filename) {
    $fid = FeatureHelp::getEntityID('file', $filename);
    return array(
      new Step\When('I visit "john/file/' . $fid . '"'),
    );
  }
    /**
   * @When /^I delete the media element "([^"]*)"$/
   */
  public function iClickToDeleteTheMedia($filename) {
    $fid = FeatureHelp::getEntityID('file', $filename);
    $file = file_load($fid);
    return array(
      new Step\When('I visit "john/file/' . $fid . '/delete"'),
      new Step\When('I press "Delete"'),
    );
  }

  /**
   * @Given /^I fill in the field "([^"]*)" with the page "([^"]*)"$/
   *
   * This step is used to fill in an autocomplete field.
   */
  public function iFillInTheFieldWithThePage($id, $title) {
    $nid = FeatureHelp::getNodeId($title);
    $element = $this->getSession()->getPage();
    $value = $title . ' [' . $nid . ']';
    $element->fillField($id, $value);
  }

  /**
   * @When /^I click the gear icon in the section navigation widget$/
   */
  public function iClickTheGearIconInTheSectionNavigation() {
    $content_region = $this->getSession()->getPage()->find('xpath', "//div[@id='block-boxes-os-pages-section-nav']");
    $gear_icon = $this->getSession()->getPage()->find('xpath', "//div[@class='contextual-links-wrapper contextual-links-processed']");
    $gear_icon_trigger_link = $this->getSession()->getPage()->find('xpath', "//div[@id='block-boxes-os-pages-section-nav']//div/a[text()='Configure']");

    $content_region->mouseOver();
    $content_region->click();
    $gear_icon->mouseOver();
    $gear_icon->click();
    $gear_icon_trigger_link->mouseOver();
    $gear_icon_trigger_link->click();
  }

  /**
   * @Given /^I visit the "([^"]*)" parameter in the current page query string with "([^"]*)" appended on vsite "([^"]*)"$/
   */
  public function iVisitTheParameterInTheCurrentPageQueryString($parameter, $appendage, $vsite) {

    $url = $this->getSession()->getCurrentUrl();
    if (preg_match("/$parameter(?:=|%3d)node\/(\S+)/i", $url, $matches)) {

      if (isset($matches[1])) {
        $this->getSession()->visit($this->locatePath((($vsite) ? "/$vsite/os/pages/" : "") . rawurldecode($matches[1]) . (($appendage) ? "/$appendage" : "")));
      } else {
        throw new Exception("Could not get a $parameter.\n");
      }
    }
  }

  /**
   * @Given /^I click "([^"]*)" in the gear menu of section navigation$/
   */
  public function iClickInTheGearMenuOfSectionNav($menu_item) {
    $gear_menu_item = $this->getSession()->getPage()->find('xpath', "//div[@id='block-boxes-os-pages-section-nav']//div/a[text()='Configure']/..//a[text()='$menu_item']");
    $gear_menu_item->click();
  }

  /**
   * @When /^I swap the order of the subpages under the page "([^"]*)"$/
   */
  public function iSwapTheOrderOfTheSubpage($page_title) {
    $nid = FeatureHelp::getNodeId($page_title);
    $this->Visit('john/os/pages/' . $nid . '/outline');
    $this->adminPanelClosed();

    $handles = $this->getSession()->getPage()->findAll('xpath', "//div[@class='handle']");

    if (sizeof($handles) > 1) {
      $handles[0]->dragTo($handles[1]);
    } else {
      throw new Exception("There needs to be at least two subpage entries to test re-ordering.\n");
    }

    return array(
      new Step\When('I press "Save Section Outline"'),
    );
  }

  /**
   * @When /^I click on "([^"]*)" button in the wysiwyg editor$/
   */
  public function iClickOnEditor($class) {
    $element = $this->getSession()->getPage()->find('xpath', "//*[contains(@class, '{$class}')]");
    $element->click();
  }
 
  /**
   * @When /^I visit the absolute path "([^"]*)"$/
   */
  public function iVisitTheAbsolutePath($path) {
    $this->getSession()->visit($path);
    $content = $this->getSession()->getPage()->getContent();
    var_dump($content);
  }
}
