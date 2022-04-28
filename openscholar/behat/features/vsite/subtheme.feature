Feature:
  Testing the term tagged items pager.

  @api @vsite
  Scenario: Testing the term tagged items pager.
    Given I am adding the subtheme "subtheme" in "john"
      And I define the subtheme "subtheme" of the theme "cleanblue" as the theme of "john"
     When I visit "/john"
     Then I should verify the existence of the css "subtheme.css"
      And I should verify the existence of the js "subtheme.js"
