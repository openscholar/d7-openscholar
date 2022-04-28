Feature:
  Testing the managing of OpenScholar


  @api @features_first @javascript
  Scenario: Check that all of the apps are turned on
    Given I am logging in as "john"
      And I visit "john"
      And I make sure admin panel is open
      And I open the admin panel to "Settings"
     When I click on the "Enable / Disable Apps" control
      #And I should see "Apps"
     Then I should see the "apps-table" table with the following <contents>:
      | Blog          | The Public |
      | Booklets      | The Public |
      | Classes       | The Public |
      | Events        | The Public |
      | Media Gallery | The Public |
      | Links         | The Public |
      | News          | The Public |
      | Basic Pages   | The Public |
      | Presentations | The Public |
      | Profiles      | The Public |
      | Publications  | The Public |
      | Reader        | The Public |
      | Software      | The Public |

  @api @features_first @javascript
    Scenario: Check site owner can't manage permissions of disabled app.
      Given I am logging in as "john"
        And I set feature "Booklets" to "Disabled" on "john"
       When I visit "john/cp/users/permissions"
       Then I should not see "Create book page content"

  @api @features_first @javascript
    Scenario: Check enabling app brings back its permissions.
      Given I am logging in as "john"
        And I set feature "Booklets" to "Public" on "john"
       When I visit "john/cp/users/permissions"
       Then I should see "Create book page content"

  @api @features_first @javascript
    Scenario: Check content editor can edit widgets by default
      Given I am logging in as "john"
       When I give the user "klark" the role "content editor" in the group "john"
        And I visit "john/user/logout"
        And I am logging in as "klark"
        And I go to "john/os/widget/boxes/os_addthis/edit"
       Then I should see "AddThis" in an "h1" element

  @api @features_first @javascript
    Scenario: Check content editor without edit widgets permission can't edit
      Given I am logging in as "john"
       When I give the user "klark" the role "content editor" in the group "john"
        And I go to "john/cp/users/permissions"
       When I click "Edit roles and permissions"
        And I press "Confirm"
        And I go to "john/cp/users/permissions"
       Then I should see the button "Save permissions"
        And I press "Close Menu"
        And I remove from the role "content editor" in the group "john" the permission "edit-boxes"
        And I open the user menu
        And I click "Logout"
        And I am logging in as "klark"
        And I go to "john/os/widget/boxes/os_addthis/edit"
       Then I should not see the text "AddThis"

  @api @features_first @javascript
    Scenario: Check rolling back permissions to re-enable widget permissions
      Given I am logging in as "john"
       #When I give the user "klark" the role "content editor" in the group "john"
        And I go to "john/cp/users/permissions"
       When I click "Restore default roles and permissions"
        And I wait for the overlay to open
        And I press "Confirm"
        And the overlay closes
        And I open the user menu
        And I click "Logout"
        And I am logging in as "klark"
        And I go to "john/os/widget/boxes/os_addthis/edit"
       Then I should see "AddThis" in an "h1" element
