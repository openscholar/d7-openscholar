Feature:
  Testing the OS reader functionality.

  @api @features_second
  Scenario: Test the Reader tab
    Given I visit "john"
     When I click "Reader"
     Then I should see "Engadget rss"

  @api @features_second
  Scenario: Test the OS reader feed importer.
    Given I am logging in as "admin"
      And I import feed items for "john"
     When I visit "john/cp/os-importer/news/manage"
      And I should see "John news importer"
      And I import the feed item "JFK was murdered"
     Then I should see the feed item "JFK was murdered" was imported
      And I should see "JFK was murdered"

  @api @features_second
  Scenario: Verify the anonymous user is being redirected to the feed item
            source page.
    Given I visit "john/news"
     When I click "JFK was murdered"
     Then I should see "Assassination of John F. Kennedy"

  @api @features_second 
  Scenario: Feed items are displayed for each site
    Given I am logging in as "admin"
      And I import feed items for "obama"
     When I visit "obama/cp/os-importer/news/manage"
      And I should see "Obama news importer"
      And I should not see "John news importer"
      And I should not see "JFK was murdered"
      And I import the feed item "Four more years is the most re-tweeted tweet"
     Then I should see the feed item "Four more years is the most re-tweeted tweet" was imported
      And I should see "Four more years is the most re-tweeted tweet"

  @api @features_second
  Scenario: Verify images in feed item description are imported as images.
    Given I am logging in as "admin"
     When I visit "john/cp/os-importer/news/manage"
      And I should see the feed item "JFK was murdered" was imported
     Then I should see the news photo "Harvard_shield-College"

  @api @features_second
  Scenario: Test the Contains filter on OS feed import page.
    Given I am logging in as "admin"
      And I visit "john/cp/os-importer/news/manage?feed_by_text=president"
      And I should see "JFK was murdered"
     When I visit "john/cp/os-importer/news/manage?feed_by_text=pancakes"
     Then I should not see "JFK was murdered"

  @api @features_second
  Scenario: Test the Status filter on OS feed import page.
    Given I am logging in as "admin"
     When I visit "john/cp/os-importer/news/manage?feed_is_imported=All"
     Then I should see "JFK was murdered"

  @api @features_second
  Scenario: Test the Importer filter on OS feed import page.
    Given I am logging in as "admin"
      And I visit "john/cp/os-importer/news/manage"
     When I click "John news importer"
     Then I should see "JFK was murdered"

  @api @features_second
  Scenario: Verify the imported news date is the original feed item date.
    Given I am logging in as "admin"
      And I visit "john/cp/os-importer/news/manage"
      And I import the feed item "Lee Harvey Oswald"
     When I visit "john/news"
      And I click "Lee Harvey Oswald"
     Then I should see "November 22, 1963"

  @api @features_second
  Scenario: Verify a the same feed can be imported to two different vsites.
    Given I am logging in as "john"
      And I import "john" feed items for "obama"
     When I visit "obama/cp/os-importer/news/manage"
     Then I should see "JFK was murdered"

  @js @features_second
  Scenario: Verify a feed item can be imported only once to the site.
    Given I am logging in as "admin"
     When I re import feed item "John news importer"
     Then I verify the feed item "JFK was murdered" exists only "1" time for "john"
      And I re import feed item "John news importer"
     Then I verify the feed item "JFK was murdered" exists only "1" time for "john"
