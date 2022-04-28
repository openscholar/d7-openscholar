Feature:
  Testing the active book TOC widget.

  @api @widgets
  Scenario: Verify that the active book TOC widget works fine.
     Given I am logging in as "john"
       And the widget "Active book TOC" is set in the "Publications" page with the following <settings>:
           | Which Book | All about nodes | select list |
       And I visit "john/publications"
      Then I should see the link "All about nodes"
