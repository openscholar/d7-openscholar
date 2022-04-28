Feature:
  Testing the page functionality.

  @api @features_first
  Scenario: Add a page content.
    Given I am logging in as "john"
      And I visit "john/node/add/page"
      And I fill in "Title" with "Page One"
      And I fill in "Body" with "New Page for testing"
      And I press "Save"
     Then I should see "Page One"
     Then I should see "New Page for testing"

  @api @features_first
  Scenario: Edit existing page content.
    Given I am logging in as "john"
      And I edit the node "Page One" in the group "john"
      And I fill in "Title" with "Parent Page"
      And I press "Save"
     Then I should see "Parent Page"

  @api @features_first @javascript
  Scenario: Change order of subpages content using "Section Outline"
    Given I am logging in as "john"
      And I create a sub page named "Subpage One" under the page "Parent Page"
      And I create a sub page named "Subpage Two" under the page "Parent Page"
      And I visit the site "john/page-one"
      And I swap the order of the subpages under the page "Parent Page"
     Then I should see "Updated book Parent Page"
      And I visit the site "john/page-one"
     Then I should match the regex "parent\s+page\s+subpage\s+two\s+subpage\s+one"

  @api @features_first
  Scenario: Add existing subpage
    Given I am logging in as "john"
      And I add a existing sub page named "Parent Page" under the page "About"
      And I fill in the field "edit-add-page" with the page "Parent Page"
      And I press "Save"
      And I visit "john/page-one"
     Then I should see "HOME / ABOUT /"

  @api @features_first
  Scenario: Correct rearrangement of section outline when parent is deleted.
    Given I am logging in as "john"
      And I create a sub page named "Child One" under the page "Subpage One"
      And I edit the node "Parent Page" in the group "john"
      And I click "Delete this page"
      And I press "Delete"
      And I visit the site "john/child-one"
     Then I should see "HOME / ABOUT / SUBPAGE ONE /"

  @api @features_first
  Scenario: Delete existing page
    Given I am logging in as "john"
      And I edit the node "Child One" in the group "john"
      And I click "Delete this page"
     Then I should see "This action cannot be undone."
      And I press "Delete"
     Then I should see "has been deleted"

  @api @features_second
  Scenario: Permission to add page Content
    Given I am logging in as "john"
      And I visit "john/cp/users/add"
      And I fill in "Member" with "alexander"
      And I press "Add member"
      And I sleep for "5"
      And I visit "john/cp/users/add"
      And I fill in "Member" with "michelle"
      And I press "Add member"
      And I sleep for "5"
      And I visit "user/logout"
    Given I am logging in as "michelle"
      And I visit "john/node/add/page"
      And I fill in "Title" with "About Michelle"
      And I press "Save"
     Then I should see "About Michelle"

  @api @features_second
  Scenario: Permission to edit own page content
    Given I am logging in as "michelle"
      And I edit the node "About Michelle" in the group "john"
      And I fill in "Title" with "About Michelle Obama"
      And I press "Save"
     Then I should see "About Michelle Obama"

  @api @features_second
  Scenario: Permission to edit any page content
    Given I am logging in as "alexander"
      And I edit the node "About Michelle Obama" in the group "john"
     Then I should see "Access Denied"

  @api @features_second
  Scenario: Permission to delete any page content
    Given I am logging in as "alexander"
      And I visit to delete the post "about-michelle" on vsite "john"
     Then I should not see "About Michelle"

  @api @features_second
  Scenario: Permission to delete own page content
    Given I am logging in as "michelle"
      And I edit the node "About Michelle Obama" in the group "john"
      And I click "Delete this page"
     Then I should see "This action cannot be undone."
      And I press "Delete"
     Then I should see "has been deleted"
