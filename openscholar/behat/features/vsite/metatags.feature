Feature:
  Testing the page metatags.

  @api @vsite
  Scenario: Make sure the metatags from type link points to the correct
            path on the "href" attribute.
    Given I am logging in as "john"
     When I visit "john/about"
     Then I validate the href attribute of metatags link from type
