Feature: Testing that nodes can be deleted
  Test that nodes can be deleted from contextual tools

  @misc_first @javascript
  Scenario: Verify that nodes can be deleted
      Given I am logging in as "admin"
        And I visit "john/classes"
        And I make sure admin panel is open
        And I open the admin panel to "Site Content"
        And I open the admin panel to "Add"
        And I click on the "Class" control in the "li[key='0_content-1_add-2_class']" element
        And the overlay opens
        And I fill in "Title" with "Dummy class"
        And I press "Save"
        And the overlay closes
        And I mouse over "Dummy class"
        And I click on "Delete" in the tools for "Dummy class"
        And the overlay opens
       When I press "Delete"
        And the overlay closes
       Then I should verify i am at "john/classes"
