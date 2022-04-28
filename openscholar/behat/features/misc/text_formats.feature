Feature:
  Testing the text formats

  @api @misc_second
  Scenario: Verify that <th> and <thead> tags show up in the filtered html text
            format.
    Given I visit "john/blog/third-blog"
     Then I should see a table with the text "Header" in its header
