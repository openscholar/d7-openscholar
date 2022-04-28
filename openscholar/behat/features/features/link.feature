Feature:
  Testing the link tab.

  @api @features_second
  Scenario: Test the Links tab
    Given I visit "john"
     When I click "Links"
     Then I should see "JFK wikipedia page"

  @api @features_first
  Scenario: Create new link content
     Given I am logging in as "john"
        And I visit "john/node/add/link"
       When I fill in "Title" with "Google"
       When I fill in "edit-field-links-link-und-0-url" with "https://www.google.com"
        And I press "Save"
        And I sleep for "2"
       Then I should see "Google"

  @api @features_first
  Scenario: Edit link content
     Given I am logging in as "john"
       And I edit the node "Google" in the group "john"
       When I fill in "Title" with "Google_one"
       When I fill in "edit-field-links-link-und-0-url" with "https://www.google.com"
        And I press "Save"
        And I sleep for "2"
       Then I should see "Google_one"

  @api @features_first
  Scenario: Delete link content
     Given I am logging in as "john"
      And I edit the node "Google_one" in the group "john"
      When I click "Delete this link"
      Then I should see "This action cannot be undone."
       And I press "Delete"
      Then I should see "has been deleted"

  @api @feature_second
  Scenario: Permission to add Content
    Given I am logging in as "john"
      And I visit "john/cp/users/add"
      And I fill in "Member" with "alexander"
      And I press "Add member"
      And I sleep for "5"
     Then I should see "alexander has been added to the group John."
      And I visit "john/cp/users/add"
      And I fill in "Member" with "michelle"
      And I press "Add member"
      And I sleep for "5"
     Then I should see "michelle has been added to the group John."
      And I visit "user/logout"
    Given I am logging in as "michelle"
      And I visit "john/node/add/link"
     When I fill in "Title" with "Issac Newton"
     When I fill in "edit-field-links-link-und-0-url" with "https://en.wikipedia.org/wiki/Isaac_Newton"
      And I press "Save"
      And I sleep for "2"
     Then I should see "Issac Newton"

  @api @feature_second
  Scenario: Permission to edit own content
    Given I am logging in as "michelle"
      And I edit the node "Issac Newton" in the group "john"
      And I fill in "Title" with "Sir Issac Newton"
      And I press "Save"
     Then I should see "Sir Issac Newton"

  @api @feature_second
  Scenario: Permission to edit any content
    Given I am logging in as "alexander"
     And I edit the node "Sir Issac Newton" in the group "john"
     Then I should see "Access Denied"

  @api @feature_second
  Scenario: Permission to delete any content
    Given I am logging in as "alexander"
      And I open the delete form for the post "links/issac-newton" on vsite "john"
     Then I should see "Access Denied"

  @api @feature_second
  Scenario: Permission to delete own content
    Given I am logging in as "michelle"
      And I edit the node "Sir Issac Newton" in the group "john"
      And I click "Delete this link"
     Then I should see "This action cannot be undone."
      And I press "Delete"
     Then I should see "has been deleted"
