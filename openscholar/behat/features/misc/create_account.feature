Feature:
  Testing the hiding and showing of the "Create new account" tab in the
  home page.
  # We did not have any tags so keep this out of the thread testing.

  @api
  Scenario: Test hiding the "Create new account" tab
    Given I am logging in as "admin"
     When I visit "admin/config/people/accounts"
          # Value of 0 correspond to "Administrators only"
     Then I select the radio button named "user_register" with value "0"
      And I press "Save configuration"
      And I click "Log out"
      And I visit "user?destination=home"
     Then I should not see "Create new account"
      And I go to "user/register"
      And I should get a "403" HTTP response

  @api
  Scenario: Test showing the "Create new account" tab
    Given I am logging in as "admin"
     When I visit "admin/config/people/accounts"
          # Value of 2 correspond to "Visitors, but administrator approval is required"
     Then I select the radio button named "user_register" with value "2"
      And I press "Save configuration"
      And I click "Log out"
      And I visit "user?destination=home"
     Then I should see "Create new account"
      And I visit "user/register"
      And I should get a "200" HTTP response
