Feature:
  Testing the comment publishing for a blog entry.

  @api @misc_first
  Scenario: Check that a user can create a new blog post with Private comments setting
    Given I am logging in as "john"
      And I set the variable "comment_blog" to "2" in the vsite "john"
     When I visit "john/blog"
      And I click "First blog"
      And I add a comment "Private comment -- lorem ipsum john doe" using the comment form
     Then I should see the text "Private comment -- lorem" under "comment-title"
      And I should see the text "Private comment -- lorem" under "view-os-blog-comments"
