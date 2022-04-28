Feature:
  Testing the presentation tab.

  @api @features_second
  Scenario: Test the Presentation tab
    Given I visit "john"
     When I click "Presentations"
     Then I should see "JFK's biography"
      And I should see "presentation about jfk"

  @api @features_second
  Scenario: Verify that the body of a presentation is displayed in the LOP.
    Given I am logging in as "john"
      And the widget "List of posts" is set in the "Presentations" page with the following <settings>:
          | Content Type               | Presentation         | select list |
          | Display style              | Teaser               | select list |
     When I visit "john/presentations"
     Then I should see "presentation about jfk"
