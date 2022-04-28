Feature:
  Testing ability to add links to the primary menu.

  @api @misc_second
  Scenario Outline: Test adding a link.
    Given I am logging in as "john"
     When I visit "john/cp/build/menu/link/new/primary-menu"
      And I select the radio button named "type" with value "url"
      And I press "Continue"
      And I fill in "title" with <title>
      And I populate in "url" with <url>
      And I press "Finish"
      And I visit "john"
      And I should see <title>
      And I click <title>
     Then I should see <output>

  Examples:
    | title                     | url                                             | output                                |
    | "Obama's Wikipedia Entry" | "https://en.wikipedia.org/wiki/Barack_Obama"    | "44th President of the United States" |
    | "Yale's Wikipedia Entry"  | "https://en.wikipedia.org/wiki/Yale_University" | "October 9, 1701"                     |
    | "Apple's Wikipedia Entry" | "https://en.wikipedia.org/wiki/Apple_Inc."      | "Steve Jobs"                          |
