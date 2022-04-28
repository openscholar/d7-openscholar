Feature:
  Testing ability to subscribe as support team for privileged users,
  that creates an expirable membership.

  @api @vsite @javascript
  Scenario: Test supporting a site, that the subscription stays after cron, and that cron can clear it
    Given I am logging in as "bill"
    When I visit "obama"
    And I open the user menu
    And I click "Support this site"
    And the overlay opens
    And I press "Join"
    And the overlay closes
    And I open the user menu
    Then I should see "Un-Subscribe from site"
    And I execute vsite cron
    And I visit "obama"
    And I open the user menu
    Then I should not see "Support this site"
    And I set the variable "vsite_support_expire" to "1 sec"
    And I execute vsite cron
    And I visit "obama"
    And I open the user menu
    Then I should see "Support this site"

  @api @vsite @javascript
  Scenario: Test subscribe for user without permission
    Given I am logging in as "michelle"
    When I visit "obama"
     And I open the user menu
    Then I should not see "Support this site"
