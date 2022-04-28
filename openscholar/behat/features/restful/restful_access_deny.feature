Feature:
  Verify access for users

  @api @restful
  Scenario: Verify a normal user can't create a box.
    Given I try to "create" a box as "demo" in "john"
