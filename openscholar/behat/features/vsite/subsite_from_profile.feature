Feature:
  Testing the creation of a site from a profile.

  @api @vsite
  Scenario: Test creation of a site from a profile.
    Given I am logging in as "admin"
      And I visit "als/people/henry-lou-gehrig"
      And I click "Create a personal website from this profile"
      And I fill in "URL" with "henry-lou"
      And I press "Submit"
     When I visit "henry-lou/bio"
     Then I should see "Richards buddy from Harlem"
      And I verify that the profile "Henry Lou Gehrig" has a child site named "Henry Lou Gehrig"
      And I visit "als/people/henry-lou-gehrig"
