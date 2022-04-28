Feature:
  Testing that after deleting a term  which is assigned to a page,
  keeps the page accessible.

  @api @taxonomy
  Scenario: Test deletion of a term.
    Given I am logging in as "john"
      And I create the term "Water" in vocabulary "science"
      And I create a new "page" entry with the name "The dead sea" in the group "john"
      And I assign the node "The dead sea" with the type "page" to the term "Water"
      And I delete the term "Water"
      And I go to "john/dead-sea"
     Then I should get a "200" HTTP response
