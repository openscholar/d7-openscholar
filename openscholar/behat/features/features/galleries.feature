Feature:
  Testing the galleries tab.

  @api @wip
  Scenario: Test the Galleries tab
    Given I visit "john"
     When I click "Galleries"
      And I click "Kittens gallery"
     Then I should see the images:
      | slideshow1 |
      | slideshow2 |
      | slideshow3 |

  @api @debug @wip
  Scenario: Test the Galleries tab
    Given I visit "/user"
     Then I should print page


  @api @wip
  Scenario: Verfity that "galleries" tab shows all nodes.
    Given I visit "john/galleries/science/wind"
     Then I should see "Kittens gallery"
      And I should see "JFK"

  @api @wip
  Scenario: Verfity that "galleries" tab shows can filter nodes by term.
     Given I visit "john/galleries/science/fire"
      Then I should see "Kittens gallery"
       And I should not see "jfk"

  @api @galleries
  Scenario: Create new image gallery content
     Given I am logging in as "john"
        And I visit "john/node/add/media-gallery"
       When I fill in "Title" with "Safari"
       When I fill in "Description" with "Visit to world safari"
        And I press "Save"
        And I sleep for "2"
       Then I should see "Safari"
       And I should see "Visit to world safari"

  @api @galleries
  Scenario: Edit the existing image gallery content
     Given I am logging in as "john"
       When I visit the "edit" form for node "galleries/safari" in site "john"
       When I fill in "Title" with "World Safari"
       When I fill in "Description" with "Enjoying world safari"
        And I press "Save"
        And I sleep for "2"
       Then I should see "World Safari"
       And I should see "Enjoying world safari"

  @api @galleries @javascript
  Scenario: Add media to existing gallery
     Given I am logging in as "john"
       And I visit "john/galleries/safari"
       And I sleep for "2"
      When I click "Add media"
       And I wait "1 second" for the media browser to open
       And I should wait for the text "Please wait while we get information on your files." to "disappear"
       And I drop the file "safari.jpg" onto the "Drag and drop files here." area
       And I should wait for "File Edit" directive to "appear"
       And I fill in the field "Alt Text" with the node "safari"
       And I click on the "Save" control
       And I visit "john/galleries/safari"
     Then I should see the images:
      | safari |

  @api @galleries @javascript
  Scenario: Edit media of a existing gallery
     Given I am logging in as "john"
       And I visit "john/galleries/safari"
       And I edit the media element "safari.jpg"
       And I fill in "Title" with "safari_edited.jpg"
       And I press "Save"
       And I visit the file "safari_edited.jpg"
      Then I should see "safari_edited.jpg"

  @api @galleries @javascript
  Scenario: Delete media of a existing gallery
     Given I am logging in as "john"
       And I visit "john/galleries/safari"
       And I delete the media element "safari_edited.jpg"
       And I sleep for "5"
       And I should see "The file(s) have been removed."

  @api @features_second @javascript
   Scenario: Add slideshow image content permission
     Given I am logging in as "john"
       And I create a "Slideshow" widget for the vsite "john" with the following <settings>:
           | edit-description  | Slideshow | textfield   |
           | edit-title        | Slideshow | textfield   |
     When the widget "Slideshow" is placed in the "About" layout
      And I visit "john/about"
     Then I should see "Add Slide"
      And I click "Add Slide"
      And the overlay opens
      And I press "Save"
      And the overlay closes
      And I should see "Slideshow Image has been created"

  @api @features_second
   Scenario: Edit own slideshow image content permission
     Given I am logging in as "john"
       And I visit "john/cp/content"
       And I click "edit"
       And I fill in "Description" with "Desert Image"
       And I press "Save"
       And I should see "Slideshow Image has been updated"

  @api @features_second
   Scenario: Edit any slideshow image content permission
     Given I am logging in as "alexander"
      When I go to "john/cp/content"
      Then I should get a "403" HTTP response

  @api @features_second
   Scenario: Delete any slideshow image content permission
     Given I am logging in as "alexander"
      When I go to "john/cp/content"
      Then I should get a "403" HTTP response

  @api @features_second
   Scenario: Delete own slideshow image content permission
     Given I am logging in as "john"
       And I visit "john/cp/content"
       And I click "edit"
       And I click "Delete this slideshow image"
       And I press "Delete"
       And I should see "Slideshow Image has been deleted"

  @api @galleries @javascript
  Scenario: Delete existing image gallery content
     Given I am logging in as "john"
      When I visit the "edit" form for node "galleries/safari" in site "john"
       And I sleep for "2"
      When I click "Delete this media gallery"
      Then I should see "This action cannot be undone."
       And I press "Delete"
       Then I should see "has been deleted"

  @api @feature_second
  Scenario: Permission to add gallery Content
    Given I am logging in as "john"
      And I visit "john/cp/users/add"
      And I fill in "Member" with "alexander"
      And I press "Add member"
      And I sleep for "5"
     Then I should see "alexander has been added to the group John."
      And I visit "john/cp/users/add"
      And I fill in "Member" with "michelle"
      And I press "Add member"
      And I sleep for "5"
     Then I should see "michelle has been added to the group John."
      And I visit "user/logout"
    Given I am logging in as "michelle"
      And I visit "john/node/add/media-gallery"
     When I fill in "Title" with "Marilyn Monroe"
      And I press "Save"
      And I sleep for "2"
     Then I should see "Marilyn Monroe"

  @api @feature_second
  Scenario: Permission to edit own content
    Given I am logging in as "michelle"
      And I visit the unaliased edit path of "galleries/marilyn-monroe" on vsite "john"
      And I fill in "Title" with "Marilyn Monroe Gallery"
      And I press "Save"
     Then I should see "Marilyn Monroe Gallery"

  @api @feature_second
  Scenario: Permission to edit any content
    Given I am logging in as "alexander"
      And I visit the unaliased edit path of "galleries/marilyn-monroe" on vsite "john"
     Then I should see "Access Denied"

  @api @feature_second
  Scenario: Permission to delete any content
    Given I am logging in as "alexander"
      And I visit the unaliased delete path of "galleries/marilyn-monroe" on vsite "john"
     Then I should see "Access Denied"

  @api @feature_second
  Scenario: Permission to delete own content
    Given I am logging in as "michelle"
      And I visit the unaliased edit path of "galleries/marilyn-monroe" on vsite "john"
      And I click "Delete this media gallery"
     Then I should see "This action cannot be undone."
      And I press "Delete"
     Then I should see "has been deleted"