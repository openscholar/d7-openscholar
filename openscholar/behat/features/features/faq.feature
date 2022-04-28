Feature:
  Testing the faq app.

  @api @features_first
  Scenario: Testing the migration of FAQ
    Given I am logging in as "john"
      And I visit "john/faq"
      And I should see "What does JFK stands for?"
     When I click "What does JFK stands for?"
     Then I should see "JFK stands for: John Fitzgerald Kennedy."

  @api @features_first
  Scenario: Testing the migration of FAQ
    Given I am logging in as "john"
      And I visit "john/node/add/faq"
      And I fill "edit-title" with random text
      And I press "Save"
     Then I should see the random string

  @api @features_first
  Scenario: Verify that the FAQ's body in the LOP is collapsed by default.
    Given I am logging in as "john"
      And the widget "List of posts" is set in the "FAQ" page with the following <settings>:
          | Content Type               | FAQ         | select list |
          | Display style              | Teaser      | select list |
     When I visit "john/faq"
     Then I should see that the "faq" in the "LOP" are collapsed


  @api @features_first
  Scenario: Verify that body length limits are respected
    Given I am logging in as "john"
      And I set the variable "os_wysiwyg_maximum_length_body" to "50"
      And I visit "john/node/add/faq"
     When I fill in "edit-title" with "Gonna fail"
      And I fill in "edit-body-und-0-value" with "01234567890123456789012345678901234567890123456789AAAAAA"
      And I press "Save"
     Then I should see "Answer cannot be longer than 50 characters but is currently 56 characters long."
      And I delete the variable "os_wysiwyg_maximum_length_body"
  
  @api @features_first
  Scenario: Create new faq content
     Given I am logging in as "john"
        And I visit "john/node/add/faq"
       When I fill in "Question" with "Frequently Asked"
       When I fill in "Answer" with "Answer Cleared"
        And I press "Save"
        And I sleep for "2"
       Then I should see "Frequently Asked"
       And I should see "Answer Cleared"

  @api @features_first
  Scenario: Default Creation date descending
    Given I am logging in as "john"
      And I set the variable "faq_sort" to "created" in the vsite "john"
     When I visit "john/faq"
     Then I should see the FAQ "Frequently Asked" comes before "What does JFK stands for?"

  @api @features_first
  Scenario: Creation date ascending
    Given I am logging in as "john"
      And I set the variable "faq_sort" to "created_asc" in the vsite "john"
     When I visit "john/faq"
     Then I should see the FAQ "Where does JFK born?" comes before "Frequently Asked"

  @api @features_first
  Scenario: Edit faq content
     Given I am logging in as "john"
       And I edit the node "Frequently Asked" in the group "john"
       And I sleep for "2"
      When I fill in "Question" with "Frequently Asked Revised"
       And I press "Save"
      Then I should see "Frequently Asked Revised"

  @api @features_first
  Scenario: Delete existing faq content
     Given I am logging in as "john"
       And I edit the node "Frequently Asked Revised" in the group "john"
       And I sleep for "2"
      When I click "Delete this faq"
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
      And I visit "john/node/add/faq"
     When I fill in "Question" with "When was JFK born?"
      And I press "Save"
     Then I should see "When was JFK born?"

  @api @feature_second
  Scenario: Permission to edit own content
    Given I am logging in as "michelle"
     And I edit the node "When was JFK born?" in the group "john"
     When I fill in "Answer" with "29 May 1917"
      And I press "Save"
     Then I should see "29 May 1917"

  @api @feature_second
  Scenario: Permission to edit any content
    Given I am logging in as "alexander"
     And I edit the node "When was JFK born?" in the group "john"
     Then I should see "Access Denied"

  @api @feature_second
  Scenario: Permission to delete any content
    Given I am logging in as "alexander"
     And I edit the node "When was JFK born?" in the group "john"
     Then I should see "Access Denied"

  @api @feature_second
  Scenario: Permission to delete own content
    Given I am logging in as "michelle"
      And I edit the node "When was JFK born?" in the group "john"
      And I click "Delete this faq"
     Then I should see "This action cannot be undone."
      And I press "Delete"
     Then I should see "has been deleted"

  @api @features_second @javascript
  Scenario: Administer FAQ App Setting
     Given I am logging in as "john"
      When I visit "john"
       And I make sure admin panel is open
      When I open the admin panel to "Settings"
       And I open the admin panel to "App Settings"
       And I open the admin panel to "FAQs"
      When I sleep for "2"
      Then I should see "Choose how FAQs will display"

  @api @features_second
  Scenario: Default disable question/answer slider behavior
    Given I am logging in as "john"
      And I set the variable "os_faq_disable_toggle" to "TRUE" in the vsite "john"
     When I visit "john/faq"
     Then I should see "JFK stands for: John Fitzgerald Kennedy."

  @api @features_second
  Scenario: Orderby Alphanumeric ascending
    Given I am logging in as "john"
      And I set the variable "faq_sort" to "title" in the vsite "john"
     When I visit "john/faq"
     Then I should see the FAQ "What does JFK stands for?" comes before "Where does JFK born?"

  @api @features_second
  Scenario: Orderby Alphanumeric descending
    Given I am logging in as "john"
      And I set the variable "faq_sort" to "title_desc" in the vsite "john"
     When I visit "john/faq"
     Then I should see the FAQ "Where does JFK born?" comes before "What does JFK stands for?"