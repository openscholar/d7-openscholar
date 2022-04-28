Feature:
  Testing that the List of posts widget shows publications in the order
  of Year/Sticky/Created date.

  @api @widgets
  Scenario: Verify that the List of posts widget shows publications in the order
            of Year.
    Given I am logging in as "john"
      And the widget "List of publications" is set in the "Publications" page with the following <settings>:
          | Content Type               | Biblio               | select list |
          | Display style              | Title                | select list |
          | Sorted By                  | Year of Publication  | select list |
     When I visit "john/publications"
     Then I should see the publication "John F. Kennedy: A Biography" comes before "Goblet of Fire" in the LOP widget

  @api @wip
  Scenario: Verify that the List of posts widget shows publications in the order
            of Year and then by sticky.
    Given I am logging in as "john"
      And I make the node "Chamber of Secrets" sticky
      And the widget "List of publications" is set in the "Publications" page with the following <settings>:
          | Content Type               | Publication          | select list |
          | Display style              | Title                | select list |
          | Sorted By                  | Year of Publication  | select list |
     When I visit "john/publications"
     Then I should see the publication "John F. Kennedy: A Biography" comes before "Chamber of Secrets" in the LOP widget
      And I should see the publication "Chamber of Secrets" comes before "Goblet of Fire" in the LOP widget

  @api @wip
  Scenario: Verify that the List of posts widget shows publications in the order
            of Year and then by sticky and then by created.
    Given I am logging in as "john"
      And I edit the node "Goblet of Fire"
      And I fill in "date[date]" with "01/01/2014"
      And I press "edit-submit"
      And I edit the node "Chamber of Secrets"
      And I fill in "date[date]" with "01/01/1999"
      And I press "edit-submit"
      And the widget "List of publications" is set in the "Publications" page with the following <settings>:
          | Content Type               | Publication          | select list |
          | Display style              | Title                | select list |
          | Sorted By                  | Year of Publication  | select list |
     When I visit "john/publications"
     Then I should see the publication "John F. Kennedy: A Biography" comes before "Chamber of Secrets" in the LOP widget
      And I should see the publication "Chamber of Secrets" comes before "Goblet of Fire" in the LOP widget
