Feature:
  Testing the os_profiles app settings.

  @api @features_second
  Scenario: Test that changing the display type affects the "/people" path,
            with display type "title".
    Given I am logging in as "john"
     When I go to the "os_profiles" app settings in the vsite "john"
      And I choose the radio button named "os_profiles_display_type" with value "title" for the vsite "john"
      And I press "Save configuration"
      And I invalidate cache
      And I visit "john/people"
     Then I should not see "Drupal developer at Gizra.inc"

  @api @features_second
  Scenario: Test that changing the display type affects the people term pages,
            for example: "john/people/science/air".
    Given I am logging in as "john"
     When I go to the "os_profiles" app settings in the vsite "john"
      And I choose the radio button named "os_profiles_display_type" with value "sidebar_teaser" for the vsite "john"
      And I press "Save configuration"
      And I invalidate cache
      And I visit "john/people/science/wind"
     Then I should see "Actress"
      And I should not see "AKA Marilyn Monroe"

  @api @features_second
  Scenario: Test that changing the display type affects in a vsite, doesn't affect
            other vsites.
    Given I am logging in as "john"
     When I go to the "os_profiles" app settings in the vsite "john"
      And I choose the radio button named "os_profiles_display_type" with value "sidebar_teaser" for the vsite "john"
      And I press "Save configuration"
      And I invalidate cache
      And I visit "obama/people"
     Then I should see "michelle@whitehouse.gov"

  @api @features_second
  Scenario: Test that changing the display type affects the "/people" path,
            With display type "teaser".
    Given I am logging in as "john"
     When I go to the "os_profiles" app settings in the vsite "john"
      And I choose the radio button named "os_profiles_display_type" with value "teaser" for the vsite "john"
      And I press "Save configuration"
      And I invalidate cache
      And I visit "john/people"
     Then I should see "Actress"
     Then I should see "AKA Marilyn Monroe"

  @api @features_second
  Scenario: Test that the prefix field of a profile is displayed correctly.
    Given I am logging in as "john"
      And I edit the node "John Fitzgerald Kennedy"
      And I fill in "Prefix" with "Mr."
      And I press "edit-submit"
     When I visit "john/people"
     Then I should see "Mr. John Fitzgerald Kennedy"
          # Remove the Prefix.
      And I edit the node "John Fitzgerald Kennedy"
      And I fill in "Prefix" with ""
      And I press "edit-submit"
      And I visit "john/people"
      And I should see "John Fitzgerald Kennedy"
      And I should not see "Mr. John Fitzgerald Kennedy"
