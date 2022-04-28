Feature:
  Testing variables.

  @api @restful
  Scenario: Testing variables overridden.
    Given I "create" the variable "name" as "john" with the value "john" in "john"
      And I "create" the variable "name" as "john" with the value "obama" in "obama"
      And I should see "john" in "john/variable/name"
      And I should see "obama" in "obama/variable/name"
     When I "update" the variable "name" as "john" with the value "john(new)" in "john"
      And I "update" the variable "name" as "john" with the value "obama(new)" in "obama"
      And I should see "john(new)" in "john/variable/name"
      And I should see "obama(new)" in "obama/variable/name"
     Then I "delete" the variable "name" as "john" with the value "john(new)" in "john"
      And I "delete" the variable "name" as "john" with the value "obama(new)" in "obama"
      And I should see "empty" in "john/variable/name"
      And I should see "empty" in "obama/variable/name"
