Feature:
  Testing the metatags.

  @api @misc_second
  Scenario: Testing default metatags.
    Given I visit "john/about"
     Then I should see the meta tag "description" with value "Page about john"

  @api @misc_second
  Scenario: Testing custom metatags.
    Given I am logging in as "john"
      And I edit the node "about" in the group "john"
      And I fill in "Meta description" with "custom tag value"
      And I press "Save"
     Then I visit "john/about"
      And I should see the meta tag "og:description" with value "custom tag value"

  @api @misc_second
  Scenario: Testing metatags settings form in a personal site.
    Given I am logging in as "john"
     When I go to "john/cp/settings"
     Then I should see "Site title"
      And I should see "Meta description"
      And I should see "Favicon"
      And I should see "Publisher URL"
      And I should see "Author URL"

  @javascript @misc_second
  Scenario: Testing metatags settings functionality in a personal site.
    Given I am logging in as "john"
      And I visit "john"
     When I open the admin panel to "Settings"
      And I open the admin panel to "Global Settings"
      And I click on the "Search Engine Optimization (SEO)" control
     Then I should see "Publisher URL"
      And I should see "Author URL"
      And I fill in "Meta description" with "meta description by john"
      And I fill in "Site title" with "John's site"
      And I press "Save"
      And I wait for page actions to complete
      And I visit "john"
     Then I should see the meta tag "description" with value "meta description by john"
      And I should see "John's site"
          # Change site title back to "John"
      And I change site title to "John" in the site "john"

  @javascript @misc_second
  Scenario: Does the favicon form open?
    Given I am logging in as "john"
      And I visit "john"
     When I open the admin panel to "Appearance"
      And I click on the "Favicon" control
     Then I should see "A 16x16 .png file to be displayed in browser shortcut icons"

  @api @misc_second
  Scenario: Testing metatags settings form in a department site.
    Given I am logging in as "alexander"
     When I go to "edison/cp/settings"
     Then I should see "Site title"
      And I should see "Meta description"
      And I should see "Favicon"
      And I should see "Publisher URL"
      And I should not see "Author URL"

  @api @misc
  Scenario: Testing metatags settings functionality in a department site.
    Given I am logging in as "alexander"
     When I go to "edison/cp/settings"
      And I fill in "Meta description" with "meta description by alexander"
      And I fill in "Site title" with "Edison's site"
      And I press "edit-submit"
      And I visit "edison"
     Then I should see the meta tag "description" with value "meta description by alexander"
      And I should see "Edison's site"
          # Change site title back to "Edison"
      And I change site title to "Edison" in the site "edison"
