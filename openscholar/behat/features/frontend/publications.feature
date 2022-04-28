Feature:
  Checking the publication author date date picker.

  @javascript @frontend
  Scenario: Checking the publication author date date picker.
    Given I am logging in as "john"
      And I set the variable "biblio_citeproc_style" to "harvard-chicago-author-date.csl" in the vsite "john"
     When I visit "john/node/add/biblio"
      And I verifying the date picker behaviour
      And I create a new publication with a type

  @javascript @frontend
  Scenario: Checking the publication author date date picker.
    Given I am logging in as "john"
     When I visit "john/node/add/biblio"
      And I create a new publication with a date picker
