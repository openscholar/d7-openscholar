Feature:
  Testing the CKEDITOR is enabled.

  @javascript @frontend
  Scenario: Verify the tiny CKEDITOR is enabled.
    Given I am logging in as "admin"
     When I visit "john/node/add/blog"
     Then I should see CKEDITOR in "Body"
