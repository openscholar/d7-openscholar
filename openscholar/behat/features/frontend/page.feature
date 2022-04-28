Feature:
  Testing JS for the page edit.

  @javascript @frontend
  Scenario: Verify the page path is not changed after editing.
    Given I am logging in as "admin"
      And I visit "john/node/add/page"
      And I fill in "Title" with "Testing page"
      And I press "Save"
      And I edit the page "Testing page"
      And I save the page address
     When I fill in "Title" with "Other page"
     Then I verify the page kept the same
      And I press "Save"
      And I verify the url did not changed
