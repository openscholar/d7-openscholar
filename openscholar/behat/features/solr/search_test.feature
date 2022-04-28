Feature:
  Testing search function using apache solr.

  @api @solr
  Scenario: Test basic search with apache solr
    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
      | edit-description  | search  | textfield   |
      | edit-bundle       | All     | select list |
      And I visit "john/blog"
     When I search for "john"
     Then I should see "filter by post type"

  @api @solr
  Scenario: Test the "filter by post type" facet
    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
      | edit-description  | search  | textfield   |
      | edit-bundle       | All     | select list |
      And I visit "john/blog"
     When I search for "john"
      And I click on "Class" under facet "Filter By Post Type"
     Then I should see "John F.kendy music"

  @api @solr
  Scenario: Test the "filter by taxonomy" facet
    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
    | edit-description  | search  | textfield   |
    | edit-bundle       | All     | select list |
      And I visit "john/blog"
     When I search for "john"
      And I click on "Wind" under facet "Filter By Taxonomy"
     Then I should see "JFK wikipedia page"

  @api @solr
  Scenario: Test the "sort by" facet
    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
    | edit-description  | search  | textfield   |
    | edit-bundle       | All     | select list |
      And I visit "john/blog"
     When I search for "john"
      And I click on "Title" under facet "Sort by"
     Then I should see "First blog"

  @api @solr
  Scenario: Test the usage of facets in series
    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
    | edit-description  | search  | textfield   |
    | edit-bundle       | All     | select list |
      And I visit "john/blog"
     When I search for "john"
      And I click on "Class" under facet "Filter By Post Type"
      And I should see "John F. Kennedy"
      And I should see "John F.kendy music"
      And I click on "Fire" under facet "Filter By Taxonomy"
     Then I should see "John F. Kennedy"
      And I should not see "John F.kendy music"

  @wip
  Scenario: Test the "Filter by post date" display facet UTC format.
    Given I visit ""
      And I fill in "search_block_form" with "\"Tesla's Blog\""
      And I press "Search"
      And I drill down to see the hour
     Then I verify the facet is in UTC format


