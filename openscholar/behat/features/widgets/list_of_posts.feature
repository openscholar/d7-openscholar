Feature:
  Testing that the List of posts widget shows book pages tagged
  with terms.

  @api @widgets
  Scenario: Verify that the user sees terms in the filter by term widget.
    Given I am logging in as "john"
      And I create a new "page" entry with the name "parent page" in the group "john"
      And I assign the page "parent page" to the term "Lake"
      And I create a sub page named "child page" under the page "parent page"
      And I assign the page "child page" to the term "Lake"
      And the widget "List of posts" is set in the "Blog" page with the following <settings>:
          | Content Type             | Page   | select list |
          | Display style            | Teaser | select list |
          | biology                  | Lake   | select list |
          | Include child book pages | check  | checkbox    |
     When I visit "john/blog"
     Then I should see "parent page"
      And I should see "child page"
