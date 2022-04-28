Feature:
  Testing the cp content views.

  @api @misc_first
  Scenario: Verify that the "Used in" column is populated correctly in the
            "cp_files" view.
    Given I am logging in as "john"
     When I visit "john/cp/content/files"
     Then I should see "Kittens gallery" in the "used in" column
