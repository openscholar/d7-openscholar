Feature:
  Testing the viste access.

  @api @vsite
  Scenario: Testing the Vsite access to the views.
    Given I visit "news"
      And I should see "I opened a new personal"
      And I should see "Lou's site news"
      And I should see "More tests to the semester"
     When I visit "john/news"
     Then I should see "I opened a new personal"
      And I should see "More tests to the semester"
      And I should not see "Lou's site news"
     When I visit "als/news"
     Then I should not see "I opened a new personal"
      And I should not see "More tests to the semester"
      And I should see "Lou's site news"

  @api @vsite @javascript
  Scenario: Testing the robot txt when site is private
    Given I am logging in as "john"
      And I change privacy of the site "einstein" to "Public on the web."
      And I change privacy of the site "einstein" to "Anyone with the link."
      And I change privacy of the site "lincoln" to "Site members only."
      And I visit "lincoln/robots.txt"
      And I should get:
      """
      User-agent: *
Disallow: /
Disallow: /einstein/
Disallow: /lincoln/
      """
    And I change privacy of the site "lincoln" to "Public on the web."
