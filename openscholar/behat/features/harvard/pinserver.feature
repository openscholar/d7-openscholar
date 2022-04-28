Feature:
  Testing the pinserver.

  # We did not have any tags so keep this out of the thread testing.
  @api
  Scenario: Verify the page /user/login changes when pinserver is enabled.
    Given I am not logged in
     When I enable pinserver
      And I visit "user/login"
      And I should see "Login via Harvard University ID (HUID)."
     Then I disable pinserver
      And I visit "user/login"
      And I should not see "Login via Harvard University ID (HUID)."

  @api
  Scenario: Verify pinserver is not overriding read-only mode.
    Given I am not logged in
      And I enable read-only mode
      And I enable pinserver
     When I visit "user/login"
      And I should see "Site is under maintenance, login and registration is currently disabled."
     Then I disable pinserver
      And I disable read-only mode

