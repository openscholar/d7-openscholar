Feature: Media Browser
  Testing the Media Browser

  @frontend @javascript
  Scenario: Invoke the browser from the standard media field
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the entity "node" with title "About"
      And I sleep for "10"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
     Then I should see "Select files to Add"

  @frontend @javascript
  Scenario: Navigate through tabs
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the entity "node" with title "About"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should see "Drag and drop files here."
     When I click on the tab "Previously uploaded files"
      And I should see "Filename"
     When I click on the tab "Embed from the web"
      And I should see "URL or HTML:"

  @wip @javascript @apparently_theres_bugs_in_selenium?
  Scenario: Verify files show up in the "Previously uploaded files" tab
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I sleep for "10"
     When I click on "Previously uploaded files" button in the media browser
      And I should see "jfk_2.jpg" in a ".media-row" element
      And I press ">>"
      And I wait "3"
     Then I should see "slideshow1.jpg"

  @frontend @javascript
  Scenario: Test the file upload work flow for a single, valid, non-duplicate file
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I drop the file "kitten-2.jpg" onto the "Drag and drop files here." area
      And I should wait for "File Edit" directive to "appear"
      And I fill in the field "Alt Text" with the node "A cute kitten"
     When I click on the "Save" control
      And I wait for page actions to complete
      And I should see "kitten-2.jpg" in a ".file-list-single" element

  @frontend @javascript
  Scenario: Test the file upload work flow for a single, valid, duplicate file, which we replace
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I click on the tab "Previously uploaded files"
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I should see "kitten-2.jpg" in a ".media-row" element
      And I click on the tab "Upload from your computer"
      And I drop the file "duplicate/kitten-2.jpg" onto the "Drag and drop files here." area
      And I sleep for "5"
     Then I should see the text "A file with the name 'kitten-2.jpg' already exists."
      And I press the "Replace" button
      And I should wait for "File Edit" directive to "appear"
     When I click on the "Save" control
      And I confirm the file "kitten-2.jpg" in the site "john" is the same file as "duplicate/kitten-2.jpg"
      And I confirm the file "kitten-2.jpg" in the site "john" is not the same file as "kitten-2.jpg"

  @frontend @javascript
  Scenario: Test the work flow for a single, valid, duplicate file, which we rename
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I click on the tab "Previously uploaded files"
      And I should see "kitten-2.jpg" in a ".media-row" element
      And I click on the tab "Upload from your computer"
      And I drop the file "duplicate/kitten-2.jpg" onto the "Drag and drop files here." area
      And I sleep for "5"
     Then I should see the text "A file with the name 'kitten-2.jpg' already exists."
      And I press the "Rename" button
      And I should wait for "File Edit" directive to "appear"
      And I fill in the field "Alt Text" with the node "A cute kitten"
     When I click on the "Save" control
      And I wait for page actions to complete
      And I should see "kitten-2_01.jpg" in a ".file-list-single" element

  @frontend @javascript
  Scenario: Test the work flow for a single, valid, duplicate file, which we cancel
    Given I am logging in as "john"
     And I wait for page actions to complete
     And I edit the node "About" in the group "john"
    When I click on the "Upload" control
     And I wait "1 second" for the media browser to open
     And I should wait for the text "Please wait while we get information on your files." to "disappear"
     And I click on the tab "Previously uploaded files"
     And I should see "kitten-2.jpg" in a ".media-row" element
     And I click on the tab "Upload from your computer"
     And I drop the file "kitten-2.jpg" onto the "Drag and drop files here." area
     And I sleep for "5"
    Then I should wait for the text "A file with the name 'kitten-2.jpg' already exists." to "appear"
     And I press the "Cancel" button
     And I should see the media browser "Upload from your computer" tab is active
    When I click on the tab "Previously uploaded files"
    Then I should see "kitten-2.jpg" in a ".media-row" element
     And I should see "kitten-2_01.jpg" in a ".media-row" element
     And I should not see "kitten-2_02.jpg" in a ".media-row" element
     And I confirm the file "kitten-2.jpg" in the site "john" is not the same file as "kitten-2.jpg"

  @frontend @javascript
  Scenario: Test the file upload work flow for multiple, valid, non-duplicate files
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I drop the files "rubber-duck.jpg, conservatory_of_flowers3.jpg" onto the "Drag and drop files here." area
      And I sleep for "5"
      And I should see "rubber-duck.jpg" in a ".file-list-single" element
      And I should see "conservatory_of_flowers3.jpg" in a ".file-list-single" element

  @frontend @javascript
  Scenario: Test the file upload work flow for multiple, valid, duplicate files, which we cancel
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I drop the files "rubber-duck.jpg, conservatory_of_flowers3.jpg" onto the "Drag and drop files here." area
      And I sleep for "5"
      And I should see "1/2"
     When I press the "Cancel" button
     Then I should not see "1/2"
     Then I should see "2/2"
      And I click on the "Cancel" control in the ".media-browser-dupe" element
     Then I should see the media browser "Upload from your computer" tab is active

  @frontend @javascript
  Scenario: Test the file upload work flow for a single, invalid file.
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "I opened a new personal" in the group "john"
     When I click on the "Choose File" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I mouse over the ".media-browser-pane .help_icon" element
     Then I should see "jpeg jpg png"
      And I should not see "pdf"
      And I should see "Max file size: 15 MB."
      And I drop the file "abc.pdf" onto the "Drag and drop files here." area
      And I should see "abc.pdf is not an accepted file type."
      And I drop the file "Expeditionary_Fighting_Vehicle_test.jpg" onto the "Drag and drop files here." area
      And I should see "Expeditionary_Fighting_Vehicle_test.jpg is larger than the maximum filesize of 15 MB"

  @frontend @javascript
  Scenario: Test the file upload work flow for multiple valid files, some of which are duplicates and some of which are not.
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I drop the files "abc.pdf, kitten-2.jpg" onto the "Drag and drop files here." area
      And I sleep for "10"
     Then I should see "A file with the name 'kitten-2.jpg' already exists."
      And I press the "Replace" button
      And I should see "abc.pdf" in a ".file-list-single" element

  @frontend @javascript
  Scenario: Test adding a youtube video to a site
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I click on the tab "Embed from the web"
      And I fill in "URL or HTML" with "https://www.youtube.com/watch?v=jNQXAC9IVRw"
      And I press the "Submit" button
      And I should wait for "File Edit" directive to "appear"
     Then the "fe-file-name" field should contain "Me at the zoo"
      And I click on the "Save" control in the "div[file-edit]" element
      And I should see "Me at the zoo" in a ".file-list-single" element

  @frontend @javascript
  Scenario: Test adding an unknown URL to a site
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I click on the tab "Embed from the web"
      And I fill in "URL or HTML" with "http://this.is.a.fake.site.com/id/52ac3d"
      And I press the "Submit" button
      And I wait "3 seconds"
     Then I should see "HTML code are from an accepted domain."

  @frontend @javascript
  Scenario: Test adding embed codes from trusted and untrusted sources
    Given I am logging in as "john"
      And I wait for page actions to complete
      And I edit the node "About" in the group "john"
     When I click on the "Upload" control
      And I wait "1 second" for the media browser to open
      And I should wait for the text "Please wait while we get information on your files." to "disappear"
      And I click on the tab "Embed from the web"
      And I fill in "URL or HTML" with "<iframe src=\"https://untrusted.domain\"></iframe>"
      And I press the "Submit" button
      And I wait "1 seconds"
     Then I should see "HTML code are from an accepted domain."
     When I whitelist the domain "trusted.domain"
      And I fill in "URL or HTML" with "<iframe src=\"https://trusted.domain\"></iframe>"
      And I press the "Submit" button
      And I should wait for "File Edit" directive to "appear"

#  @frontend @javascript @wip
#  Scenario: Test the node save after inserting a file through WYSIWYG editor and deleting it through media browser immediately before saving
#    Given I am logging in as "john"
#      And I wait for page actions to complete
#      And I edit the entity "node" with title "About"
#      And I sleep for "10"
#      And I click on "cke_button__media" button in the wysiwyg editor
#      And I wait "1 second" for the media browser to open
#      And I should wait for the text "Please wait while we get information on your files." to "disappear"
#      And I drop the file "kitten-3.jpg" onto the "Drag and drop files here." area
#      And I should wait for "File Edit" directive to "appear"
#      And I fill in the field "Alt Text" with the node "Cute kitten"
#      And I click on the "Save" control
#      And I sleep for "10"
#      And Switch to the iframe "mediaStyleSelector"
#      And I wait for page actions to complete
#      And I click on link "Submit" under "media-browser-page"
#      And the overlay closes
#      And I press "Media browser"
#      And I wait "1 second" for the media browser to open
#      And I should wait for the text "Please wait while we get information on your files." to "disappear"
#      And I click on "Previously uploaded files" button in the media browser
#     Then I should see "kitten-3.jpg" in a ".media-row" element
#      And I click on the first "Delete" control in the ".media-row" element
#      And I press "Delete"
#      And I press "Cancel"
#      And I press "Save"
#     Then I should see "Page About has been updated"