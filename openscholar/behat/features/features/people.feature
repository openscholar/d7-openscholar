Feature:
  Testing the people tab.

  @api @features_second
  Scenario: Test the People tab
    Given I visit "john"
     When I click "People"
      And I click "John Fitzgerald Kennedy"
     Then I should see "often referred to by his initials JFK"

  @api @features_second
  Scenario: Testing the autocomplete field or profile syncing.
    Given I am logging in as "john"
      And I visit "john/cp/people/sync-profiles"
      And I fill in the field "autocomplete" with the node "Hillary Diane Rodham Clinton"
      And I press "Submit"
      And I should see "The person Hillary Diane Rodham Clinton has created. You can visit their page."
     When I click "visit"
     Then I should see "Hillary Diane Rodham Clinton"
      And I should see "67th United States Secretary of State"
    # Verify the user is in john's vsite and the source node vsite.
      And I should see "John"

  @api @features_second
  Scenario: When syncing the same node we need to check we updated the copied
  node and create a new one.
#    Given I am logging in as "john"
    # try to solve with john later on.
    Given I am logging in as "john"
    Given I am logging in as "john"
      And I edit the node "Hillary Diane Rodham Clinton" in the group "obama"
      And I fill in "Address" with "White house"
      And I press "Save"
      And I visit "john/cp/people/sync-profiles"
      And I fill in the field "autocomplete" with the node "Hillary Diane Rodham Clinton"
      And I press "Submit"
      And I should see "The person Hillary Diane Rodham Clinton has updated. You can visit their page."
     When I click "visit"
     Then I should see "Hillary Diane Rodham Clinton"
      And I should see "67th United States Secretary of State"
    # Verify the user is in john's vsite and the source node vsite.
      And I should see "John"
      And I should see "White house"

  @api @features_second
  Scenario: When syncing the same node we need to check we updated the copied
            node and create a new one.
#    Given I am logging in as "john"
    # try to solve with john later on.
    Given I am logging in as "john"
    Given I am logging in as "john"
      And I edit the node "Hillary Diane Rodham Clinton" in the group "obama"
      And I fill in "Address" with "White house"
      And I press "Save"
      And I visit "john/cp/people/sync-profiles"
      And I fill in the field "autocomplete" with the node "Hillary Diane Rodham Clinton"
      And I press "Submit"
      And I should see "The person Hillary Diane Rodham Clinton has updated. You can visit their page."
     When I click "visit"
     Then I should see "Hillary Diane Rodham Clinton"
      And I should see "67th United States Secretary of State"
    # Verify the user is in john's vsite and the source node vsite.
      And I should see "John"
      And I should see "White house"

  @api @features_second
  Scenario: Empty the value of a field from the original node and check the
            listener node updated.
    Given I am logging in as "john"
      And I edit the node "Hillary Diane Rodham Clinton" in the group "obama"
     When I fill in "Address" with ""
      And I press "Save"
      And I sleep for "10"
      And I visit "john/people/hillary-diane-rodham-clinton"
     Then I should not see "new address"

  @api @features_second
  Scenario: Make sure that when a source node is deleted the copied node becomes
            editable.
    Given I am logging in as "john"
      And I visit "john/people/hillary-diane-rodham-clinton"
      And I edit the node "Hillary Diane Rodham Clinton" in the group "john"
      And I should not see "Professional Titles/Affiliations"
          # Delete the original node.
     When I delete the node of type "person" named "Hillary Diane Rodham Clinton" in the group "obama"
      And I edit the node "Hillary Diane Rodham Clinton" in the group "john"

  @api @features_second
  Scenario: Test changing the owner of a VSite.
    Given I am logging in as "admin"
      And I give the user "john" the role "vsite admin" in the group "obama"
     When I am logging in as "john"
      And I edit the membership of "michelle" in vsite "obama"
      And I check the box "Set as site owner"
      And I press "Save"
      And I verify that "michelle" is the owner of vsite "obama"
      And I should verify that the user "michelle" has a role of "vsite admin" in the group "obama"
     Then I am logging in as "michelle"
      And I edit the membership of "john" in vsite "obama"
      And I check the box "Set as site owner"
      And I press "Save"
      And I verify that "john" is the owner of vsite "obama"
      And I should verify that the user "john" has a role of "vsite admin" in the group "obama"

  @api
  Scenario: Test changing the owner of a VSite via the "change owner" link.
    Given I am logging in as "john"
      And I visit "obama/cp/users"
      And I click "Change owner"
     When I select "michelle" from "User name"
      And I press "Save"
     Then I should see "The user michelle is now the owner of the site Obama."
      And I verify that "michelle" is the owner of vsite "obama"
      And I should verify that the user "john" has a role of "vsite admin" in the group "obama"
          # Return "john" to be the site owner.
     When I am logging in as "michelle"
      And I visit "obama/cp/users"
      And I click "Change owner"
     When I select "john" from "User name"
      And I press "Save"
     Then I should see "The user john is now the owner of the site Obama."
      And I verify that "john" is the owner of vsite "obama"
          # Reassign michelle to be a basic user.
      And I give the user "michelle" the role "vsite user" in the group "obama"

  @api @features_second
  Scenario: Testing the order of the people.
    Given I visit "john/people"
     Then I verify "Charles Anderson" comes before "Kristen Anderson"
