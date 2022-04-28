Feature:
  Testing browse function using apache solr.

  @api @wip
  Scenario: Test basic people browse with apache solr
    Given I am logging in as "john"
      And the widget "Faceted taxonomy" is set in the "People" page by the name "people" with the following <settings>:
        | Content type           | Person              | select list |
      And I am not logged in
      And I visit "john/browse/people"
     Then I should see "faceted taxonomy"
      And I should see the text "People" under "content-inner"
      And I should see "Norma"
     Then I click "Air"
      And I should not see "Norma"

  @api @wip
  Scenario: Test browse with one term not showing categories block
    Given I am logging in as "john"
      And I change privacy of the site "obama" to "Public on the web. "
     Then I am not logged in
      And I visit "obama/browse/blogs"
     Then I should not see "faceted taxonomy"
      And I should see the text "Blog" under "content-inner"
      And I should see "Me and michelle obama"

  @api @wip
  Scenario: Test direct browsing of category
    Given I am logging in as "john"
      And the widget "Faceted taxonomy" is set in the "Galleries" page by the name "galleries" with the following <settings>:
        | Content type           | Media Gallery              | select list |
      And I am not logged in
    Given I visit "john/browse/gallery/?f[0]=sm_og_vocabulary%3Ataxonomy_term%3A3"
     Then I should see "faceted taxonomy"
      And I should see the text "Media Galleries" under "content-inner"
      And I should not see "John doe biography"
      And I should see "Kittens gallery"
