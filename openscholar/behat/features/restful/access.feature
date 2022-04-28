Feature:
  Testing access.

  @api @restful
  Scenario: Testing private group content type consuming
    Given I define "obama" as a "private" group
     When I consume "api/blog/12" as "demo"
     Then I verify the request "failed"

  @api @restful
  Scenario: Testing public group content consuming
    Given I define "obama" as a "public" group
     When I consume "api/blog/12" as "demo"
     Then I verify the request "passed"

  @api @restful
  Scenario: Testing OG audience field population restrictions
    Given I try to post a "blog" as "alexander" to "john"
      And I verify it "failed"
     When I try to post a "blog" as "john" to "john"
     Then I verify it "passed"
