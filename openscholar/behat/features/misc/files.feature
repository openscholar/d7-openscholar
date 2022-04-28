Feature:
  Testing the file deletion behavior.

  @api @misc_first
  Scenario: Verify that the when removing a file from a node, the file is not deleted.
    Given I am logging in as "john"
      And I visit "john/cp/content/files"
      And I should see "Example gallery" in the "used in" column for the row "slideshow7.jpg"
     When I remove the file "slideshow7.jpg" from the node "Example gallery" of type "media gallery"
      And I visit "john/cp/content/files"
     Then I should not see "Example gallery" in the "used in" column for the row "slideshow7.jpg"

  @api @misc_first
  Scenario: Verify that when using file_delete() on a used file, the file stays.
    Given I am logging in as "john"
      And I am deleting the file "slideshow7.jpg"
     When I visit "john/cp/content/files"
     Then I should see "slideshow7.jpg"
      And I should verify the file "slideshow7.jpg" exists

  @api @misc_first
  Scenario: Verify that the when deleting a node with attached files, the files
            are not being deleted.
    Given I am logging in as "john"
     When I visit "john/cp/content/files"
      And I should see "Example gallery" in the "used in" column
      And I delete the node of type "media_gallery" named "Example gallery"
      And I visit "john/cp/content/files"
          # Verify that the files are not being deleted.
     Then I should see "slideshow8.jpg"
      And I should see "slideshow9.jpg"
