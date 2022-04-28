Feature:
  Testing the viewing of list of subsites, which should be allowed only to
  admins.

  @api @vsite
  Scenario: Test view subsites for user with permission
    Given I am logging in as "alexander"
     When I visit "edison/subsites"
     Then I should see "Tesla"

  @api @vsite
  Scenario: Test view subsites for user with no permission
    Given I am logging in as "michelle"
     When I go to "edison/subsites"
     Then I should get a "403" HTTP response
