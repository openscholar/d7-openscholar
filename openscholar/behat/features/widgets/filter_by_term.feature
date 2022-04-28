Feature:
  Testing the filter by term widget.

  @api @widgets
  Scenario: Verify that the user sees terms in the filter by term widget.
    Given I am logging in as "john"
      And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
        | Vocabularies           | authors             | select list |
        | Show empty terms       | check               | checkbox    |
        | Show child terms       | check               | checkbox    |
        | Taxonomy tree depth    | Show all children   | select list |
        | Show number of posts   | uncheck             | checkbox    |
     When I visit "john/publications"
     Then I should see "Filter by term"
      And I should see the following <links>
        | Antoine de Saint-Exupéry |
        | Douglas Noël Adams       |
        | Antoine de Saint-Exupéry |

  @api @widgets
  Scenario: Verify that the number of tagged posts appended to the term name.
    Given I am logging in as "john"
      And I assign the node "John F. Kennedy" to the term "Antoine de Saint-Exupéry"
      And I assign the node "John F. Kennedy" to the term "Stephen William Hawking"
      And I set the term "Stephen William Hawking" under the term "Antoine de Saint-Exupéry"
      And I set the term "Douglas Noël Adams" under the term "Stephen William Hawking"
      And the widget "Filter by term" is set in the "Classes" page with the following <settings>:
        | Vocabularies                     | authors | select list |
        | Show empty terms                 | check   | checkbox    |
        | Show number of posts             | check   | checkbox    |
        | Show child terms                 | check   | checkbox    |
     When I visit "john/classes"
     Then I should see "Antoine de Saint-Exupéry (1)"
      And I should see "Stephen William Hawking (1)"

  @api @widgets
  Scenario: Verify the widget can show/hide the child terms.
    Given I am logging in as "john"
      And I set the term "Stephen William Hawking" under the term "Antoine de Saint-Exupéry"
      And I assign the node "John F. Kennedy" to the term "Stephen William Hawking"
      And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
        | Vocabularies         | authors | select list |
        | Show empty terms     | check   | checkbox    |
        | Show child terms     | uncheck | checkbox    |
      And I visit "john/publications"
      And I should not see "Stephen William Hawking"
      And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
        | Vocabularies         | authors | select list |
        | Show empty terms     | check   | checkbox    |
        | Show child terms     | check   | checkbox    |
     When I visit "john/publications"
     Then I should see "Stephen William Hawking"

  @api @widgets
  Scenario: Verify the widget can show/hide the child terms by the depth setting.
    Given I am logging in as "john"
      And I set the term "Stephen William Hawking" under the term "Antoine de Saint-Exupéry"
      And I set the term "Douglas Noël Adams" under the term "Stephen William Hawking"
      And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
        | Vocabularies         | authors   | select list |
        | Show empty terms     | check     | checkbox    |
        | Show child terms     | check     | checkbox    |
        | Taxonomy tree depth  | 2nd Level | select list |
      And I visit "john/publications"
      And I should see "Antoine de Saint-Exupéry"
      And I should see "Stephen William Hawking"
      And I should not see "Douglas Noël Adams"
      And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
        | Vocabularies         | authors   | select list |
        | Show empty terms     | check     | checkbox    |
        | Show child terms     | check     | checkbox    |
        | Taxonomy tree depth  | 3rd Level | select list |
     When I visit "john/publications"
     Then I should see "Antoine de Saint-Exupéry"
      And I should see "Stephen William Hawking"
      And I should see "Douglas Noël Adams"

  @api @widgets
  Scenario: Verify the widget can show/hide the child terms by the depth setting.
    Given I am logging in as "john"
      And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
        | Vocabularies           | authors   | select list |
        | Show empty terms       | check     | checkbox    |
        | Show term descriptions | check     | checkbox    |
     When I visit "john/publications"
     Then I should get:
          """
          Antoine de Saint-Exupéry
          Wrote The little prince
          Stephen William Hawking
          Wrote A Brief History of Time
          Douglas Noël Adams
          Wrote The Hitchhiker's Guide to the Galaxy
          """

  @api @widgets
  Scenario: Verify the terms links direct us to the correct path.
    Given I assign the node "Me and michelle obama" with the type "blog" to the term "Barack Hussein Obama"
     When I visit the original page for the term "Barack Hussein Obama"
     Then I should not get a "200" HTTP response

  @api @widgets
  Scenario: Test that the "filter by term" is rendered with the right link.
    Given I am logging in as "john"
      And the widget "Filter by term" is set in the "Publications" page with the following <settings>:
          | Widget Description     | Taxonomy            | textfield   |
          | Vocabularies           | science             | select list |
      And I visit "john/publications"
     When I click "Air (1)"
     Then I should see "The little prince"

  @api @widgets
  Scenario: Verify that an empty term is shown if it has non-empty children.
    Given I am logging in as "john"
      And the widget "Filter by term" is set in the "Calendar" page with the following <settings>:
        | Widget Description   | Taxonomy  | textfield   |
        | Vocabularies         | authors   | select list |
        | Show empty terms     | uncheck   | checkbox    |
      And I set the term "Douglas Noël Adams" under the term "Antoine de Saint-Exupéry"
      And I set the term "Stephen William Hawking" under the term "Douglas Noël Adams"
      And I unassign the node "Halleys Comet" with the type "event" from the term "Douglas Noël Adams"
     When I visit "john/calendar"
     Then I should see "Douglas Noël Adams"
      And I should see "Antoine de Saint-Exupéry"
      And I should see "Stephen William Hawking"

  @api @widgets
  Scenario: Verify the nodes are filtered by the selected term.
    Given I am logging in as "john"
      And the widget "Filter by term" is set in the "News" page with the following <settings>:
        | Vocabularies         | science   | select list |
      And I visit "john/news"
      And I should see "This is a new site generated via the vsite options in open scholar."
      And I should see "There are more tests available on the tests list"
     When I click "Fire (1)"
     Then I should see "This is a new site generated via the vsite options in open scholar."
      And I should not see "There are more tests available on the tests list"

  @api @widgets
  Scenario: Verify automatic post type determination is working properly
    Given I am logging in as "john"
      And the widget "Filter by term" is set in the "News" page with the following <settings>:
        | Post types           | Determine for me  | select list |
        | Show number of posts | uncheck           | checkbox    |
      And I visit "john/news"
     Then I should see "Air"
      And I should not see "Air (1)"
     When the widget "Filter by term" is set in the "News" page with the following <settings>:
        | Post types           | Determine for me  | select list |
        | Show number of posts | check             | checkbox    |
    And I visit "john/news"
     Then I should see "Air (1)"

  @api @widgets
  Scenario: Verify past tagged events link to the past events view.
    Given I am logging in as "john"
      And I bind the content type "event" with "science"
      And I assign the node "Past event" with the type "event" to the term "air"
      And the widget "Filter by term" is set in the "News" page with the following <settings>:
        | Post types    | Select post type   | select list |
        | Vocabularies  | science            | select list |
        | Type          | Past event         | select list |
     When I visit "john/news"
      And I should see "Air (1)"
      And I click "Air (1)"
          # Verify only tagged events show up.
     Then I should see "Past event body"
      And I should not see "More recent past event body"

  @api @widgets
  Scenario: Verify past tagged events link to the upcoming events view.
    Given I am logging in as "john"
      And I bind the content type "event" with "authors"
      And the widget "Filter by term" is set in the "News" page with the following <settings>:
        | Post types   | Select post type   | select list |
        | Vocabularies | authors            | select list |
        | Type         | Upcoming event     | select list |
     When I visit "john/news"
      And I should see "Antoine de Saint-Exupéry (1)"
      And I click "Antoine de Saint-Exupéry (1)"
          # Verify only tagged events show up.
     Then I should see "Halley's Comet appearing"
      And I should not see "Testing event body"
