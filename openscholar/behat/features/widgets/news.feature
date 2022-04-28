Feature:
  Testing news widget.

  @api @widgets @javascript
  Scenario: Verify "Latest News" widget
    Given I am logging in as "john"
      And I visit "john"
      And I click the big gear
      And I click "Layout"
      And I drag the "Latest News" widget to the "sidebar-first" region
      And I visit "john"
      And I should see "LATEST NEWS"