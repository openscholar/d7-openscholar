Feature:
  Testing the term tagged items pager.

  @api @taxonomy
  Scenario: Testing the term tagged items pager.
     Given I am logging in as "john"
      When I assign the node "John F. Kennedy" to the term "Stephen William Hawking"
       And I assign the node "I opened a new personal" with the type "news" to the term "Stephen William Hawking"
       And I assign the node "First blog" with the type "blog" to the term "Stephen William Hawking"
       And I assign the node "John doe biography" with the type "page" to the term "Stephen William Hawking"
       And I assign the node "John doe\'s curriculum" with the type "page" to the term "Stephen William Hawking"
       And I assign the node "I opened a new personal" with the type "news" to the term "Stephen William Hawking"
       And I set the variable "os_taxonomy_items_per_page" to "3"
       And I visit "john/authors/stephen-william-hawking"
      Then I should see a pager

  @api @taxonomy
  Scenario: Testing term descriptions only appearing on page 1
     Given I am logging in as "john"
      Then I visit "john/authors/stephen-william-hawking"
       And I should see "Wrote A Brief History of Time"
       And I visit "john/authors/stephen-william-hawking?page=1"
       And I should not see "Wrote A Brief History of Time"

  @api @taxonomy
  Scenario: Testing the cancel button for the reset terms to alphabetical order
     Given I am logging in as "john"
       And I visit "john/cp/build/taxonomy/authors_personal1"
      When I press "Reset to alphabetical"
       And I click "Cancel"
      Then I should see "Stephen William Hawking"
