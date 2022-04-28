Feature: Revisions functionality testing.

  @misc_second @api @revisions
  Scenario: Create and View revisions of a blog with the right permissions
      Given I am logging in as "john"
        And I create a new "blog" with title "My New Blog Post" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "My New Blog Post"

      Given I am logging in as "john"
        And I create a revision of "My New Blog Post" where I change the "title" to "My New Revised Blog Post"
       Then I should be able to see "1" revisions for "My New Revised Blog Post"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a blog
      Given I am logging in as "john"
        And I revert "My New Revised Blog Post" to revision "1"
       Then I should be able to see "2" revisions for "My New Blog Post"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_blog" to "2"
        And I am logging in as "john"
        And I create a revision of "My New Blog Post" where I change the "title" to "My New Revised Again Blog Post"
       Then I should be able to see "2" revisions for "My New Revised Again Blog Post"

  @misc_second @api @revisions
  Scenario: Delete the first version of a blog
      Given I am logging in as "john"
        And I delete revision "1" of "My New Revised Again Blog Post"
       Then I should be able to see "1" revisions for "My New Revised Again Blog Post"

  @misc_second @api @revisions
  Scenario: View revisions of a blog without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "My New Revised Again Blog Post"
        And I should not be permitted to "Revert" revisions for "My New Revised Again Blog Post"
        And I should not be permitted to "Delete" revisions for "My New Revised Again Blog Post"

  @misc_second @api @revisions
  Scenario: Create and View revisions of a book
      Given I am logging in as "john"
        And I create a new "book" with title "My New Book Page" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "My New Book Page"

      Given I am logging in as "john"
        And I create a revision of "My New Book Page" where I change the "title" to "My New Revised Book Page"
       Then I should be able to see "1" revisions for "My New Revised Book Page"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a book
      Given I am logging in as "john"
        And I revert "My New Revised Book Page" to revision "1"
       Then I should be able to see "2" revisions for "My New Book Page"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_book" to "2"
        And I am logging in as "john"
        And I create a revision of "My New Book Page" where I change the "title" to "My New Revised Again Book Page"
       Then I should be able to see "2" revisions for "My New Revised Again Book Page"

  @misc_second @api @revisions
  Scenario: Delete the first version of a book
      Given I am logging in as "john"
        And I delete revision "1" of "My New Revised Again Book Page"
       Then I should be able to see "1" revisions for "My New Revised Again Book Page"

  @misc_second @api @revisions
  Scenario: View revisions of a book without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "My New Revised Again Book Page"
        And I should not be permitted to "Revert" revisions for "My New Revised Again Book Page"
        And I should not be permitted to "Delete" revisions for "My New Revised Again Book Page"

  @misc_second @api @revisions
  Scenario: Create and View revisions of a faq
      Given I am logging in as "john"
        And I create a new "faq" with title "My New FAQ" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "My New FAQ"

      Given I am logging in as "john"
        And I create a revision of "My New FAQ" where I change the "title" to "My New Revised FAQ"
       Then I should be able to see "1" revisions for "My New Revised FAQ"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a faq
      Given I am logging in as "john"
        And I revert "My New Revised FAQ" to revision "1"
       Then I should be able to see "2" revisions for "My New FAQ"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_faq" to "2"
        And I am logging in as "john"
        And I create a revision of "My New FAQ" where I change the "title" to "My New Revised Again FAQ"
       Then I should be able to see "2" revisions for "My New Revised Again FAQ"

  @misc_second @api @revisions
  Scenario: Delete the first version of a faq
      Given I am logging in as "john"
        And I delete revision "1" of "My New Revised Again FAQ"
       Then I should be able to see "1" revisions for "My New Revised Again FAQ"

  @misc_second @api @revisions
  Scenario: View revisions of a faq without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "My New Revised Again FAQ"
        And I should not be permitted to "Revert" revisions for "My New Revised Again FAQ"
        And I should not be permitted to "Delete" revisions for "My New Revised Again FAQ"

  @misc_second @api @revisions
  Scenario: Create and View revisions of a link
      Given I am logging in as "john"
        And I create a new "link" with title "My New Link" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "My New Link"

      Given I am logging in as "john"
        And I create a revision of "My New Link" where I change the "title" to "My New Revised Link"
       Then I should be able to see "1" revisions for "My New Revised Link"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a link
      Given I am logging in as "john"
        And I revert "My New Revised Link" to revision "1"
       Then I should be able to see "2" revisions for "My New Link"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_link" to "2"
        And I am logging in as "john"
        And I create a revision of "My New Link" where I change the "title" to "My New Revised Again Link"
       Then I should be able to see "2" revisions for "My New Revised Again Link"

  @misc_second @api @revisions
  Scenario: Delete the first version of a link
      Given I am logging in as "john"
        And I delete revision "1" of "My New Revised Again Link"
       Then I should be able to see "1" revisions for "My New Revised Again Link"

  @misc_second @api @revisions
  Scenario: View revisions of a link without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "My New Revised Again Link"
        And I should not be permitted to "Revert" revisions for "My New Revised Again Link"
        And I should not be permitted to "Delete" revisions for "My New Revised Again Link"

  @misc_second @api @revisions
  Scenario: Create and View revisions of a news item
      Given I am logging in as "john"
        And I create a new "news" with title "My New News" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "My New News"

      Given I am logging in as "john"
        And I create a revision of "My New News" where I change the "title" to "My New Revised News"
       Then I should be able to see "1" revisions for "My New Revised News"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a news item
      Given I am logging in as "john"
        And I revert "My New Revised News" to revision "1"
       Then I should be able to see "2" revisions for "My New News"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_news" to "2"
        And I am logging in as "john"
        And I create a revision of "My New News" where I change the "title" to "My New Revised Again News"
       Then I should be able to see "2" revisions for "My New Revised Again News"

  @misc_second @api @revisions
  Scenario: Delete the first version of a news item
      Given I am logging in as "john"
        And I delete revision "1" of "My New Revised Again News"
       Then I should be able to see "1" revisions for "My New Revised Again News"

  @misc_second @api @revisions
  Scenario: View revisions of a news without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "My New Revised Again News"
        And I should not be permitted to "Revert" revisions for "My New Revised Again News"
        And I should not be permitted to "Delete" revisions for "My New Revised Again News"

  @misc_second @api @revisions
  Scenario: Create and View revisions of a page
      Given I am logging in as "john"
        And I create a new "page" with title "My New Page" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "My New Page"

      Given I am logging in as "john"
        And I create a revision of "My New Page" where I change the "title" to "My New Revised page"
       Then I should be able to see "1" revisions for "My New Revised page"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a page
      Given I am logging in as "john"
        And I revert "My New Revised Page" to revision "1"
       Then I should be able to see "2" revisions for "My New Page"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_page" to "2"
        And I am logging in as "john"
        And I create a revision of "My New Page" where I change the "title" to "My New Revised Again Page"
       Then I should be able to see "2" revisions for "My New Revised Again Page"

  @misc_second @api @revisions
  Scenario: Delete the first version of a page
      Given I am logging in as "john"
        And I delete revision "1" of "My New Revised Again Page"
       Then I should be able to see "1" revisions for "My New Revised Again Page"

  @misc_second @api @revisions
  Scenario: View revisions of a page without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "My New Revised Again Page"
        And I should not be permitted to "Revert" revisions for "My New Revised Again Page"
        And I should not be permitted to "Delete" revisions for "My New Revised Again Page"

  @misc_second @api @revisions
  Scenario: Create and View revisions of a profile
      Given I am logging in as "john"
        And I create a new "person" with title "Person Test" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "Person Test"

      Given I am logging in as "john"
        And I create a revision of "Person Test" where I change the "title" to "Person Test Revised"
       Then I should be able to see "1" revisions for "Person Test Revised"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a profile
      Given I am logging in as "john"
        And I revert "Person Test Revised" to revision "1"
       Then I should be able to see "2" revisions for "Person Test"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_person" to "2"
        And I am logging in as "john"
        And I create a revision of "Person Test" where I change the "title" to "Person Test Revised Again"
       Then I should be able to see "2" revisions for "Person Test Revised Again"

  @misc_second @api @revisions
  Scenario: Delete the first version of a profile
      Given I am logging in as "john"
        And I delete revision "1" of "Person Test Revised Again"
       Then I should be able to see "1" revisions for "Person Test Revised Again"

  @misc_second @api @revisions
  Scenario: View revisions of a profile without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "Person Test Revised Again"
        And I should not be permitted to "Revert" revisions for "Person Test Revised Again"
        And I should not be permitted to "Delete" revisions for "Person Test Revised Again"

  @misc_second @api @revisions
  Scenario: Create and View revisions of a publication
      Given I am logging in as "john"
        And I create a new "biblio" with title "My New Publication" in the "john" site
       Then I should not be able to see the "revisions" contextual link for "My New Publication"

      Given I am logging in as "john"
        And I create a revision of "My New Publication" where I change the "title" to "My New Revised Publication"
       Then I should be able to see "1" revisions for "My New Revised Publication"

  @misc_second @api @revisions
  Scenario: Revert to the first version of a publication
      Given I am logging in as "john"
        And I revert "My New Revised Publication" to revision "1"
       Then I should be able to see "2" revisions for "My New Publication"

  @misc_second @api @revisions
  Scenario: User is restricted in the number of revisions they are allowed to create
      Given I set the variable "restrict_node_revision_number_for_biblio" to "2"
        And I am logging in as "john"
        And I create a revision of "My New Publication" where I change the "title" to "My New Revised Again Publication"
       Then I should be able to see "2" revisions for "My New Revised Again Publication"

  @misc_second @api @revisions
  Scenario: Delete the first version of a publication
      Given I am logging in as "john"
        And I delete revision "1" of "My New Revised Again Publication"
       Then I should be able to see "1" revisions for "My New Revised Again Publication"

  @misc_second @api @revisions
  Scenario: View revisions of a publication without the permissions to revert/delete
      Given I am logging in as "bill"
       Then I should not be able to see the "revisions" contextual link for "My New Revised Again Publication"
        And I should not be permitted to "Revert" revisions for "My New Revised Again Publication"
        And I should not be permitted to "Delete" revisions for "My New Revised Again Publication"

  @misc_second @api @revisions
  Scenario: Should not be able to create revisions of a class
      Given I am logging in as "john"
        And I create a new "class" with title "My New Class" in the "john" site
        And I edit the node "My New Class"
       Then I should not see "fieldset" element with the class "node-form-revision-information"

  @misc_second @api @revisions
  Scenario: Should not be able to create revisions of a event
      Given I am logging in as "john"
        And I create a new "event" with title "My New Event" in the "john" site
        And I edit the node "My New Event"
       Then I should not see "fieldset" element with the class "node-form-revision-information"

  @misc_second @api @revisions
  Scenario: Should not be able to create revisions of a gallery
      Given I am logging in as "john"
        And I create a new "media gallery" with title "My New Gallery" in the "john" site
        And I edit the node "My New Gallery"
       Then I should not see "fieldset" element with the class "node-form-revision-information"

  @misc_second @api @revisions
  Scenario: Should not be able to create revisions of a presentation
      Given I am logging in as "john"
        And I create a new "presentation" with title "My New Presentation" in the "john" site
        And I edit the node "My New Presentation"
       Then I should not see "fieldset" element with the class "node-form-revision-information"

  @misc_second @api @revisions
  Scenario: Should not be able to create revisions of a software project
      Given I am logging in as "john"
        And I create a new "software project" with title "My New Software" in the "john" site
        And I edit the node "My New Software"
       Then I should not see "fieldset" element with the class "node-form-revision-information"
