Feature: Testing that when searching for a name on th site, the search doesn't
         bring back nodes authored by that name. Index of the author's name is
         should be performed only on a blog content type.

  @api @solr
  Scenario: Testing that results don't include nodes created by the searched
            author in a content type which is not a blog.
    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
      | edit-description  | search  | textfield   |
      | edit-bundle       | All     | select list |
      And I visit "john/blog"
     When I search for "john"
      And I click on "Event" under facet "Filter By Post Type"
     Then I should see "John F. Kennedy birthday"
      And I should not see "Halleys Comet"

  @api @solr
  Scenario: Testing that results include nodes created by the searched author
            in case of a blog content type.
    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
      | edit-description  | search  | textfield   |
      | edit-bundle       | All     | select list |
      And I visit "john/blog"
     When I search for "john"
      And I click on "Blog entry" under facet "Filter By Post Type"
     Then I should see "First blog"
