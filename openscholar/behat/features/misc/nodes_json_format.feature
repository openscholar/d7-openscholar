Feature:
  Testing the RESTWS for json output from nodes.

  @api @wip
  Scenario: Verify for the json output for a specific node.
    Given I visit "edison/node/1.json"
     Then I should see the following <json>:
          | title | Edison |
