Feature:
  Testing that the Feed Reader widget is able to get items from an RSS feed and display them.

  @api @widgets @javascript
  Scenario: Create a feed and a feed reader widget
    Given I am logging in as "john"
      And I start creating a post of type "feed" in site "john"
      And I fill in "Title" with "Gazette"
      And I fill in "edit-field-url-und-0-url" with "http://feeds.feedburner.com/HarvardGazetteOnline"
      And I fill in "Title" with "Gazette"
      And I press "edit-submit"
      And I create a "Feed Reader" widget for the vsite "john" with the following <settings>:
          | edit-description  | Gazette Reader    | textfield   |
          | edit-title        | Gazette News Feed | textfield   |
          | edit-feed         | Gazette           | select list |
      And the widget "Gazette News Feed" is placed in the "About" layout
      And I visit "john/about"
     Then I should see "Gazette News Feed"

  @api @widgets @javascript
  Scenario: Check to see if the new feed reader widget displays feed items
    Given the widget "Gazette News Feed" is placed in the "About" layout
      And I visit "john/about"
     Then I should see "Gazette News Feed"
      And I should see "div" element with the class "feed_item"

