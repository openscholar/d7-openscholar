Feature:
  Testing node type creating.

  @api @misc_first
  Scenario: Verify we can create node type.
    Given I am logging in as "admin"
      And I visit "admin/structure/types/add"
     When I fill in "Name" with "Foo"
     When I fill in "Machine-readable name" with "foo"
      And I press "Save content type"
     Then I should see "The content type Foo has been added."
