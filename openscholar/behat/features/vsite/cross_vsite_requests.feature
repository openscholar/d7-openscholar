Feature:
  In order to keep owned custom domains exclusively for only one site, as a
  visitor I should not be able to access other sites on custom domains.

  @api @wip
  Scenario: Page available on purl base domain, but not found on custom domain.
    Given I am logging in as "admin"
     When I visit "site/register"
      And I fill "edit-domain" with "cross-vsite-request"
      And I press "edit-submit"
      And I visit "cross-vsite-request"
     Then I should get a "200" HTTP response
      And I should see the text "cross-vsite-request"
     When I am on a site with a custom domain "custom.com"
      And I visit "cross-vsite-request"
     Then I should get a "404" HTTP response
      And I should not see the text "cross-vsite-request"

