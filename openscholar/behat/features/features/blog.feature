Feature:
  Testing the blog tab.

  @api @features_first
  Scenario: Test the Blog tab
     Given I visit "john"
      When I click "Blog"
      Then I should see "First blog"

  @api @features_first
  Scenario: Test the Blog archive
    Given I visit "john"
      And I click "Blog"
      And I should see "Blog posts by month"
     When I visit "john/blog/archive/all"
      And I should see "First blog"
      And I visit "john/blog/archive/all/201301"
     Then I should see "January 2013"
      And I should not see "First blog"

  @api @wip
  Scenario: Testing the import of blog from RSS.
    Given I am logging in as "admin"
      And I import the blog for "john"
     When I visit "john/os-importer/blog/manage"
      And I should see "John blog importer"
      And I import the feed item "NASA"
     Then I should see the feed item "NASA" was imported
      And I should see "NASA stands National Aeronautics and Space Administration."

  @api @features_first
  Scenario: Update the created date of a blog to be older than should
            be allowed.
    Given I am logging in as "john"
      And I visit "tesla/node/22/edit"
     When I fill in "Posted on" with "1901-05-30 10:43:58 -0400"
      And I press "Save"
      And I sleep for "2"
     Then I should see "Please enter a valid date for 'Posted on'"

 @api @features_first
  Scenario: Update the created date of a blog to be futher in the furture
            than allowed.
    Given I am logging in as "john"
      And I visit "tesla/node/22/edit"
     When I fill in "Posted on" with "3040-05-30 10:43:58 -0400"
      And I press "Save"
      And I sleep for "2"
     Then I should see "Please enter a valid date for 'Posted on'"

 @api @features_first @create_new_blog_content @os_blog
 Scenario: Create new blog content
    Given I am logging in as "john"
      And I visit "john/node/add/blog"
     When I fill in "Title" with "A day in the life of The POTUS."
#    When I fill in "Body" with "Each day the President is briefed."
      And I press "Save"
      And I sleep for "2"
     Then I should see "A day in the life of The POTUS"
#     And I should see "Each day the President is briefed."

 @api @features_first @edit_existing_blog_content @os_blog
 Scenario: Edit existing blog content
    Given I am logging in as "john"
     And I edit the node "A day in the life of The POTUS." in the group "john"
     When I fill in "Title" with "Another day in the life of The POTUS."
#    When I fill in "Body" with "Each day the President eats lunch."
      And I press "Save"
      And I sleep for "2"
     Then I should see "Another day in the life of The POTUS."
#     And I should see "Each day the President eats lunch."

 @api @administer_blog_settings @os_blog @javascript
 Scenario: Administer Blog Settings
    Given I am logging in as "john"
     When I visit "john/blog"
     When I make sure admin panel is open
     When I click "App Settings"
     When I click "Blog Comments"
     When I sleep for "2"
     Then I should see "Choose which comment type you'd like to use"

 @api @select_private_comments @os_blog @javascript
 Scenario: Select "Private comments"
    Given I am logging in as "john"
      And I open the "Blog Comments" settings form for the site "john"
      And I choose the radio button named "blog_comments_settings" with value "comment" for the vsite "john"
      And I press "Save"
     When I visit "john/blog"
     Then I should see "Add new comment"

 @api @javascript @select_disqus_comments @os_blog
 Scenario: Select "Disqus comments"
    Given I am logging in as "john"
      And I open the "Blog Comments" settings form for the site "john"
      And I choose the radio button named "blog_comments_settings" with value "disqus" for the vsite "john"
      And I fill in "Disqus Shortname" with "openscholar"
      And I press "Save"
     When I visit "john/blog"
     Then I should see "Add new comment"
     When I make sure admin panel is closed
     When I click "Add new comment"
     When I sleep for "7"
     Then I should see disqus

 @api @select_no_comments @os_blog @javascript
 Scenario: Select "No Comments"
    Given I am logging in as "john"
      And I visit "john/blog"
      And I make sure admin panel is open
      And I open the "Blog Comments" settings form for the site "john"
      And I choose the radio button named "blog_comments_settings" with value "nc" for the vsite "john"
      And I press "Save"
      And I sleep for "3"
      And I visit "john/blog"
     Then I should not see "Add new comment"

 @api @features_first @delete_any_blog_content @os_blog
 Scenario: Delete blog content
    Given I am logging in as "john"
      And I edit the node "Another day in the life of The POTUS." in the group "john"
      And I sleep for "2"
     When I click "Delete this blog entry"
     Then I should see "Are you sure you want to delete"
      And I press "Delete"
     Then I should see "has been deleted"

 @api @features_first @create_new_blog_content_permissions @os_blog
 Scenario: Create new blog content (permissions)
    Given I am logging in as "michelle"
     Then I can't visit "john/node/add/blog"

 @api @features_first @delete_any_blog_content_permissions @os_blog
 Scenario: Delete any blog content (permissions)
    Given I am logging in as "michelle"
     Then I can not visit "delete" form for node "blog/first-blog" in group "john"

 @api @features_first @create_new_blog_content_permissions @os_blog
 Scenario: Create new blog content (permissions)
    Given I am logging in as "michelle"
     Then I can't visit "john/node/add/blog"

 @api @features_first @edit_any_blog_content_permissions @os_blog
 Scenario: Edit any blog content (permissions)
    Given I am logging in as "michelle"
     Then I can not visit "edit" form for node "blog/first-blog" in group "john"
