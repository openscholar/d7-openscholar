Feature:
  Testing booklets

 @api @features_first @create_new_booklets_content @os_booklets @javascript
 Scenario: Create new booklets content
    Given I am logging in as "john"
      And I visit "john/node/add/book"
     When I fill in "Title" with "Profiles In Courage"
      And I press "Save"
      And I sleep for "5"
     Then I should see "Profiles In Courage"

 @api @features_first @edit_existing_booklets_content @os_booklets @javascript
 Scenario: Edit existing booklets content
    Given I am logging in as "john"
      And I edit the node "Profiles In Courage" in the group "john"
      And I fill in "Title" with "Profiles In Courage by John F. Kennedy and Ted Sorensen"
      And I press "Save"
      And I sleep for "5"
     Then I should see "Ted Sorensen"

 @api @features_first @os_booklets @add_child_page_to_existing_booklet_content
 Scenario: Add child page to existing booklet content
    Given I am logging in as "john"
      And I visit the site "john/book/profiles-courage"
      And I click "Add child page"
      And I fill in "Title" with "John Quincy Adams"
      And I press "Save"
      And I sleep for "5"
     Then I should see "John Quincy Adams"

 @api @features_first @os_booklets @add_a_child_page_to_existing_booklet_content
 Scenario: Add a second child page to existing booklet content
    Given I am logging in as "john"
      And I visit the site "john/book/profiles-courage"
      And I click "Add child page"
      And I fill in "Title" with "Daniel Webster"
      And I press "Save"
      And I sleep for "5"
     Then I should see "Daniel Webster"

 @api @wip @delete_any_booklets_content @os_booklets
 Scenario: Delete booklets content
    Given I am logging in as "john"
      And I edit the node "Profiles In Courage" in the group "john"
     When I click "Delete this book page"
     Then I should see "Are you sure you want to delete"
     When I sleep for "5"
      And I press "Delete"
     Then I should see "has been deleted"

  @api @wip @os_booklets @change_order_of_booklet_content_in_outline @javascript
  Scenario: change order of booklet content in outline
     Given I am logging in as "john"
       And I visit the site "john/book/profiles-courage"
       And I swap the order of the first two items in the outline on vsite "john"
      Then I should see "Updated book Profiles in Courage"
       And I visit the parent directory of the current URL
      Then I should match the regex "TABLE\s+OF\s+CONTENTS\s+Profiles\s+In\s+Courage\s+Daniel\s+Webster\s+John\s+Quincy\s+Adams"

 @api @features_first @os_booklets @add_more_child_pages_to_existing_booklet_content
 Scenario: Add a second child page to existing booklet content
    Given I am logging in as "john"
      And I visit the site "john/book/profiles-courage"
      And I click "Add child page"
      And I visit the "overlay" parameter in the current query string with "" appended on vsite ""
      And I sleep for "6"
      And I fill in "Title" with "Thomas Hart Benton"
      And I press "Save"
      And I visit the site "john/book/profiles-courage"
      And I click "Add child page"
      And I visit the "overlay" parameter in the current query string with "" appended on vsite ""
      And I sleep for "6"
      And I fill in "Title" with "Sam Houston"
      And I press "Save"

 @api @features_first @os_booklets @change_order_of_booklet_content_in_booklet_information @javascript
 Scenario: change order of booklet content in booklet information field
    Given I am logging in as "john"
      And I visit the site "john/book/profiles-courage"
      And I click "John Quincy Adams"
      And I click the gear icon in the content region
      And I click "Edit" in the gear menu
      And I visit the "destination" parameter in the current query string with "edit" appended on vsite "john"
      And I click "Booklet information"
      And I select "-- Sam Houston" from "Parent item"
      And I press "Save"
     Then I should match the regex "table\s+of\s+contents\s+profiles\s+in\s+courage\s+by\s+john\s+f.\s+kennedy\s+and\s+ted\s+sorensen\s+daniel\s+webster\s+sam\s+houston\s+john\s+quincy\s+adams"

#@api @features_first @delete_any_booklets_content @os_booklets @javascript
#Scenario: Delete booklets content
#   Given I am logging in as "john"
#    And I edit the node "Profiles In Courage" in the group "john"
#    When I click "Delete this book page"
#    Then I should see "Are you sure you want to delete"
#    When I sleep for "5"
#     And I press "Delete"
#    Then I should see "has been deleted"

 @api @wip @features_first @os_booklets @delete_booklet_content_in_outline @javascript
 Scenario: delete booklet content in outline
    Given I am logging in as "john"
      And I visit the site "john/book/profiles-courage"
      And I click the gear icon in the content region
      And I click "Outline" in the gear menu
      And I visit the "destination" parameter in the current query string with "outline" appended on vsite "john"
      And I click "delete"
     Then I should see "Are you sure you want to delete Daniel Webster?"
      And I press "Delete"
     Then I should see "Book page Daniel Webster has been deleted"
      And I click "Profiles In Courage"
     Then I should match the regex "table\s+of\s+contents\s+profiles\s+in\s+courage\s+by\s+john\s+f.\s+kennedy\s+and\s+ted\s+sorensen\s+sam\s+houston\s+john\s+quincy\s+adams"

#@api @features_first @os_booklets @correct_re_arrangement_of_booklet_outline_when_parent_is_deleted
#Scenario: correct re-arrangement of booklet outline when parent is deleted
#   Given I am logging in as "john"
#     And I visit the site "john/book/profiles-courage"

#  @api @features_first @javascript @wip
#  Scenario: Verify "Active Book's Table of Contents" widget
#    Given I am logging in as "john"
#      And I visit "john"
#
#      And I click the big gear
#      And I click "Layout"
#      And I drag the "Active Book's TOC" widget to the "header-first" region
#
#      And I visit "john/os/widget/boxes/os_booktoc/edit/cp-layout"
#      And I select "Profiles In Courage" from "Which Book"
#      And I press "Save"
#
#      And I visit "john"
#
#     Then I should match the regex "table\s+of\s+contents\s+profiles\s+in\s+courage\s+by\s+john\s+f.\s+kennedy\s+and\s+ted\s+sorensen\s+sam\s+houston\s+john\s+quincy\s+adams"

# os_booklets	widget	Recent Documents

 @api @features_first @create_new_book_content_permissions @os_booklets
 Scenario: Create new book content (permissions)
    Given I am logging in as "michelle"
     Then I can't visit "john/node/add/book"

 @api @features_first @delete_any_book_content_permissions @os_booklets
 Scenario: Delete any book content (permissions)
    Given I am logging in as "michelle"
     Then I can not visit "delete" form for node "book/profiles-courage" in group "john"

 @api @features_first @create_new_book_content_permissions @os_booklets
 Scenario: Create new book content (permissions)
    Given I am logging in as "michelle"
     Then I can't visit "john/node/add/book"

 @api @features_first @edit_any_book_content_permissions @os_booklets
 Scenario: Edit any book content (permissions)
    Given I am logging in as "michelle"
     Then I can not visit "edit" form for node "book/profiles-courage" in group "john"

 @api @features_first @delete_any_book_content_permissions @os_booklets
 Scenario: Delete any book content (permissions)
    Given I am logging in as "michelle"
     Then I can not visit "delete" form for node "book/profiles-courage" in group "john"
