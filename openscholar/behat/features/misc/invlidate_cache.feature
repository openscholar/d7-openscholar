Feature:
  Testing scenarios for cache invalidate.

  @api @wip
  Scenario: Verify the cache is invalidate.
    Given I am logging in as "john"
     When I set the variable "views_og_cache_invalidate_node" to "1"
      And I visit "john/people"
      And I click "Add New"
      And I click "Person"
      And I fill in "First Name" with "Foo"
      And I fill in "Last Name" with "Bar"
      And I press "Save"
      And I visit "john/people"
     Then I should see "Foo Bar"

  @api @wip
  Scenario: Verify the cache is not invalidate.
    Given I am logging in as "john"
     When I set the variable "views_og_cache_invalidate_node" to "0"
      And I visit "john/people"
      And I click "Foo Bar"
      And I click "Delete"
      And I press "Delete"
      And I visit "john/people"
      And I should see "Foo Bar"
      And I invalidate cache
      And I visit "john/people"
     Then I should not see "Foo Bar"

