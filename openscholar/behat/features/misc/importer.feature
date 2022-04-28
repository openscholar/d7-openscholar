Feature: Testing the importer.

  @api @misc_first
  Scenario Outline: Testing the csv importing for content types.
    Given I am logging in as "admin"
     When I visit <import-address>
      And I visit <address>
     Then I should see <title>
      And I should see <body>

  Examples:
    | import-address                            | address                                   | title                   | body                          |
    | "john/os-importer-demo/blog"              | "john/blog"                               | "Blog from csv"         | "Testing import of blog"      |
    | "john/os-importer-demo/news"              | "john/news"                               | "News from csv"         | "Testing import of news"      |
    | "john/os-importer-demo/event"             | "john/calendar?type=day&day=2020-10-17"   | "Event from csv"        | ""                            |
    | "john/os-importer-demo/page"              | "john/page-csv"                           | "Page from csv"         | "Testing import of page"      |
    | "john/os-importer-demo/class"             | "john/classes"                            | "Class from csv"        | "Testing import of class"     |
    | "john/os-importer-demo/faq"               | "john/faq"                                | "FAQ from csv"          | ""                            |
    | "john/os-importer-demo/presentation"      | "john/presentations"                      | "Presentation from csv" | ""                            |
    | "john/os-importer-demo/software_project"  | "john/software"                           | "Software from csv"     | "Testing import of software"  |
    | "john/os-importer-demo/link"              | "john/links"                              | "Link from csv"         | "Testing import of links"     |
    | "john/os-importer-demo/person"            | "john/people"                             | "Person from csv"       | "Testing import of people"    |
    | "john/os-importer-demo/media_gallery"     | "john/galleries"                          | "Gallery from csv"      | "Testing import of gallery"   |

  @api @misc_first
  Scenario: Verify that the vocabularies and terms from the CSV created
  successfully.
    Given I visit "john/blog/blog-csv"
      And I should see "Johnny B good"
      And I should see "Californication"
      And I should see "Chuck Berry"
      And I should see "Red hot chili peppers"
          # Verify that vocabularies are not being lower-cased.
     When I am logging in as "john"
      And I visit "john/cp/build/taxonomy"
     Then I should find the text "Artists"


  @api @misc_first
  Scenario: Verify the hebrew text was imported.
    Given I visit "john/blog"
     Then I should see "בדיקה של כותרת"
      And I should see "בדיקה של תוכן"

  @api @misc_first
  Scenario: Verify the spanish text was imported.
    Given I visit "john/news"
     Then I should see "Text in spanish"
      And I should see "Speaker: Luis Mendonça de Carvalho"

  @api @misc_first
  Scenario: Verify the year of a class is imported correctly.
    Given I visit "john/classes"
     Then I should see "1950"

  @pai @misc_first
  Scenario: Checking the vocabs imported properly.
    Given I am logging in as "john"
      And I visit "john/cp/build/taxonomy"
      And I should see "bad_vocab"
      And I should see "good_vocab"
     When I click "bad_vocab"
     Then I should see "Bad& (2)"
      And I should see "Good& (2)"

  @api @misc_first
  Scenario: Verify we can delete vocab with a bad name.
    Given I am logging in as "john"
      And I visit "john/cp/build/taxonomy/bad_vocab/edit"
      And I press "Delete"
     When I press "Delete"
     Then I should see "Deleted vocabulary bad_vocab."

  @api @misc_first
  Scenario: Verify that textarea length limits are respected when importing nodes from a file
    Given I am logging in as "john"
      And I set the variable "os_wysiwyg_maximum_length_body" to "50"
      And I visit "john/cp/os-importer/faq"
      And I attach the file "faq.csv" to "File"
     When I press "Submit"
     Then I should see "The value of the 'body' field can not be longer than '50' characters."
      And I delete the variable "os_wysiwyg_maximum_length_body"

