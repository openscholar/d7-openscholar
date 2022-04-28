Feature:
  Testing the simple view widget.

  @api @widgets
  Scenario: Verify the simple view widget works after tagging node to term.
     Given I am logging in as "john"
       And I set the variable "os_boxes_cache_enabled" to "1"
      When the widget "Simple view list" is set in the "Classes" page with the following <settings>:
        | authors                  | Stephen William Hawking  | select list |
        | science                  | Air                      | select list |
       And I visit "john/classes"
      Then I should not see "First blog"
       And I visit "john/classes"
      Then I should not see "First blog"
       And response header "x-drupal-cache-os-boxes-plugin" should be "os_sv_list_box"
      When I assign the node "First blog" with the type "blog" to the term "Stephen William Hawking"
       And I assign the node "First blog" with the type "blog" to the term "Air"
       And I visit "john/classes"
      Then I should see "First blog"
       And response header "x-drupal-cache-os-boxes-plugin" should not be "os_sv_list_box"
       And I visit "john/classes"
      Then I should see "First blog"
       And response header "x-drupal-cache-os-boxes-plugin" should be "os_sv_list_box"

  @api @wip
  Scenario: Verify the simple view widget pager and cache
      Given I am logging in as "john"
        And I set the variable "os_boxes_cache_enabled" to "1"
       When the widget "Simple view list" is set in the "Classes" page with the following <settings>:
         | Show pager               | check                    | checkbox    |
        And I visit "john/classes"
        And I should print page
       Then I should see "1 of 6"
        And I visit "john/classes"
       Then I should see "1 of 6"
        And response header "x-drupal-cache-os-boxes-plugin" should be "os_sv_list_box"
       When the widget "Simple view list" is set in the "Classes" page with the following <settings>:
         | Show pager               | check                    | checkbox     |
         | Number of items          | 2                        | select list  |
        And I visit "john/classes"
       Then I should see "1 of 18"
        And I visit "john/classes"
       Then I should see "1 of 18"
        And response header "x-drupal-cache-os-boxes-plugin" should be "os_sv_list_box"

