Feature:
  Testing the OS Reports for sites feature

  @api @wip
  Scenario: Trying to view the report form without the proper access
      Given I am logging in as "john"
       Then I can't visit "/admin/reports/os/site"

  @api @wip
  Scenario: Trying to view the report form with the proper access
      Given I am logging in as "admin"
        And I go to "/admin/reports/os/site"
       Then I should see the text "Sites Report"

  @api @wip
  Scenario: Running a site report with all available optional columns
      Given I am logging in as "admin"
        And I run the "site" report with "Optional Columns" <checked>:
            | Creation date                |
            | Created by                   |
            | Last content update          |
            | Non-content changes          |
            | Privacy level                |
            | Custom domain                |
            | Custom theme uploaded        |
            | Site type/preset             |
       Then I will see a report with content in the following <columns>:
            | site title            | populated       |
            | site owner email      | populated       |
            | os install            | populated       |
            | site created          | populated       |
            | site created by       | populated       |
            | content last updated  | may be blank    |
            | other site changes    | may be blank    |
            | site privacy setting  | populated       |
            | custom domain         | populated       |
            | custom theme uploaded | populated       |
            | preset                | populated       |
            | site url              | populated       |

  @api @wip
  Scenario: Running a site report that searches site owner email addresses for a keyword
      Given I am logging in as "admin"
        And I run the "site" report with "keyword" set to "gov" and <checkboxes> selected:
            | email |
       Then I will see a report with the following <rows>:
            | site title | site owner email   | os install | site url |
            | Edison     | alexander@bell.gov |            | edison   |
            | John       | jfk@whitehouse.gov |            | john     |
            | Obama      | jfk@whitehouse.gov |            | obama    |
            | Abraham    | jfk@whitehouse.gov |            | lincoln  |
            | Einstein   | jfk@whitehouse.gov |            | einstein |
            | Tesla      | jfk@whitehouse.gov |            | tesla    |

  @api @wip
  Scenario: Running a site report that searches site owner email addresses for a keyword
      Given I am logging in as "admin"
        And I run the "site" report with "keyword" set to "john" and <checkboxes> selected:
            | username |
       Then I will see a report with the following <rows>:
            | site title | site owner email   | os install | site url |
            | John       | jfk@whitehouse.gov |            | john     |
            | Obama      | jfk@whitehouse.gov |            | obama    |
            | Abraham    | jfk@whitehouse.gov |            | lincoln  |
            | Einstein   | jfk@whitehouse.gov |            | einstein |
            | Tesla      | jfk@whitehouse.gov |            | tesla    |

  @api @wip
  Scenario: Running a site report that searches site owner email addresses for a keyword
      Given I am logging in as "admin"
        And I run the "site" report with "keyword" set to "john" and <checkboxes> selected:
            | site title |
       Then I will see a report with the following <rows>:
            | site title | site owner email   | os install | site url |
            | John       | jfk@whitehouse.gov |            | john     |

  @api @wip
  Scenario: Running a site report that searches site owner email addresses for a keyword
      Given I am logging in as "admin"
        And I run the "site" report with "keyword" set to "Rumpelstiltskin" and <checkboxes> selected:
            | email      |
            | username   |
            | site title |
       Then I will see a report with no results

  @api @wip
  Scenario: Running a site report limited by content last updated dates
      Given I am logging in as "admin"
        And I run the "site" report with "Content last updated before" set to the "beginning" of this "year"
       Then I will see a report with no results

  @api @wip
  Scenario: Running a site report limited by content last updated dates
      Given I am logging in as "admin"
        And I run the "site" report with "Content last updated before" set to the "end" of this "year"
       Then I will see a report with "content last updated" values "less than or equal" to the "end" of this "year"

  @api @wip
  Scenario: Running a site report limited by site creation date start
      Given I am logging in as "admin"
        And I run the "site" report with "Creation start" set to the "beginning" of this "year"
       Then I will see a report with "site created" values "greater than or equal" to the "beginning" of this "year"

  @api @wip
  Scenario: Running a site report limited by site creation date end
      Given I am logging in as "admin"
        And I run the "site" report with "Creation start" set to the "end" of this "year"
       Then I will see a report with no results

  @api @wip
  Scenario: Running a site report limited by site creation date end
      Given I am logging in as "admin"
        And I run the "site" report with "Creation end" set to the "beginning" of this "year"
       Then I will see a report with no results

  @api @wip
  Scenario: Running a site report limited by site creation date start
      Given I am logging in as "admin"
        And I run the "site" report with "Creation end" set to the "end" of this "year"
       Then I will see a report with "site created" values "less than or equal" to the "end" of this "year"
