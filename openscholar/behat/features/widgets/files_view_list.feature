Feature:
  Testing the files view widget.

  @api @wip
  Scenario: Verify the files view widget works after tagging node to term.
     Given I am logging in as "john"
      When the widget "Files view list" is set in the "Classes" page with the following <settings>:
         | display                  | Title  | select list |
       And I visit "john/classes"
       And I should see "media_gallery.csv"
       And I visit "john/classes"
       And I should see "media_gallery.csv"
       And response header "x-drupal-cache-os-boxes-plugin" should be "os_sv_list_file"
      Then I edit the entity "file" with title "media_gallery.csv"
       And I fill in "edit-filename" with "changed file"
       And I press "edit-submit"
       And I visit "john/classes"
      Then I should not see "media_gallery.csv"
       And I should see "changed file"
       And I visit "john/classes"
       And response header "x-drupal-cache-os-boxes-plugin" should be "os_sv_list_file"
