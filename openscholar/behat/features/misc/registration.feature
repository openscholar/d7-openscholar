Feature:
  Testing the event registration module.

  @api @wip
  Scenario: Limit the registration capacity to 1 and verify it for a normal user.
    Given I am logging in as "john"
      And I turn on event registration on "Halleys Comet"
     When I visit "john/event/halleys-comet"
      And I set the event capacity to "1"
      And I fill in "Email" with "g@gmail.com"
      And I press "Signup"
     When I am logging in as "michelle"
      And I visit "john/event/halleys-comet"
      And I should not see "Sign up for Halleys Comet"
     Then I delete "john" registration

  @api @wip
  Scenario: Limit the registration capacity to 2 and verify it for a normal user.
    Given I am logging in as "john"
     When I visit "john/event/halleys-comet"
      And I set the event capacity to "2"
      And I fill in "Email" with "g@gmail.com"
      And I press "Signup"
     When I am logging in as "michelle"
      And I visit "john/event/halleys-comet"
      And I should see "Sign up for Halleys Comet"
     Then I delete "john" registration

  @api @misc_second
  Scenario: Test adding event.
    Given I am logging in as "john"
      And I visit "john/node/add/event"
      And I fill in "Title" with "My New Event"
      And I check the box "Signup"
      And I press "Save"
     When I visit "john/calendar"
     Then I should see "My New Event"

  @api @wip
  Scenario: Test registering to event.
    Given I am logging in as "john"
      And I make "bill" a member in vsite "john"
      And I am logging in as "admin"
      And I make registration to event without javascript available
     When I am logging in as "bill"
      And I visit "john/event/my-new-event"
      And I fill in "Email" with "bill@example.com"
      And I fill in "Full name" with "Bill Clinton"
      And I fill in "Department" with "Astronomy"
      And I press "Signup"
     Then I am logging in as "john"
      And I visit "john/event/my-new-event"
      And I click "Manage Registrations"
      And I should see "bill@example.com"
      And I am logging in as "admin"
      And I make registration to event without javascript unavailable
      
  @api @misc_second
  Scenario: Verify that registration shows correctly for repeated events and the option
            to switch to another date is present.
    Given I am logging in as "john"
     When I create a new repeating event with title "Repeating Signup event" on "2021-08-03" that repeats "4" times and repeats "Weekly" with Repeat on "Fri"
      And I visit "john/calendar"
     Then I should see the event "Repeating Signup event" in the LOP
      And I visit "john/event/repeating-signup-event?delta=2"
      And I should see "Repeating Signup event"
      And I should see "Sign up for this event"
      And I should see "another date"
      
  @api @misc_second
  Scenario: Verify that registration shows correctly for non-repeated events and the option
            to switch to another date is not present.
    Given I am logging in as "john"
     When I create a new registration event with title "Johns Event"
      And I visit "john/calendar"
     Then I visit "john/event/Johns-event"
      And I should see "Johns Event"
      And I should see "Sign up for this event"
      And I should not see "another date"
      

