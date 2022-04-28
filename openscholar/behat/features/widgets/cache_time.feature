Feature:
  Testing cache time value for os_boxes.

  @api @widgets
  Scenario: Test the caching time of a widget.
    Given I am logging in as "john"
      And the widget "Cache time test" is set in the "Publications" page
     When I visit "john/publications"
      And I should see "aaaa"
      And I should not see "bbbb"
      And I visit "john/publications"
      And I should see "aaaa"
      And I should not see "bbbb"
      And I sleep for "20"
     Then I visit "john/publications"
      And I should see "bbbb"
      And I should not see "aaaa"
