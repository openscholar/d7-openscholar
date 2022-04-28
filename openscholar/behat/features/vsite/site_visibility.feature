Feature:
  Testing the visibility field.

  @api @vsite
  Scenario Outline: Define the site visibility field to "Anyone with the link"
                    and test that anonymous users can view the site.
     Given I visit <request-url>
      Then I should see <text>

  Examples:
    | request-url                     | text                                                |
    | "einstein"                      | "Einstein"                                          |
    | "einstein/blog"                 | "Mileva Maric"                                      |
    | "einstein/blog/mileva-Maric"    | "Yesterday I met Mileva, what a nice girl :)."      |

  @api @vsite @javascript
  Scenario: Testing private vsite cannot be seen by anonymous users.
    Given I am logging in as "john"
     When I change privacy of the site "obama" to "Site members only. "
      And I visit "obama/user/logout"
      And I wait for page actions to complete
      And I go to "obama"
     Then I should see "Private Site"

  @api @vsite
  Scenario: Testing private vsite cannot be seen by members from another vsite.
    Given I am logging in as "alexander"
      And I go to "obama"
     Then I should see "Private Site"

  @api @vsite @javascript
  Scenario: Testing private vsite can be seen by support team members.
    Given I am logging in as "bill"
      And I go to "obama"
      And I should see "Obama" in an "h1" element
      And I open the user menu
      And I click "Support this site"
      And the overlay opens
      And I should see "Are you sure you want to join the web site"
      And I press "Join"
      And the overlay closes
     Then I should see "Your subscription request was sent."

  @api @wip
  Scenario: Testing unsubscribing a support team member.
    Given I am logging in as "bill"
      And I go to "obama"
      And I should see "Obama" in an "h1" element
      And I click "Unsubscribe obama"
      And I should see "Are you sure you want to unsubscribe from the group"
      And I press "Remove"
     Then I should see "Support obama"

  @api @vsite @javascript
  Scenario: Testing public vsite can be viewed by anonymous users.
    Given I am logging in as "john"
     When I change privacy of the site "obama" to "Public on the web. "
      And I open the user menu
      And I click "Logout"
      And I visit "obama"
     Then I should see "Obama" in an "h1" element

  @api @vsite
  Scenario: Testing public vsite can be seen by members from another vsite.
    Given I am logging in as "alexander"
      And I visit "obama"
     Then I should see "Obama" in an "h1" element
