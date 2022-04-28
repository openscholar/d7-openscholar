Feature:
  Testing the read-only mode.

  # We did not have any tags so keep this out of the thread testing.

  @api
  Scenario: Verify the readonly mode prevents access to /user.
    Given I am not logged in
     When I enable read-only mode
      And I visit "user"
      And I should see "Site is under maintenance, login and registration is currently disabled."
     Then I disable read-only mode
      And I visit "user"
      And I should not see "Site is under maintenance, login and registration is currently disabled."

  @api
  Scenario: Verify the readonly mode prevents access to /user/login.
    Given I am not logged in
     When I enable read-only mode
      And I visit "user/login"
      And I should see "Site is under maintenance, login and registration is currently disabled."
     Then I disable read-only mode
      And I visit "user/login"
      And I should not see "Site is under maintenance, login and registration is currently disabled."

  @api
  Scenario: Verify the readonly mode prevents access to /user/password.
    Given I am not logged in
     When I enable read-only mode
      And I visit "user/password"
      And I should see "Site is under maintenance, login and registration is currently disabled."
     Then I disable read-only mode
      And I visit "user/password"
      And I should not see "Site is under maintenance, login and registration is currently disabled."

  @api
  Scenario: Verify the readonly mode prevents access to /user/register.
    Given I am not logged in
     When I enable read-only mode
      And I visit "user/register"
      And I should see "Site is under maintenance, login and registration is currently disabled."
     Then I disable read-only mode
      And I visit "user/register"
      And I should not see "Site is under maintenance, login and registration is currently disabled."
