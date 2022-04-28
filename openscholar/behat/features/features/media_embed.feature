Feature:
  Testing the use of html code as files

  @api @features_second
  Scenario: Test that an html code will fail if it's not on the whitelist
    Given I am logging in as "john"
      And I visit "john/media/browser"
      And for "edit-embed-code" I enter "<iframe width='560' height='315' src='//www.youtube.com/embed/wl8RXCRr070' frameborder='0' allowfullscreen></iframe>"
      And I press "Submit"
     Then I should see "is from an untrusted domain."

  @api @features_second
  Scenario: Test that an html code will pass if it's on the whitelist
    Given I am logging in as "john"
      And I whitelist the domain "youtube.com"
      And I visit "john/media/browser"
      And for "edit-embed-code" I enter "<iframe width='560' height='315' src='//www.youtube.com/embed/wl8RXCRr070' frameborder='0' allowfullscreen></iframe>"
      And I press "Submit"
     Then I should not see "is from an untrusted domain."
