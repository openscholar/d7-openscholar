Feature: Testing OpenScholar calendar page.

  @api @features_first
  Scenario: Test the Calendar tab
    Given I visit "john"
     When I click "Calendar"
     Then I should see "Testing event"
      And I should see the link "Export" under "view-os-events"

  @api @features_first
  Scenario: Adding vocabulary for events content
    Given I am logging in as "john"
      And I create the vocabulary "event type" in the group "john" assigned to bundle "event"
      And I visit "john/cp/build/taxonomy/eventtype/add"
      And I fill in "Name" with "astronomy"
      And I check the box "Generate automatic URL alias"
     When I press "edit-submit"
     Then I verify the alias of term "astronomy" is "john/event-type/astronomy"

  @api @features_first
  Scenario: Adding term for existing events content
    Given I am logging in as "john"
      And I visit "john/cp/build/taxonomy/eventtype/add"
      And I fill in "Name" with "birthday"
      And I check the box "Generate automatic URL alias"
     When I press "edit-submit"
     Then I verify the alias of term "astronomy" is "john/event-type/astronomy"

	@api @features_first
	Scenario: Create new event and assign a term to it
    Given I am logging in as "john"
      And I create an upcoming event with title "Someone" in the site "john"
      And I assign the node "Someone" with the type "event" to the term "birthday"
     Then I should get a "200" HTTP response

  @api @features_first
  Scenario: Assigning terms to events
    Given I am logging in as "john"
      And I assign the node "John F. Kennedy birthday" with the type "event" to the term "birthday"
      And I assign the node "Halleys Comet" with the type "event" to the term "astronomy"
     Then I should get a "200" HTTP response

  @api @features_first
  Scenario: Test the 'Day' Calendar tab
    Given I visit "john/calendar?type=day&day=2020-05-29"
     Then I should see the text "John F. Kennedy birthday" under "view-display-id-page_1"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/birthday?type=day&day=2020-05-29"
     Then I should see the text "John F. Kennedy birthday" under "view-display-id-page_1"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/astronomy?type=day&day=2020-05-29"
     Then I should not see the text "John F. Kennedy birthday" under "view-display-id-page_1"

  @api @features_first
  Scenario: Test the 'Week' Calendar tab
    Given I visit "john/calendar?week=2020-W22&type=week"
     Then I should see the text "John F. Kennedy birthday" under "view-display-id-page_1"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/birthday?week=2020-W22&type=week"
     Then I should see the text "John F. Kennedy birthday" under "view-display-id-page_1"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/astronomy?week=2020-W22&type=week"
     Then I should not see the text "John F. Kennedy birthday" under "view-display-id-page_1"

  @api @features_first
  Scenario: Test the 'Month' Calendar tab
    Given I visit "john/calendar?type=month&month=2020-05"
     Then I should see the link "John F. Kennedy birthday" under "view-display-id-page_1"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/birthday?type=month&month=2020-05"
     Then I should see the link "John F. Kennedy birthday" under "view-display-id-page_1"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/astronomy?type=month&month=2020-05"
     Then I should not see the text "John F. Kennedy birthday" under "view-display-id-page_1"

  @api @features_first
  Scenario: Test the 'Year' Calendar tab
    Given I visit "john/calendar?type=year&year=2020"
     Then I should see the link "29" under "has-events"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/birthday?type=year&year=2020"
     Then I should see the link "29" under "view-display-id-page_1"
      And I should not see the link "Export" under "view-os-events"
     When I visit "john/calendar/event-type/astronomy?type=year&year=2020"
     Then I should not see the link "29" under "view-display-id-page_1"
      
  @api @features_first
  Scenario: Testing the Past Events list
    Given I visit "john/calendar/past_events"
     Then I should see the link "Past event" under "view-os-events"
      And I should not see the link "Export" under "view-os-events"
      
  @api @features_first
  Scenario: Testing the Upcoming Events list
    Given I visit "john/calendar/upcoming"
     Then I should see the link "Future event" under "view-os-events"
      And I should not see the link "Past event" under "view-os-events"
     When I click on link "iCal" under "content"
     Then I should find the text "SUMMARY:Halleys Comet" in the file
      And I should not find the text "Past event" in the file
      
  @api @features_first
  Scenario: Testing the Upcoming Events list limited by term
    Given I visit "john/calendar/upcoming/event-type/birthday"
     Then I should see the link "Someone" under "view-os-events"
      And I should not see the link "Halleys Comet" under "view-os-events"
     When I click on link "iCal" under "content"
     Then I should find the text "SUMMARY:Someone" in the file
      And I should not find the text "SUMMARY:Halleys Comet" in the file
      
  @api @features_first
  Scenario: Testing the single event export in iCal format.
    Given I visit "john/event/testing-event"
     When I click on link "iCal" under "content"
     Then I should find the text "SUMMARY:Testing event" in the file
      And I should not find the text "SUMMARY:John F. Kennedy birthday" in the file
      And I should not find the text "SUMMARY:Halleys Comet" in the file

  @api @features_first
  Scenario: Test that site-wise calendar is disabled
     Given I go to "calendar"
      Then I should get a "403" HTTP response

  @api @features_first
  Scenario: Test the next week tab
    Given I visit "john/calendar"
      And I click "Week"
      And I click "Navigate to next week"
     Then I should verify the next week calendar is displayed correctly
