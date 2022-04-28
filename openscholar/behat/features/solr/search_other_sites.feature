Feature:
  Testing search function using apache solr for searching in other sites
  (selected sites and/or subsites).

  @api @solr
  Scenario: Check search results are from the selected list of sites.
    Given I am logging in as "john"
      And the widget "Solr search" is set in the "Blog" page with the following <settings>:
      | edit-description  | search  | textfield   |
      | edit-bundle       | All     | select list |
      And I add to the search results the sites "obama"
     When I visit "john/blog"
      And I search for "john"
     Then I click on "Obama (1)" under facet "Filter by other sites"
          # Result form "john"
      And I should not see "Who was JFK?"
          # Result form "obama"
      And I should see "Me and michelle obama"

  @api @solr
  Scenario: Check search results are from the selected list of subsites.
    Given I am logging in as "alexander"
      And I add to the search results the site's subsites
      And I set the default site to "edison"
      And the widget "Solr search" is set in the "Blog" page with the following <settings>:
        | edit-description  | search  | textfield   |
        | edit-bundle       | All     | select list |
     When I visit "edison/blog"
      And I search for "blog"
     Then I click on "Tesla (1)" under facet "Filter by other sites"
          # Result form "tesla"
      And I should see "Tesla's blog"

  @api @solr
  Scenario: Verify that other site widget won't show if it's empty.

    Given I am logging in as "john"
    And the widget "Solr search" is set in the "Blog" page with the following <settings>:
      | edit-description  | search  | textfield   |
      | edit-bundle       | All     | select list |
      And I add to the search results the sites "obama"
     When I visit "john/blog"
      And I search for "music"
     Then I should not see "Filter by other sites"
