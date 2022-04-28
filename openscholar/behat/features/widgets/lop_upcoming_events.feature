Feature:
  Testing that the List of posts widget shows upcoming events.

  @api @widgets
  Scenario: Verify that the List of posts widget shows upcoming events.
    Given I am logging in as "john"
     When I visit "john/calendar"
     Then I should see "Upcoming events"

  @api @widgets
  Scenario: Verify that the List of posts widget shows a new upcoming event.
    Given I am logging in as "john"
     When I create a new "event" with title "New event" in the site "john"
      And I visit "john/calendar"
     Then I should see the event "New event" in the LOP

  @api @widgets
  Scenario: Verify that the List of posts widget shows a new repeating event
            and that repeats are displayed correctly.
    Given I am logging in as "john"
     When I create a new repeating event with title "Repeating event" on "2021-08-03" that repeats "3" times and repeats "Weekly" with Repeat on "Fri"
      And I visit "john/calendar"
     Then I should see the event "Repeating event" in the LOP
      And I visit "john/event/repeating-event?delta=2"
      And I should see the date of "2021-08-13" on repeat of the event
      And I should see "Repeats every week every Friday, 3 times"

  @api @widgets
  Scenario: Verify that the List of posts widget shows a "more" link when showing
            events with number of repeats higher than the number of items to be
            displayed.
    Given I am logging in as "john"
     When I create a new repeating event with title "Another Repeating event" on "2021-08-03" that repeats "5" times and repeats "Weekly" with Repeat on "Fri"
      And the widget "Upcoming events" is set in the "Software" page with the following <settings>:
          | Content Type               | Event  | select list |
          | Display style              | Title  | select list |
          | more                       | check  | checkbox    |
          | more_link                  | calendar | textfield   |
          | Number of items to display | 3      | select list |
      And I visit "john/software"
     Then I should see "more"
