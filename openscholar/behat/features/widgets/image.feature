Feature:
  Testing the image gallery widget.

  @api @wip
  Scenario: Verify that the image gallery widget works fine.
      Given I am logging in as "john"
       And the widget "Image gallery" is set in the "Publications" page with the following <settings>:
            | Gallery | Kittens | select list |
        And I visit "john/publications"
       Then I should see the images:
            | slideshow1 |
